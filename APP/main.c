#include "stm32f10x.h"
#include "stdio.h"
#include "hal_key.h"
#include "hal_rtc.h"
#include "hal_time.h"
#include "hal_usart.h"
#include "encryption.h"
#include "FreeRTOS.h"
#include "task.h"
#include "hal_sys.h"
#include "task_config.h"
#include "timers.h"
#include "miio.h"
#include "led.h"
#include "lcd_tft.h"
#include "ili9341.h"
#include "fun.h"
#include "rtosEx.h"

static void funOSTask(void *pvparamters);
static void lcdTask(void *pvParameters);
static void miioOSTask(void *pvParameters);

//����:2020-02-21
//NVIC���ȼ����� @NVIC_PriorityGroup_4
//����IDLE�ж�ȡ��DMA�رյ�ָ��
//miioSendPsec������ʹ��printf �Լ������ڴ洦��ת���ַ�
//�޸Ĵ����ж����ȼ�Ϊ@configMAX_SYSCALL_INTERRUPT_PRIORITY

int main (void)
{
	//����0~15������ռʽ���ȼ� ����Ҫ�����ȼ� 
	//ע��:(ʹ��RTOS������±�������NVIC_PriorityGroup_4��16����ռ)
	//��ΪRTOS���ж���15 ��ϵͳ�����������ȼ���configMAX_SYSCALL_INTERRUPT_PRIORITY = 11
	//RTOS�ٽ���ֻ���������ȼ���ֵ���ڻ����configMAX_SYSCALL_INTERRUPT_PRIORITYֵ���ж�
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	HalUSARTInit(HAL_USART_PORT1,HAL_BOUND_115200,NULL);

	xTaskCreate(lcdTask, "lcdTask",LCD_TASK_STACK_SIZE, NULL, LCD_TASK_PRIORITY, NULL);
	xTaskCreate(funOSTask, "funOSTask",FUN_TASK_STACK_SIZE, NULL, FUN_TASK_PRIORITY, NULL);
	xTaskCreate(miioOSTask, "miioOSTask",MIIO_TASK_STACK_SIZE, NULL, MIIO_TASK_PRIORITY, NULL);
  vTaskStartScheduler();//��ʼ����
}

static void lcdTask(void *pvParameters)
{
	lcd_init();
	ili_blk_on();
	while(1)
	{
		rtosShowStatus();
		lcd_show_string(0,10*16+8,"MIIO WiFi model Info:",WHITE,FILL_MODE);
		lcd_show_string(0,11*16+8,miioGetVersion(),WHITE,FILL_MODE);
		lcd_show_string(0,12*16+8,miioGetMac(),WHITE,FILL_MODE);
		lcd_log("%s      ",miioGetTime());
		vTaskDelay(100);
	}
}

static void funOSTask(void *pvparamters)
{
	HalKeyInit();
	xTimerStart(xTimerCreate("HalKeyHook",1/portTICK_RATE_MS,pdTRUE,NULL,HalKeyTask),0);
	ledInit();
	funInit();
	while(1)
	{
		funTask();
		vTaskDelay(100);
	}
}

static void miioOSTask(void *pvParameters)
{
	miioInit();//��ʼ��ģ��
	while(1)
	{
		miioTask();
	}
}

