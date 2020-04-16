#ifndef _BMS_H_
#define _BMS_H_
#include "stm32f10x.h"
#define BAT_PORT									GPIOB
#define BAT_CHARGE_BIT						GPIO_Pin_4
#define BAT_CHARGE								(BAT_PORT->IDR & BAT_CHARGE_BIT)

#define HIGH_PULL_RES							100.0//������ֵ(��λ��ǧŷ)
#define LOW_PULL_RES							47.0 //������ֵ(��λ��ǧŷ)
#define MAX_VOLTAGE								8.1	 //��ߵ�ѹ(��λ����)
#define LOW_VOLTAGE								7.0	 //��͵�ѹ(��λ����)
typedef struct{
float voltage;//��ѹ
u8 volume;//����
}BMS_INFO_t;


void BMS_Init(void);
BMS_INFO_t *BMS_InfoGet(void);
void BMS_Task(void);
#endif
