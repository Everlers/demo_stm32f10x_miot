#ifndef _STMFLASH_H_
#define _STMFLASH_H_
#include "stm32f10x.h"

#define	DESTRUCT_SAVE_ADDR			0x0800F800//�Իٴ����¼��ַ 128Byte
#define ENCRYPTION_SAVE_ADDR		0x0800F880//����ID����λ�� 32Byte
#define FLASH_START_ADDR				0x0800F8A0//�û��Զ������� 1888Byte

#define HAL_FLASH_FREE_RTOS			1//ʹ��FreeRTOS����ϵͳ���ڴ����

#if defined (STM32F10X_HD) || defined (STM32F10X_HD_VL) || defined (STM32F10X_CL) || defined (STM32F10X_XL)
  #define FLASH_PAGE_SIZE    ((uint16_t)0x800)
#else
  #define FLASH_PAGE_SIZE    ((uint16_t)0x400)
#endif

typedef enum{
	FlashDone = 0,	//��ɲ���
	FlashWriteError,//д�����
	FlashEraseError,//��������
}HalFlashErrorCode;


void HalFlashRead(u32 FlashAddr,u8 *ReadData,u32 ReadLen);
HalFlashErrorCode HalFlashWrite(u32 FlashAddr,u8 *WriteData,u16 WriteLen);

#endif
