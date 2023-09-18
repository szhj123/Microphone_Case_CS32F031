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
#include "app_batt.h"
#include "app_hall.h"
#include "app_flash.h"
/* Private typedef --------------------------------------*/
/* Private define ----------------------------------------*/
/* Private macro ---------------------------------------*/
/* Private function -------------------------------------*/
static void App_Upg_Handler(void *arg );
static void App_Risk_Handler(void *arg );
/* Private variables ------------------------------------*/
static upg_para_t upgPara;
static sirk_para_t sirkPara;

void App_Upg_Init(void )
{
    Drv_Task_Regist_Period(App_Upg_Handler, 0, 1, NULL);

    Drv_Task_Regist_Period(App_Risk_Handler, 0, 1, NULL);
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
                if(App_Ebud_Get_Rx_Chrg_Stat() != EBUD_CHRG_NONE)
                {
                    upgPara.verApp = 0;
                    
                    upgPara.delayCnt = 0;
                                        
                    App_Com_Tx_Cmd_Get_Fw_Ver();
                    
                    upgPara.stat = UPG_STAT_COMPARE_VER;
                }
            }
            break;
        }
        case UPG_STAT_COMPARE_VER:
        {
            if(upgPara.delayCnt > 250)
            {
                upgPara.delayCnt = 0;
                
                if(upgPara.responseFlag)
                {
                    upgPara.responseFlag = 0;
                    
                    if(upgPara.verApp != VER_APP)
                    {                            
                        upgPara.stat = UPG_STAT_START;
                    }
                }
                else
                {
                    if(++upgPara.reConnectCnt >= 3)
                    {
                        Drv_Com_Tx_Disable(COM1);
                        
                        Drv_Com_Tx_Disable(COM2);

                        Drv_Com_Tx_Disable(COM3);
                        
                        upgPara.reConnectCnt = 0;

                        upgPara.delayCnt = 0;
                        
                        upgPara.stat = UPG_STAT_FORCE_CHARGE;

                        return ;
                    }
                    else
                    {
                        upgPara.stat = UPG_STAT_GET_VER;
                    }
                }
            }

            if(upgPara.verApp == VER_APP)
            {
                if(App_Hall_Get_State() == HALL_CLOSE)
                {
                    upgPara.stat = UPG_STAT_EXIT;
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
        case UPG_STAT_FORCE_CHARGE:
        {
            if(App_Hall_Get_State() == HALL_CLOSE)
            {
                upgPara.stat = UPG_STAT_EXIT;
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

static void App_Risk_Handler(void *arg )
{
    if(sirkPara.delayCnt < 0xffff)
    {
        sirkPara.delayCnt++;
    }
    
    switch(sirkPara.stat)
    {
        case SIRK_STAT_GET_DATA:
        {
            //if(App_Ebud_Get_Tx1_Chrg_Stat() != EBUD_CHRG_NONE && App_Ebud_Get_Tx2_Chrg_Stat() != EBUD_CHRG_NONE)
            {
                sirkPara.sirkLeftResponse = 0;
                sirkPara.sirkRightResponse = 0;
                sirkPara.sirkMiddleResponse = 0;
                
                App_Com_Tx_Cmd_Get_Sirk(DEVICE_TX1);

                App_Com_Tx_Cmd_Get_Sirk(DEVICE_TX2);
                
                App_Com_Tx_Cmd_Get_Sirk(DEVICE_RX);

                App_Com_Tx_Cmd_Get_Random_Sirk();

                sirkPara.delayCnt = 0;

                sirkPara.stat = SIRK_STAT_WAIT_GET_DATA_END;
            }
            break;
        }
        case SIRK_STAT_WAIT_GET_DATA_END:
        {
            if(sirkPara.delayCnt > 250)
            {                
                sirkPara.delayCnt = 0;
                
                if(sirkPara.sirkLeftResponse & sirkPara.sirkRightResponse & sirkPara.sirkMiddleResponse & sirkPara.sirkRandomResponse)
                {
                    sirkPara.stat = SIRK_STAT_COMPARE_DATA;
                }
                else
                {
                    if(++sirkPara.reConnectCnt >= 3)
                    {
                        sirkPara.reConnectCnt = 0;

                        Drv_Com_Tx_Disable(COM1);
                        
                        Drv_Com_Tx_Disable(COM2);

                        Drv_Com_Tx_Disable(COM3);

                        sirkPara.stat = SIRK_STAT_FORCE_CHARGE;
                    }
                    else
                    {
                        sirkPara.stat = SIRK_STAT_GET_DATA;
                    }
                }
                
            }
            break;
        }
        case SIRK_STAT_COMPARE_DATA:
        {
            if(App_Hall_Get_State() == HALL_CLOSE)
            {
                if(App_Sirk_Check_Empty(sirkPara.sirkMiddleBuf, SIRK_DATA_PACK_MAX_SIZE))
                {
                    sirkPara.stat = SIRK_STAT_SET_DATA;
                }
                else
                {
                    if(App_Sirk_Compare_Data(sirkPara.sirkLeftBuf, sirkPara.sirkRightBuf, SIRK_DATA_PACK_MAX_SIZE))
                    {
                        sirkPara.stat = SIRK_STAT_SET_DATA;
                    }
                    else
                    {
                       sirkPara.stat = SIRK_STAT_EXIT;
                    }
                }
            }
            break;
        }
        case SIRK_STAT_SET_DATA:
        {
            if(App_Hall_Get_State() == HALL_CLOSE)
            {
                sirkPara.sirkLeftResponse = 0;

                sirkPara.sirkRightResponse = 0;

                sirkPara.sirkMiddleResponse = 0;
                
                App_Com_Tx_Cmd_Set_Sirk(DEVICE_TX1, sirkPara.sirkRandomBuf, SIRK_DATA_PACK_MAX_SIZE);
                    
                App_Com_Tx_Cmd_Set_Sirk(DEVICE_TX2, sirkPara.sirkRandomBuf, SIRK_DATA_PACK_MAX_SIZE);
                    
                App_Com_Tx_Cmd_Set_Sirk(DEVICE_RX, sirkPara.sirkRandomBuf, SIRK_DATA_PACK_MAX_SIZE);

                sirkPara.delayCnt = 0;
                
                sirkPara.stat = SIRK_STAT_WAIT_SET_DATA_END;
            }
            else
            {
                upgPara.verFlag = 0;
                
                sirkPara.stat = SIRK_STAT_GET_DATA;
            }
            break;
        }
        case SIRK_STAT_WAIT_SET_DATA_END:
        {
            if(App_Hall_Get_State() == HALL_CLOSE)
            {
                if(sirkPara.delayCnt > 250)
                {
                    if(sirkPara.sirkLeftResponse & sirkPara.sirkRightResponse & sirkPara.sirkMiddleResponse )
                    {
                        sirkPara.stat = SIRK_STAT_EXIT;
                    }
                    else
                    {
                        sirkPara.delayCnt = 0;
                        
                        sirkPara.stat = SIRK_STAT_SET_DATA;
                    }
                }
            }
            else
            {
                upgPara.verFlag = 0;
                
                sirkPara.stat = SIRK_STAT_GET_DATA;
            }
            break;
        }
        case SIRK_STAT_FORCE_CHARGE:
        {
            if(App_Hall_Get_State() == HALL_CLOSE)
            {
                sirkPara.stat = SIRK_STAT_EXIT;
            }
            break;
        }
        case SIRK_STAT_EXIT:
        {
            if(App_Hall_Get_State() == HALL_OPEN)
            {
                upgPara.verFlag = 0;
                
                sirkPara.stat = SIRK_STAT_GET_DATA;
            }
            break;
        }
        
        default: break;
    }
}


void App_Upg_Set_Fw_Ver(uint8_t verBld, uint8_t verApp, uint8_t verHw )
{
    upgPara.verBld = verBld;
    upgPara.verApp = verApp;
    upgPara.verHw  = verHw;

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

void App_Sirk_Save_Data(uint8_t devType, uint8_t *buf, uint8_t length )
{
    uint8_t i;
    uint8_t *dataPtr = NULL;

    if(devType == DEVICE_TX1)
    {
        dataPtr = sirkPara.sirkLeftBuf;

        sirkPara.sirkLeftResponse = 1;
    }
    else if(devType == DEVICE_TX2)
    {
        dataPtr = sirkPara.sirkRightBuf;

        sirkPara.sirkRightResponse = 1;
    }
    else if(devType == DEVICE_RX)
    {
        dataPtr = sirkPara.sirkMiddleBuf;

        sirkPara.sirkMiddleResponse = 1;
    }

    for(i=0;i<length;i++)
    {
        *(dataPtr+i) = buf[i];
    }
}

void App_Sirk_Set_Response(uint8_t devType )
{
    if(devType == DEVICE_TX1)
    {
        sirkPara.sirkLeftResponse = 1;
    }
    else if(devType == DEVICE_TX2)
    {
        sirkPara.sirkRightResponse = 1;
    }
    else if(devType == DEVICE_RX)
    {
        sirkPara.sirkMiddleResponse = 1;
    }
}

void App_Sirk_Save_Random_Data(uint8_t *buf, uint8_t length )
{
    uint8_t i;

    for(i=0;i<length;i++)
    {
        sirkPara.sirkRandomBuf[i] = buf[i];
    }

    sirkPara.sirkRandomResponse = 1;
}

uint8_t App_Sirk_Check_Empty(uint8_t *buf, uint8_t length )
{
    uint8_t i;
    uint8_t sum = 0;
    uint8_t retVal = 0;

    for(i=0;i<length;i++)
    {
        sum += buf[i];
    }

    if(sum == 0)
    {
        retVal = 1;
    }
    else
    {
        retVal = 0;
    }
		
		return retVal;
}

uint8_t App_Sirk_Compare_Data(uint8_t *buf1, uint8_t *buf2, uint8_t length )
{
    uint8_t retVal = 0;
    uint8_t i;

    for(i=0;i<length;i++)
    {
        if(buf1[i] != buf2[i])
        {
            retVal = 1;

            break;
        }
    }

    return retVal;
}

