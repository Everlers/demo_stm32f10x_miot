#include "hal_sys.h"

void WFI_SET(void)
{
	__ASM volatile("wfi");		  
}
//�ر������ж�
void INTX_DISABLE(void)
{		  
	__ASM volatile("cpsid i");
}
//���������ж�
void INTX_ENABLE(void)
{
	__ASM volatile("cpsie i");		  
}
/*
__asm void SystemReset(void)
{
	MOV R0, #1 //;
	MSR FAULTMASK, R0 //; ??FAULTMASK ????????
	LDR R0, =0xE000ED0C //;
	LDR R1, =0x05FA0004 //;
	STR R1, [R0] //; ??????
	deadloop
	B deadloop //; ???????????????
}*/
//����ջ����ַ
//addr:ջ����ַ
__asm void MSR_MSP(u32 addr) 
{
    MSR MSP, r0 			//set Main Stack value
    BX r14
}
