#include "encryption.h"
#include "hal_flash.h"
#include "string.h"

#if ID_WRITE_ADDR
static u8 *EncryptionCPUID = (u8 *)ID_WRITE_ADDR;//���ܺ��CPUID
#else
static const u8 EncryptionCPUID[12] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};//���ܺ��CPUID
#endif
static const u32 termCountdown = TERM_COUNTDOWN;//���޵�����(��CPUID���������¿�ʼ����)

#if EMPOWER_CODE_WRITE
//оƬ��Ȩ
//��оƬIDͨ��������д��Flash 
void encryptionEmpower(void)
{
	u8 *CPUID = (u8 *)0x1FFFF7E8;
	u8 tmp[12];
	for(u8 i=0;i<CPUID_LENGTH;i++){//����CPUID
		tmp[i] = CPUID[i]>>1;
		tmp[i] |= CPUID[i]<<6;
	}
	if(memcmp(EncryptionCPUID,tmp,CPUID_LENGTH))//���û�д�����ܺ��ID
		HalFlashWrite((u32)EncryptionCPUID,tmp,CPUID_LENGTH);//������ܺ��CPUID��Flash
}
#endif

//ȡ��������Ȩ
void encryptionCancelEmpower(void)
{
	u8 Clear[]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
	HalFlashWrite((u32)EncryptionCPUID,Clear,CPUID_LENGTH);
}

//��Ȩ��֤
void encryptionEmpowerCheck(void)
{
	u8 *CPUID = (u8 *)0x1FFFF7E8;
	u8 tmp[12];
	for(u8 i=0;i<CPUID_LENGTH;i++){//����CPUID
		tmp[i] = CPUID[i]>>1;
		tmp[i] |= CPUID[i]<<6;
	}
	if(memcmp(tmp,EncryptionCPUID,CPUID_LENGTH)){//���ID����ȷ(δ��Ȩ)
		u32 *Countdown = (u32 *)&termCountdown;
		if(*Countdown == 0){//����������
			char DestructData[] = {"Unauthorized hardware!"};
			HalFlashWrite(0x08000000,(u8 *)DestructData,strlen(DestructData));
		}
		else{
			u32 tmp = *Countdown-1;
			HalFlashWrite((u32)&termCountdown,(u8 *)&tmp,sizeof(termCountdown));
		}
	}
}
