#ifndef malloc_h
#define malloc_h
#include "stm32f10x.h"

#define MALLOC
#define MEM_BLOCK_SIZE 								32//����һ���ڴ���С�ֽ�
#define MAX_MEM_RAM_SIZE 							(1 * 1024)//��̬�ڴ������ֽڴ�С
#define MAX_ALLOC_TABLE_SIZE 					(MAX_MEM_RAM_SIZE / MEM_BLOCK_SIZE)//ÿ��Ĵ�С

typedef void (*MallocErrorCb) (void);//�ڴ�������Ļص�

typedef struct {
	u8 Mem_Base[MAX_MEM_RAM_SIZE];//�ڴ��
	u8 Mem_Map[MAX_ALLOC_TABLE_SIZE];//�ڴ����״̬
	u8 Init_Flag;//��ʼ��״̬ 
}Malloc_TypeDef;

void mymem_set(u8 *map_add,u8 num,u32 len);
void mymen_copy(u8 *a1,u8 *a2,u32 len);

void malloc_Init(MallocErrorCb ErrorCB);
float mem_rate(void);
void *Malloc(u32 len);
void Free(void *ptr);

#endif
