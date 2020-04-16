#ifndef _USART_H_
#define _USART_H_
#include "stm32f10x.h"

#define DEUSART1										1
#define DEUSART2										0
#define DEUSART3										0

#define HAL_BOUND_115200						115200
#define HAL_BOUND_9600							9600

#define USART_TX_LEN								256  //����������ֽ���
#define USART1_REC_LEN  						256  //���崮��1�������ֽ��� 
#define USART_REC_LEN  							256  //�����������ֽ��� 

#define debug if(1)JlinkUSART

typedef enum {
	HAL_USART_PORT1,
	HAL_USART_PORT2,
	HAL_USART_PORT3,
}USART_Port;//����ʹ�õĶ˿�
typedef void (*USARTReceiveIDLE_CB) ( u8 *Buf, u16 Len );

void HalUSARTInit(USART_Port Portx, u32 bound, USARTReceiveIDLE_CB Callback);
void HalUSARTWriteData(USART_Port USARTx,u8 *Buff,u32 DataLen);//USART Send (DMA)
void HalUSARTString(USART_Port USARTx,void *character);//Usart character string Send (DMA)
void HalUSARTReceiveCmd(USART_Port USARTx,FunctionalState NewState);//Enable or Disable USART DMA Receive

u8 AccumulationCheck(u8 *Data,u8 Length);//�ۼ�У��
u8 XOR_Check(u8 *Data,u8 Length);//���У��
void MyPrintf(char *fmt,...);
void PrintfString(char *fmt,...);//�����ӡ
void JlinkUSART(char *fmt,...);//Jlink CDC USART

#endif


