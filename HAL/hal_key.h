#ifndef _HAL_KEY_H_
#define _HAL_KEY_H_
#include "hal_sys.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

#define HAL_KEY_SHAKE_TIME						20//20ms祛抖时间
#define HAL_KEY_LONG_PRESS_TIME				1800//1800ms长按时间

#define HAL_KEY_1										BV(1)
#define HAL_KEY_2										BV(2)
#define HAL_KEY_ALL									(HAL_KEY_POWER)

/*io pin config*/
#define HAL_KEY_PORT  GPIOA->IDR
#define HAL_KEY_SW_1	GPIO_Pin_11
#define HAL_KEY_SW_2	GPIO_Pin_12
#define HAL_KEY_SW_ALL (HAL_KEY_SW_1|HAL_KEY_SW_2)

void HalKeyInit(void);
void HalKeyTask(TimerHandle_t timer);
u8 HalKeyGetShortPress(u8 sw);
u8 HalKeyGetLongPress(u8 sw);
u8 HalKeyGetShortPressNoClear(u8 sw);
u8 HalKeyGetLongPressNoClear(u8 sw);
u8 HalKeyGetLoosen(u8 sw);

#endif
