/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.4.0
  * @date    10/15/2010
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}


#include "led.h"
//ջ���ݶ��� CPU�Ĵ�����ջ�е�����
typedef struct
{
	u32 R0;
	u32 R1;
	u32 R2;
	u32 R3;
	u32 R12;
	u32 LR;
	u32 PC;
	u32 xPSR;
}STACK_DATA_TYPE;

//��ȡ��ǰCPU��ջָ��MSP
__asm u32 getMSP(void) 
{
  mrs r0, msp
	bx lr
}

//��ȡ��ǰCPU��ջָ��PSP
__asm u32 getPSP(void) 
{
  mrs r0, psp
	bx lr
}

//û�в���ϵͳʱ���쳣����
void NotOSHardFault_Handler(u32 msp_addr)
{
	STACK_DATA_TYPE *p;									//��ջ�д洢������
	msp_addr -= 4;											//��ջָ���ȥ4����ΪĬ�϶�ջָ��ָ�������һ���յĵط�-���Ա����ȥ4
	if((msp_addr >> 20) != 0x200)		//�жϵ�ַ��Χ��������0x200xxxxx ��Χ �����ջָ�뱻�ƻ����޷���¼�ֳ���
		return;
	msp_addr += 8;											//�����жϺ󣬶�ջ�ֽ�����2��u32���ݣ������Ҫ������
	p = (STACK_DATA_TYPE *)msp_addr;
	p->PC -= 3;													//PCָ��Ҫ��ȥ3
	if(p == (STACK_DATA_TYPE *)0)
		p = (STACK_DATA_TYPE *)0x20000000;
	while(1);
}

//�в���ϵͳʱ���쳣����
void OSHardFault_Handler(u32 psp_addr)
{
	STACK_DATA_TYPE *p;										//��ջ�д洢������
	psp_addr -= 4;												//��ջָ���ȥ4����ΪĬ�϶�ջָ��ָ�������һ���յĵط�-���Ա����ȥ4
	if((psp_addr >> 20) != 0x200)		//�жϵ�ַ��Χ��������0x200xxxxx ��Χ �����ջָ�뱻�ƻ����޷���¼�ֳ���
		return;
	p = (STACK_DATA_TYPE *)psp_addr;
	p->PC -= 3;												//PCָ��Ҫ��ȥ3
	if(p == (STACK_DATA_TYPE *)0)
		p = (STACK_DATA_TYPE *)0x20000000;
	while(p);
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
	u32 msp_addr = getMSP();		//��ȡ�߳�ģʽ�¶�ջָ��λ��
	u32 psp_addr = getPSP();		//��ȡ�ж��µĶ�ջָ��λ��-����OS������

	ledWarning();//LED����
	OSHardFault_Handler(psp_addr);//ʹ�ܲ���ϵͳ
	//NotOSHardFault_Handler(msp_addr);	//û��ʹ�ܲ���ϵͳ
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
/*
void SVC_Handler(void)
{
}
*/

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
/*
void PendSV_Handler(void)
{
}
*/

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
/*
void SysTick_Handler(void)
{
}
*/

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
