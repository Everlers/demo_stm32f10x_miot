#include "led.h"

led_t led;

void ledInit(void)
{
	TIM_TimeBaseInitTypeDef TIMInitstruct;
	TIM_OCInitTypeDef TIMOCInitStruct;
	GPIO_InitTypeDef GPIOInitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);
	
	GPIOInitStruct.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	GPIOInitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIOInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIOInitStruct);
	GPIOB->ODR |= GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	
	TIMInitstruct.TIM_Prescaler = 1;//分频
	TIMInitstruct.TIM_CounterMode = TIM_CounterMode_Up;//向上计数
	TIMInitstruct.TIM_Period = LED_WHITE_BRIGHTNESS_MAX;//重载值
	TIMInitstruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM4,&TIMInitstruct);//初始化定时器
	
	TIMOCInitStruct.TIM_OCMode = TIM_OCMode_PWM2;
	TIMOCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIMOCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIMOCInitStruct.TIM_Pulse = 0;
	TIM_OC2Init(TIM4,&TIMOCInitStruct);
	TIM_OC2PreloadConfig(TIM4,TIM_OCPreload_Enable);
	
	TIM_OC3Init(TIM4,&TIMOCInitStruct);
	TIM_OC3PreloadConfig(TIM4,TIM_OCPreload_Enable);
	
	TIM_OC4Init(TIM4,&TIMOCInitStruct);
	TIM_OC4PreloadConfig(TIM4,TIM_OCPreload_Enable);
	
	TIM_CtrlPWMOutputs(TIM4,ENABLE);
	TIM_Cmd(TIM4,ENABLE);//使能定时器
	
	led.sw = RESET;
	led.Brightnes = 1;
}

void ledWarning(void)
{
	TIM_SetCompare2(TIM4,0);
	TIM_SetCompare3(TIM4,LED_WHITE_BRIGHTNESS_MAX);
	TIM_SetCompare4(TIM4,0);
}

void ledBrightnesSet(u8 brightness)
{
	brightness = MAX_VALUE(brightness,1);
	brightness = MIN_VALUE(brightness,LED_WHITE_BRIGHTNESS_MAX);
	led.Brightnes = brightness;
	if(led.sw == SET)
	{
		TIM_SetCompare2(TIM4,brightness);
		TIM_SetCompare3(TIM4,brightness);
		TIM_SetCompare4(TIM4,brightness);
	}
}

u8 ledBrightnesGet (void)
{
	return led.Brightnes;
}

void ledSwitchSet(FlagStatus sw)
{
	led.sw = sw;
	if(led.sw == SET)
	{
		TIM_SetCompare2(TIM4,led.Brightnes);
		TIM_SetCompare3(TIM4,led.Brightnes);
		TIM_SetCompare4(TIM4,led.Brightnes);
	}
	else
	{
		TIM_SetCompare2(TIM4,0);
		TIM_SetCompare3(TIM4,0);
		TIM_SetCompare4(TIM4,0);
	}
}

FlagStatus ledSwitchGet(void)
{
	return led.sw;
}
