#include "fun.h"
#include "hal_key.h"
#include "miio.h"
#include "led.h"
#include "FreeRTOS.h"
#include "task.h"
#include "string.h"

void funInit(void)
{
	
}

void funTask(void)
{
	if(HalKeyGetShortPress(HAL_KEY_2))
	{
		miioModelRestore();//恢复出厂设置
	}
	if(HalKeyGetShortPress(HAL_KEY_1))
	{
		if(ledSwitchGet())
			ledSwitchSet(RESET);
		else
			ledSwitchSet(SET);
		property_t *property = pvPortMalloc(sizeof(property_t));
		MALLOC_CHECK(property);
		miioPropertyDeinit(property);
		property->format = PROPERTY_FORMAT_BOOLEAN;
		property->data.boolean.value = ledSwitchGet();
		miioPropertyChanged(IID_2_Light,IID_2_1_On,property);//属性改变通知
		miioEvents(IID_2_Light,IID_2_1_LowBattery,1,property);//事件通知
		vPortFree(property);
	}
}
