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

//日期:2020-02-21
//NVIC优先级配置 @NVIC_PriorityGroup_4
//串口IDLE中断取消DMA关闭的指令
//miioSendPsec函数不使用printf 自己创建内存处理转行字符
//修改串口中断优先级为@configMAX_SYSCALL_INTERRUPT_PRIORITY

int main (void)
{
	//配置0~15级的抢占式优先级 不需要子优先级 
	//注意:(使用RTOS的情况下必须设置NVIC_PriorityGroup_4既16级抢占)
	//因为RTOS的中断是15 受系统管理的最大优先级是configMAX_SYSCALL_INTERRUPT_PRIORITY = 11
	//RTOS临界区只会屏蔽优先级数值大于或等于configMAX_SYSCALL_INTERRUPT_PRIORITY值的中断
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	HalUSARTInit(HAL_USART_PORT1,HAL_BOUND_115200,NULL);

	xTaskCreate(lcdTask, "lcdTask",LCD_TASK_STACK_SIZE, NULL, LCD_TASK_PRIORITY, NULL);
	xTaskCreate(funOSTask, "funOSTask",FUN_TASK_STACK_SIZE, NULL, FUN_TASK_PRIORITY, NULL);
	xTaskCreate(miioOSTask, "miioOSTask",MIIO_TASK_STACK_SIZE, NULL, MIIO_TASK_PRIORITY, NULL);
  vTaskStartScheduler();//开始调度
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
	miioInit();//初始化模组
	while(1)
	{
		miioTask();
	}
}

