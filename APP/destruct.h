#ifndef _DESTRUCT_H_
#define _DESTRUCT_H_
#include "stm32f10x.h"
#include "Hal_flash.h"

//���ӳ���ʱ�ĳ����д��һ��UUID ֮���������д�� ������뱻��¼����һ��û��UUID�İ��������ܵ�����������

#define DES_WRITE_UUID												0//������ӵ�һ����¼���� ����Ҫд��UUID  ����ǵ�һ�γ�¼��д0  
#define DES_UUID_LENGTH												16//128λ��UUID
#define DES_CODE_RUN_ADDR											0x08000000//����������е��׵�ַ (�����þ�����������׵�ַ)
#define DES_FLAG_SAVE_ADDR										DESTRUCT_SAVE_ADDR//��־�Ѿ��޸Ĺ����豸�����ٴ���
#define DES_NUM_SAVE_ADDR											DES_FLAG_SAVE_ADDR+1//�������д�����Flash��ַ
#define	DES_UUID_SAVE_ADDR										DES_NUM_SAVE_ADDR + 4//UUID�����ַ 

#define DES_WHILE_OR_DESTRUCT									1//��ѭ���������ٳ���  1:��ѭ��  0:����
#define DES_NUMBER														2000//���ô���  ������������
#define DES_REVISE_FLASG											0xAA//���  ��������������޸Ĵ˱����������ʹ�ô�����


typedef enum{
DES_UUID,
DES_NUM,
}DestructNumOrUUID_t;//�Ի�ѡ�������

void DestructSystem(void);//�Ի�ϵͳ(��������ʱ����һ�μ���һ�ο��ô���)
#endif
