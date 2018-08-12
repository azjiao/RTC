/*********************************************************
  功能： RTC实时时钟试验
  描述： 精英版RTC实时时钟试验,通过串口1设置当前时间，并获取当前RTC时间.
  设计： azjiao
  版本： 0.1
  日期： 2018年08月06日
 *********************************************************/

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "template.h"
#include "dev.h"
#include "modRTU.h"
#include "modMaster.h"
#include "modSlave.h"
#include "rtc.h"

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
    delay_Init(); //采用查询方式延时，应在文件delay.c中注释掉#define APP1MSPLUS宏定义。
    RTC_Init();  //RTC初始化.
    Usart1_Init(19200);
    //Modbus通讯初始化.
    Modbus_Init();
    Iwdg_Init(4, 625);  //独立看门狗初始化：预分频系数4对应64，RLR值为625，这样看门狗定时1s。

    ReceiveFrame(&RX_Struct);

    while(1)
    {
        Modbus_Slave();
        if(Modbus_Status_Struct.bBusy){
            LED1_OFF;
            LED0_ON;
        }
        else{
            LED0_OFF;
            LED1_ON;
        }
       Iwdg_Feed();
    }

}





