#ifndef _LED_H_
#define _LED_H_
#include "hal_sys.h"

#define LED_WHITE_BRIGHTNESS_MAX			100//�׵��������

typedef struct{
	FlagStatus 	sw;//����
	u8 					Brightnes;//����
}led_t;

void ledInit(void);
FlagStatus ledSwitchGet(void);
void ledSwitchSet(FlagStatus sw);

void ledWarning(void);
u8 ledBrightnesGet (void);
void ledBrightnesSet(u8 brightness);
#endif
