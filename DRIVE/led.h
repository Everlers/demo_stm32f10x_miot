#ifndef _LED_H_
#define _LED_H_
#include "hal_sys.h"

#define LED_WHITE_BRIGHTNESS_MAX			100//白灯最大亮度

typedef struct{
	FlagStatus 	sw;//开关
	u8 					Brightnes;//亮度
}led_t;

void ledInit(void);
FlagStatus ledSwitchGet(void);
void ledSwitchSet(FlagStatus sw);

void ledWarning(void);
u8 ledBrightnesGet (void);
void ledBrightnesSet(u8 brightness);
#endif
