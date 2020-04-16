#ifndef _MY_FAT_H_
#define _MY_FAT_H_
#include "stm32f10x.h"
#include "ff.h"
#include "diskio.h"

#define SD_CD_PORT			GPIOA
#define SD_CD_BIT				GPIO_Pin_15
#define SD_CD						(SD_CD_PORT->IDR & SD_CD_BIT)

#define PROFILE_EXPLAIN					"-----------profile-----------\r\n"

typedef struct{
char profile_explain[sizeof(PROFILE_EXPLAIN)-1];//�����ļ�˵��
char gps_auto[sizeof("GPS=x\r\n")-1];//gps��������
char gprs_auto[sizeof("GPRS=x\r\n")-1];//GPRS��������
}profile_t;

typedef enum{
SD_NULL,//SD����Ч
SD_VALID,//SD����Ч
}sd_sta;

typedef void (*SDCardExintCB)(sd_sta sta);

profile_t *get_default_profile (void);
FRESULT ReadProfileParam(profile_t *profile_s);
FRESULT Mf_Mount(void);
FRESULT Mf_ReadDir(DIR *dir,FILINFO *fileinfo);
u16 LoadAllMusic(char *path,char **chr,char *suffix,u16 maxfnl,u16 maxfn);
void SDCardInsertExintRegister(SDCardExintCB CB);
#endif
