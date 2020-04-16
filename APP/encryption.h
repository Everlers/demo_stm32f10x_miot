#ifndef _ENCRYPTION_H_
#define _ENCRYPTION_H_
#include "stm32f10x.h"

//ʹ�����ⷽ����CPUID���м��ܼ���д�뵽ָ��Flash��ַ(���ɱ���������,�������������߸�����ʹ�ù̶���ַ)
//ע�⣡������ܺ��IDд���ַ�ɱ��������� ��ÿ����¼������Ҫ��Ȩһ��

#define ID_WRITE_ADDR										ENCRYPTION_SAVE_ADDR//���ܺ��IDд�뵽ָ��λ�� �����ַΪ0���ձ���������ĵ�ַд��ID (ID����12���ֽ�)
#define EMPOWER_CODE_WRITE							1//�Ƿ�д����Ȩ����(�����д������Ҫ�����CPUID���м��ܲ�д�뵽ָ����Flash��ַ)
#define CPUID_LENGTH										12//CPUID ����
#define TERM_COUNTDOWN									10//������δ��Ȩ�������ʹ�ô���(�����ϵ���߸�λ����)

void encryptionEmpower(void);//����оƬ������Ȩ(��CPUID���ܱ�����Flash)
void encryptionCancelEmpower(void);//ȡ��������Ȩ(������Flash�����CPUID)
void encryptionEmpowerCheck(void);//��Ȩ��֤(ÿ�����г���ǰ���ü���,�����֤�����򵽴�һ��������ݻ�Flash�׵�ַ����)

#endif
