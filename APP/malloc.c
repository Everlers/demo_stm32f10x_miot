#include "malloc.h"
#include "stdio.h"
#include "sys.h"

Malloc_TypeDef mem_dev;
MallocErrorCb Error;

void malloc_Init(MallocErrorCb ErrorCB){
	mymem_set(mem_dev.Mem_Map,0,MAX_ALLOC_TABLE_SIZE);//�ڴ������ʼ��
	mymem_set(mem_dev.Mem_Base,0,MAX_ALLOC_TABLE_SIZE);//�ڴ�س�ʼ��
	mem_dev.Init_Flag = 1;//��ʼ����ɱ�־
	Error = ErrorCB;
}

//�ڴ�����
void mymem_set(u8 *map_add,u8 num,u32 len)
{
	u8 *s = map_add;
	while(len--)
		*s++ = num;
}

//�ڴ渴��
void mymen_copy(u8 *a1,u8 *a2,u32 len){
	u8 *des = a1;
	u8 *src = a2;
	while(len--)
		*des++ = *src++;
}

//�ڴ�ʹ����
float mem_rate(void){
	float rate;
	u32 i;
	rate = 0;
	for(i=0;i<MAX_ALLOC_TABLE_SIZE;i++){
		if(mem_dev.Mem_Map[i])rate++;
	}
	return (rate*100.0)/(float)MAX_ALLOC_TABLE_SIZE;
}

void *Malloc(u32 len){
	u32 i,offset;
	u32 nmemb; //��Ҫ���ڴ��
	u32 cmemb=0; //�ҵ����ڴ��
	if(!mem_dev.Init_Flag){malloc_Init(NULL);}//���ó�ʼ��
	if(len == 0)return ((void *)0);//����Ҫ����
	nmemb = len/MEM_BLOCK_SIZE;
	if(len%MEM_BLOCK_SIZE)nmemb++;
	INTX_DISABLE();
	for(offset=MAX_ALLOC_TABLE_SIZE-1;offset>0;offset--){
		if(mem_dev.Mem_Map[offset] == 0) cmemb++;
		else cmemb = 0;
		if(cmemb == nmemb){//�ҵ������ڴ�
			for(i=0;i<nmemb;i++){
				mem_dev.Mem_Map[i+offset]=nmemb;
			}
			return ((void *)((offset*MEM_BLOCK_SIZE)+(u32)(mem_dev.Mem_Base)));//���ر�����ַ
		}
	}
	INTX_ENABLE();
	if(Error)
		Error();
	return (void *)0;//û���ҵ�
}

void Free(void *ptr){
	u32 Mem_addr;
	u32 i,offset;
	u32 Bsize;
	offset = (u32)ptr;
	offset -= (u32)&mem_dev.Mem_Base;
	INTX_DISABLE();
	if(offset < MAX_MEM_RAM_SIZE){
		Mem_addr = offset/MEM_BLOCK_SIZE;//���ַ
		Bsize = mem_dev.Mem_Map[Mem_addr];//������
		for(i=0;i<Bsize;i++){
			mem_dev.Mem_Map[i+Mem_addr] = 0;
		}
	}
	INTX_ENABLE();
}
