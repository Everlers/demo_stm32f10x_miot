#include "My_FAT.h"
#include "sdio_sdcard.h"
#include "hal_usart.h"
#include "stdlib.h"
#include "delay.h"

static void SD_Card_Printf_Info(void);

static SDCardExintCB SDCardCB;//�ڴ濨����λص�
FATFS SD_FATFS;//FAT�ļ�ϵͳ
profile_t default_profile={
PROFILE_EXPLAIN,
"GPS=0\r\n",
"GPRS=1\r\n",
};//���û�������ļ� Ĭ�ϵ�����

profile_t *get_default_profile (void)
{
	return &default_profile;
}

//��ȡ�����ļ�����
FRESULT ReadProfileParam(profile_t *profile_s)
{
	FIL profile;
	UINT len;
	
	if(profile_s == NULL)
		return FR_INVALID_PARAMETER;
	if(f_open(&profile,"profile.txt",FA_OPEN_EXISTING|FA_READ) != FR_OK)//�������ļ� ������ɹ�
	{
		if(f_open(&profile,"profile.txt",FA_OPEN_ALWAYS|FA_WRITE) != FR_OK)//�������ļ����û���򴴽����µ��ļ�
			return FR_NO_FILE;//�����ļ�ʧ��û�д��ļ�
		if(f_write(&profile,&default_profile,sizeof(profile_t),&len) != FR_OK)//д��Ĭ�ϵ������ļ�
			return FR_NO_FILE;
		f_close(&profile);//�رղ�����
		if(f_open(&profile,"profile.txt",FA_OPEN_EXISTING|FA_READ) != FR_OK)//���´�
			return FR_NO_FILE;
	}
	if(f_read(&profile,profile_s,sizeof(profile_t),&len) != FR_OK)//��ȡ����
		return FR_NO_FILE;
	f_close(&profile);
	return FR_OK;
}

FRESULT Mf_Mount(void)
{
	FRESULT res;
	if(disk_initialize(0)){//��ʼ��SD��ͨѶ
		//MyPrintf("SD init error\r\n");
		return res = FR_INVALID_DRIVE;
	}
	else
		SD_Card_Printf_Info();//��ӡSD����Ϣ
	if((res = f_mount(&SD_FATFS,"0:",0)) != FR_OK)//����SD��
		return res;
		//MyPrintf("SD mount error\r\n");
	//else
		//MyPrintf("SD mount OK\r\n");
	return res;
}

//������������
//���� 	path ·��  
//			chr �ļ��������λ�� 
//			suffixֻ����˺�׺�ļ� 
//			maxfnl ����ļ�������
//			maxfn��󱣴�����
//����ֵ ��ȡ�����ļ����� 
u16 LoadAllMusic(char *path,char **chr,char *suffix,u16 maxfnl,u16 maxfn)
{
	DIR dir;
	FILINFO fileinfo;
	FRESULT res;
	u8 i,num = 0;
	fileinfo.lfname = malloc(_MAX_LFN);
	res = f_opendir(&dir,path);
	if(res != FR_OK)
			return 0;
	for(i=0;i<maxfn;i++)
	{
		res = Mf_ReadDir(&dir,&fileinfo);
		if(res != FR_OK)
			break;
		//if(fileinfo.lfname[0] == 0)
		//	break;
		if(strstr(fileinfo.lfname,suffix) != NULL)
		{
			if(chr[num] == RESET)
			{
				chr[num] = malloc(strlen(fileinfo.lfname)+1);
			}
			if(chr[num] == 0)return i;//�ڴ������� ֱ�ӷ����Ѿ���ȡ�����ļ�
			if(strlen(fileinfo.lfname) > maxfnl)return i;//�ļ������� ֱ�ӷ����Ѿ���ȡ�����ļ�
			memcpy(chr[num],fileinfo.lfname,strlen(fileinfo.lfname));
			chr[num][strlen(fileinfo.lfname)] = '\0';
			num ++;
		}
	}
	return num;
}
//�ڴ濨�����ⲿ�ж�
void SDCardInsertExintRegister(SDCardExintCB CB)
{
	GPIO_InitTypeDef GPIO_InitStrict;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_InitStrict.GPIO_Pin = SD_CD_BIT;
	GPIO_InitStrict.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStrict.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SD_CD_PORT,&GPIO_InitStrict);//��ʼ��SD����CD����
	
	EXTI_ClearITPendingBit(EXTI_Line15);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource15);
	EXTI_InitStructure.EXTI_Line = EXTI_Line15;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; 
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//�������ж�
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;                     
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
	
	SDCardCB = CB;
}

//SD��CD���ŷ����ı� �ڴ濨���γ������
void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line15) != RESET)  
	{
		delay_ms(20);
		if(SDCardCB)
		{
			if(SD_CD)
				SDCardCB(SD_NULL);//�ڴ濨���γ�
			else
				SDCardCB(SD_VALID);//�ڴ濨����
		}
		EXTI_ClearITPendingBit(EXTI_Line15);
	}
}

//���ȡ�ļ���
//����ֵ:ִ�н��
FRESULT Mf_ReadDir(DIR *dir,FILINFO *fileinfo)
{
	FRESULT res;
 	fileinfo->lfsize = _MAX_LFN;
	//fileinfo->lfname = Malloc(fileinfo->lfsize);	//�����ڴ�  
	if(fileinfo->lfname == NULL)
		return FR_NOT_ENOUGH_CORE;
	res = f_readdir(dir,fileinfo);//��ȡһ���ļ�����Ϣ
	fileinfo->lfname=(fileinfo->lfname[0]!=0)? fileinfo->lfname:fileinfo->fname;//ѡ�����ֻ��߶̵�����
	return res;
}

/**********************************************************
* �������� ---> ��ӡSD����Ϣ������
* ��ڲ��� ---> none
* ������ֵ ---> none
* ����˵�� ---> 1����ӡ��������������
*               2����ӡ�������͵�����
*               3����ӡ����������Ϣ
**********************************************************/
static void SD_Card_Printf_Info(void)
{
	switch(SDCardInfo.CardType)	//������
	{
		case SDIO_HIGH_CAPACITY_SD_CARD:	//��������
			//MyPrintf("Card Type: SDHC V2.0\r\n");
			break;
			
		case SDIO_STD_CAPACITY_SD_CARD_V1_1:	//��׼����V1.1
			//MyPrintf("Card Type: SDSC V1.1\r\n");
			break;
			
		case SDIO_STD_CAPACITY_SD_CARD_V2_0:	//��׼����V2.0
			//MyPrintf("Card Type: SDSC V2.0\r\n");
			break;

		case SDIO_MULTIMEDIA_CARD:	//MMC��
			//MyPrintf("Card Type: MMC Card\r\n");
			break;
	}

	//MyPrintf("Card ManufacturerID: %d\r\n",SDCardInfo.SD_cid.ManufacturerID);			//������ID
 	//MyPrintf("Card RCA: %d\r\n",SDCardInfo.RCA);										//����Ե�ַ
	//MyPrintf("Card Capacity: %d MB\r\n",(uint32_t)SDCardInfo.CardCapacity);	//��ʾ����
 	//MyPrintf("Card BlockSize: %d\r\n",SDCardInfo.CardBlockSize);					//��ʾ���С
}
