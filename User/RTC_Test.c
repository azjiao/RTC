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
    dtStruct dt;
    uint16_t u16Sec = 0;
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
    ReceiveFrame(&RX_Struct); 
    while(1)
    {
        //通过串口2来同步时间。
        //当DQ0.0为1时同步时钟。
        //DQ0.0应该在全部时间信息传送完毕后复位。
        //尽量采用一次传送全部时钟信息，而不是分多次传送，以减少同步误差。
        Modbus_Slave();
        
        //组合日期时间为dtStruct类型。
        dt.u16Year = HR_4xxxx.u16Data[5];
        dt.u16Mon = HR_4xxxx.u16Data[4];
        dt.u16Day = HR_4xxxx.u16Data[3];
        dt.u16Hour = HR_4xxxx.u16Data[2];
        dt.u16Min = HR_4xxxx.u16Data[1];
        dt.u16Sec = HR_4xxxx.u16Data[0];
        if(DQ_0xxxx.u8Data[0] == 0x01)
            Set_RTC(&dt);
        
        //打印当前时间。
        if(u16Sec != Calendar.u16Sec)
        {
            printf("当前日期时间 %d-%d-%d %d:%d:%d\n", Calendar.u16Year, Calendar.u16Mon, Calendar.u16Day, Calendar.u16Hour, Calendar.u16Min, Calendar.u16Sec);
            u16Sec = Calendar.u16Sec;
            LED0 = !LED0;
        }
        
        Iwdg_Feed();
    }

}





