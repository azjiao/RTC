/*********************************************************
  功能： RTC实时时钟试验
  描述： 精英版RTC实时时钟试验,通过串口2的RS485口设置当前时间，并获取当前RTC时间通过串口1发到串口助手.
  设计： azjiao
  版本： 0.1
  日期： 2018年08月06日
 *********************************************************/

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "template.h"
#include "dev.h"
#include "rtc.h"
#include "modRTU.h"
#include "modSlave.h"

void assert_failed(uint8_t* file, uint32_t line)
{
    //printf("Param Error! in file name: xxx, in line %d\n",line);
    //while(1);
}



int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    led_Init();
    key_Init();
    delay_Init(); 
    RTC_Init();  //RTC初始化.
    Usart1_Init(19200);
    Modbus_Init();  //串口2作为RS485初始化。
    Iwdg_Init(4, 625);  //独立看门狗初始化：预分频系数4对应64，RLR值为625，这样看门狗定时1s。
    ReceiveFrame(&RX_Struct);  
    dtStruct dt;
    /*
    while(1)
    {
        delay_ms(500);
        {
            LED1_OFF;
            LED0_ON;
        }
        Iwdg_Feed();
        delay_ms(500);
        {
            LED0_OFF;
            LED1_ON;
        }
        Iwdg_Feed();
    }
    */
    
    while(1)
    {
        Modbus_Slave();
        if(HR_4xxxx.u16Data[0] > 0)
            LED0_ON;
        else
            LED0_OFF;
        
        //组合日期时间为dtStruct类型。
        dt.u16Year = HR_4xxxx.u16Data[0];
        dt.u16Mon = HR_4xxxx.u16Data[1];
        dt.u16Day = HR_4xxxx.u16Data[2];
        dt.u16Hour = HR_4xxxx.u16Data[3];
        dt.u16Min = HR_4xxxx.u16Data[4];
        dt.u16Sec = HR_4xxxx.u16Data[5];
        if(DQ_0xxxx.u8Data[0] == 0x01)
            Set_RTC(&dt);
        
        Iwdg_Feed();
    }

}





