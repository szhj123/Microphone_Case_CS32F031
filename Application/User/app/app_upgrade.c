/********************************************************
* @file       app_upgrade.c
* @author     szhj13
* @version    V1.0
* @date       2023-05-06
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes -----------------------------------------------*/
#include "app_upgrade.h"
#include "app_com.h"
#include "app_hall.h"
#include "app_flash.h"
/* Private typedef --------------------------------------*/
/* Private define ----------------------------------------*/
/* Private macro ---------------------------------------*/
/* Private function -------------------------------------*/
static void App_Upg_Handler(void *arg );
/* Private variables ------------------------------------*/
static upg_para_t upgPara;

void App_Upg_Init(void )
{
    Drv_Task_Regist_Period(App_Upg_Handler, 0, 1, NULL);
}

static void App_Upg_Handler(void *arg )
{
    if(upgPara.delayCnt < 0xffff)
    {
        upgPara.delayCnt++;
    }
    
    switch(upgPara.stat)
    {
        case UPG_STAT_GET_VER:
        {
            if(App_Hall_Get_State() == HALL_OPEN)
            {
                upgPara.appVer = VER_APP;
                
                App_Com_Tx_Cmd_Get_Fw_Ver();
           
                upgPara.delayCnt = 0;
                
                upgPara.stat = UPG_STAT_COMPARE_VER;
            }
            break;
        }
        case UPG_STAT_COMPARE_VER:
        {
            if(upgPara.delayCnt > 500)
            {
                upgPara.delayCnt = 0;

                if(upgPara.responseFlag)
                {
                    upgPara.responseFlag = 0;
                    
                    if(upgPara.appVer != 0)
                    {
                        if(upgPara.appVer != VER_APP)
                        {                            
                            upgPara.stat = UPG_STAT_START;
                        }
                        else
                        {
                            upgPara.stat = UPG_STAT_EXIT;
                        }
                    }
                    else
                    {
                        upgPara.stat = UPG_STAT_GET_VER;
                    }
                }
                else
                {
                    App_Com_Tx_Cmd_Get_Fw_Ver();
                }
            }
            break;
        }
        case UPG_STAT_START:
        {
            if(App_Hall_Get_State() == HALL_CLOSE)
            {
                if(upgPara.delayCnt > 300)
                {
                    upgPara.delayCnt = 0;

                    //Todo: get firmware information from earbud
                    App_Com_Tx_Cmd_Get_Fw_Size();
                    
                    upgPara.stat = UPG_STAT_GET_FW_INFO;
                }
            }
            else
            {
                upgPara.delayCnt = 0;

                upgPara.stat = UPG_STAT_GET_VER;
            }
            break;
        }
        case UPG_STAT_GET_FW_INFO:
        {
            if(upgPara.responseFlag)
            {
                upgPara.responseFlag = 0;

                if(upgPara.fwSize)
                {
                    App_Flash_Set_Fw_Size(upgPara.fwSize);
                    
                    App_Flash_Erase_App2();

                    upgPara.fwOffset = 0;

                    App_Com_Tx_Cmd_Get_Fw_Data(upgPara.fwOffset, FW_DATA_PACK_MAX_SIZE);

                    upgPara.stat = UPG_STAT_GET_FW_DATA;
                }
            }
            else 
            {
                if(App_Hall_Get_State() == HALL_OPEN)
                {
                    upgPara.delayCnt = 0;
                    
                    upgPara.stat = UPG_STAT_GET_VER;
                }
            }
            break;
        }
        case UPG_STAT_GET_FW_DATA:
        {
            if(upgPara.responseFlag)
            {
                upgPara.responseFlag = 0;
                
                App_Flash_Write_App2(upgPara.fwOffset, upgPara.fwDataBuf, upgPara.fwDataLen);

                upgPara.fwOffset += FW_DATA_PACK_MAX_SIZE;

                if(upgPara.fwOffset < upgPara.fwSize)
                {
                    if((upgPara.fwSize - upgPara.fwOffset) > FW_DATA_PACK_MAX_SIZE)
                    {
                        App_Com_Tx_Cmd_Get_Fw_Data(upgPara.fwOffset, FW_DATA_PACK_MAX_SIZE);
                    }
                    else
                    {
                        App_Com_Tx_Cmd_Get_Fw_Data(upgPara.fwOffset, upgPara.fwSize - upgPara.fwOffset);
                    }
                }
                else
                {                    
                    upgPara.fwOffset = upgPara.fwSize;
                    
                    App_Com_Tx_Cmd_Get_Fw_CRC();
                    
                    upgPara.stat = UPG_STAT_GET_FW_CRC;
                }
            }
            else 
            {
                if(App_Hall_Get_State() == HALL_OPEN)
                {
                    upgPara.delayCnt = 0;
                    
                    upgPara.stat = UPG_STAT_GET_VER;
                }
            }
            break;
        }
        case UPG_STAT_GET_FW_CRC:
        {
            if(upgPara.responseFlag)
            {
                upgPara.responseFlag = 0;

                uint16_t calFwCrc = App_Flash_Cal_Fw_Checksum();

                if(upgPara.fwCrc == calFwCrc)
                {
                    App_Flash_Upg_Enable();

                    App_Jump_to_Bld();
                }

                upgPara.stat = UPG_STAT_EXIT;
            }
            else 
            {
                if(App_Hall_Get_State() == HALL_OPEN)
                {
                    upgPara.delayCnt = 0;
                    
                    upgPara.stat = UPG_STAT_GET_VER;
                }
            }
            break;
        }
        case UPG_STAT_EXIT:
        {
            if(App_Hall_Get_State() == HALL_OPEN)
            {
                upgPara.stat = UPG_STAT_GET_VER;
            }
            break;
        }
        default: break;
    }
}

void App_Upg_Set_Fw_Ver(uint8_t bldVer, uint8_t appVer, uint8_t hwVer )
{
    upgPara.bldVer = bldVer;
    upgPara.appVer = appVer;
    upgPara.hwVer  = hwVer;

    upgPara.responseFlag = 1;
}

void App_Upg_Set_Fw_Size(uint32_t fwSize )
{
    upgPara.fwSize = fwSize;

    upgPara.responseFlag = 1;
}

void App_Upg_Set_Fw_Data(uint8_t *buf, uint8_t length )
{
    uint8_t i;

    for(i=0;i<length;i++)
    {
        upgPara.fwDataBuf[i] = buf[i];
    }

    upgPara.fwDataLen = length;

    upgPara.responseFlag = 1;
}

void App_Upg_Set_Fw_CRC(uint16_t fwCrc )
{
    upgPara.fwCrc = fwCrc;

    upgPara.responseFlag = 1;
}

uint16_t App_Upg_Get_Fw_Data_Offset(void )
{
    return upgPara.fwOffset;
}

