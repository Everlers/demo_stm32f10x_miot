#include "destruct.h"
#include "hal_flash.h"
#include "delay.h"
#include "string.h"

//�Ի�ϵͳ
DestructNumOrUUID_t DestructNumOrUUID = DES_UUID;//ѡ���ⷽʽ

//�Ի����������
const u8 clear_data[]={
"The program has been destroyed,\
 Please contact Software Engineer"
};

//Ψһʶ���� ��Ԥ���������ص���İ����ϣ�
//����ڰ��ӵĴ���ռ���δ�����������ַ����Իٴ���
const u8 UUID[DES_UUID_LENGTH]={
"XieYuFeng-GuoDun"
};

void DestructNumSet(u32 num)
{
	u8 flag;
	HalFlashRead(DES_FLAG_SAVE_ADDR,&flag,1);//��ȡ���
	if(flag != DES_REVISE_FLASG)//���û���޸Ĺ����豸�Ŀ����ô���
	{
		flag = DES_REVISE_FLASG;//����Ѿ��޸�
		HalFlashWrite(DES_FLAG_SAVE_ADDR,&flag,1);//д����
		HalFlashWrite(DES_NUM_SAVE_ADDR,(u8 *)&num,4);//�޸Ŀ��ô���
	}
}

void DestructSystem(void)
{
	Check:
	if(DestructNumOrUUID == DES_NUM)
	{
		u32 num;
		DestructNumSet(DES_NUMBER);//����ʹ�ô���
		HalFlashRead(DES_NUM_SAVE_ADDR,(u8 *)&num,4);
		if(num == 0)//�����������
		{
			if(DES_WHILE_OR_DESTRUCT)//�����������ѭ��
				while(1){}
			else//��������
				HalFlashWrite(DES_CODE_RUN_ADDR,(u8 *)clear_data,sizeof(clear_data));
		}
		num-=1;
		HalFlashWrite(DES_NUM_SAVE_ADDR,(u8 *)&num,4);
	}
	else if(DestructNumOrUUID == DES_UUID)
	{
		u8 uuid[sizeof(UUID)];
		HalFlashRead(DES_UUID_SAVE_ADDR,uuid,sizeof(uuid));
		if(memcmp(uuid,UUID,sizeof(uuid)))//���UUID����
		{
			if(DES_WRITE_UUID)//����ǳ���������д��UUID
				HalFlashWrite(DES_UUID_SAVE_ADDR,(u8 *)UUID,sizeof(UUID));//д��UUID��Flash
			else{//������Ǹ��µĴ��� ��UUID�Բ��� ��ԭ��PCB
				DestructNumOrUUID = DES_NUM;//���ô�������
				goto Check;//ִ�д�������
			}
		}
	}
}
