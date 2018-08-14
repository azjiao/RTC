/*********************************************************
  ���ܣ� RTCʵʱʱ������
  ������ ��Ӣ��RTCʵʱʱ������,ͨ������2��RS485�����õ�ǰʱ�䣬����ȡ��ǰRTCʱ��ͨ������1������������.
  ��ƣ� azjiao
  �汾�� 0.1
  ���ڣ� 2018��08��06��
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
    RTC_Init();  //RTC��ʼ��.
    Usart1_Init(19200);
    Modbus_Init();  //����2��ΪRS485��ʼ����
    Iwdg_Init(4, 625);  //�������Ź���ʼ����Ԥ��Ƶϵ��4��Ӧ64��RLRֵΪ625���������Ź���ʱ1s��     
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
        //ͨ������2��ͬ��ʱ�䡣
        //��DQ0.0Ϊ1ʱͬ��ʱ�ӡ�
        //DQ0.0Ӧ����ȫ��ʱ����Ϣ������Ϻ�λ��
        //��������һ�δ���ȫ��ʱ����Ϣ�������Ƿֶ�δ��ͣ��Լ���ͬ����
        Modbus_Slave();
        
        //�������ʱ��ΪdtStruct���͡�
        dt.u16Year = HR_4xxxx.u16Data[5];
        dt.u16Mon = HR_4xxxx.u16Data[4];
        dt.u16Day = HR_4xxxx.u16Data[3];
        dt.u16Hour = HR_4xxxx.u16Data[2];
        dt.u16Min = HR_4xxxx.u16Data[1];
        dt.u16Sec = HR_4xxxx.u16Data[0];
        if(DQ_0xxxx.u8Data[0] == 0x01)
            Set_RTC(&dt);
        
        //��ӡ��ǰʱ�䡣
        if(u16Sec != Calendar.u16Sec)
        {
            printf("��ǰ����ʱ�� %d-%d-%d %d:%d:%d\n", Calendar.u16Year, Calendar.u16Mon, Calendar.u16Day, Calendar.u16Hour, Calendar.u16Min, Calendar.u16Sec);
            u16Sec = Calendar.u16Sec;
            LED0 = !LED0;
        }
        
        Iwdg_Feed();
    }

}





