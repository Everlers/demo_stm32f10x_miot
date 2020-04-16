#include "hal_key.h"

static u8 key = 0;
static u8 keyLongPress;
static u16 longPressTime = 0;

static u16 pollKey = HAL_KEY_SW_ALL;
static u16 waitKey = 0;
static u8 shakeTime;

void HalKeyInit(void)
{
	GPIO_InitTypeDef GPIOInitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIOInitStruct.GPIO_Pin = HAL_KEY_SW_ALL;
	GPIOInitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIOInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIOInitStruct);
}

static void shortPressPoll(u16 port)
{
	if((waitKey & HAL_KEY_SW_1) && (port & HAL_KEY_SW_1)){
		key |= HAL_KEY_1;
	}
	if((waitKey & HAL_KEY_SW_2) && (port & HAL_KEY_SW_2)){
		key |= HAL_KEY_2;
	}
}

static void longPressPoll(void)
{
	if(waitKey & HAL_KEY_SW_1){
		keyLongPress |= HAL_KEY_1;
	}
	else
		keyLongPress &= ~HAL_KEY_1;
	
	if(waitKey & HAL_KEY_SW_2){
		keyLongPress |= HAL_KEY_2;
	}
	else
		keyLongPress &= ~HAL_KEY_2;
}

void HalKeyTask(TimerHandle_t timer)
{
	if(shakeTime == 0 && pollKey != HAL_KEY_SW_ALL)//����а��²��Ҷ���ʱ���
	{
		if(pollKey == (HAL_KEY_PORT&HAL_KEY_SW_ALL))//����״̬ȷ��
		{
			waitKey = ~pollKey;
			waitKey &= HAL_KEY_SW_ALL;
			pollKey = HAL_KEY_SW_ALL;
		}
		else{
			pollKey = HAL_KEY_SW_ALL;
		}
	}
	
	if((HAL_KEY_PORT & HAL_KEY_SW_ALL) !=  HAL_KEY_SW_ALL && shakeTime == 0 && waitKey == 0)//����а�ť����
	{
		pollKey = (HAL_KEY_PORT & HAL_KEY_SW_ALL);//�õ���ť״̬
		shakeTime = HAL_KEY_SHAKE_TIME;//�����ʱ��
	}
	
	if(waitKey)//����а�������
	{
		u16 port = HAL_KEY_PORT & HAL_KEY_SW_ALL;
		if(longPressTime < HAL_KEY_LONG_PRESS_TIME){//�����������ʱ��
			longPressTime++;
			shortPressPoll(port);//ִ�ж̰�ɨ��
		}
		else if(longPressTime == HAL_KEY_LONG_PRESS_TIME)//����ʱ�䵽
		{
			longPressTime++;
			longPressPoll();//ִ�г���ɨ��
		}
		waitKey ^= (port&waitKey);//����Ѿ��ɿ��İ�ť
	}
	else{//���а�ť�Ѿ��ɿ�
		keyLongPress = 0;
		longPressTime =  0;
	}
	
	if(shakeTime)
		shakeTime--;
}

//��ѯ�Ƿ��ɿ�
u8 HalKeyGetLoosen(u8 sw)
{
	return (HAL_KEY_PORT & sw) ? 1:0;
}

u8 HalKeyGetShortPress(u8 sw)
{
	u8 s = (sw&key) ? 1:0;
	key ^= sw&key;
	return s;
}

u8 HalKeyGetLongPress(u8 sw)
{
	return(sw&keyLongPress) ? 1:0;
}

u8 HalKeyGetShortPressNoClear(u8 sw)
{
	return sw & key;
}

u8 HalKeyGetLongPressNoClear(u8 sw)
{
	return sw & keyLongPress;
}




