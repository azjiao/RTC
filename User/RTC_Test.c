/*********************************************************
  ���ܣ� RTCʵʱʱ������
  ������ ��Ӣ��RTCʵʱʱ������,ͨ������1���õ�ǰʱ�䣬����ȡ��ǰRTCʱ��.
  ��ƣ� azjiao
  �汾�� 0.1
  ���ڣ� 2018��08��06��
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
    delay_Init(); //���ò�ѯ��ʽ��ʱ��Ӧ���ļ�delay.c��ע�͵�#define APP1MSPLUS�궨�塣
    RTC_Init();  //RTC��ʼ��.
    Usart1_Init(19200);
    //ModbusͨѶ��ʼ��.
    Modbus_Init();
    Iwdg_Init(4, 625);  //�������Ź���ʼ����Ԥ��Ƶϵ��4��Ӧ64��RLRֵΪ625���������Ź���ʱ1s��

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





