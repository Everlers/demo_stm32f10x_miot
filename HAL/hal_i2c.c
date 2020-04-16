#include "hal_i2c.h"
#include "FreeRTOS.h"
#include "task.h"

void HalI2cInit(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(IIC_PORT_CLOCK, ENABLE);//ʹ��GPIOBʱ��
	
  GPIO_InitStructure.GPIO_Pin = IIC_SCL | IIC_SDA;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(IIC_PORT, &GPIO_InitStructure);//��ʼ��
	IIC_SCL_SET;
	IIC_SDA_SET;
}
void HalI2cStart(void)
{
	IIC_SDA_OUT;
	IIC_SDA_SET;	  	  
	IIC_SCL_SET;
	vTaskDelay(4);
 	IIC_SDA_CLR;
	vTaskDelay(4);
	IIC_SCL_CLR;
}
void HalI2cStop(void)
{
	IIC_SDA_OUT;
	IIC_SDA_CLR;
	IIC_SCL_SET;
 	vTaskDelay(4); 
	IIC_SDA_SET;
	vTaskDelay(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 HalI2cWaitAck(void)
{
	u8 ucErrTime=0;
	IIC_SDA_IN;      //SDA����Ϊ����  
	IIC_SDA_SET;vTaskDelay(1);	   
	IIC_SCL_SET;vTaskDelay(1);	 
	while(IIC_SDA_DAT)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			HalI2cStop();
			return 1;
		}
	}
	IIC_SCL_CLR;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
void HalI2cAck(void)
{
	IIC_SCL_CLR;
	IIC_SDA_OUT;
	IIC_SDA_CLR;
	vTaskDelay(2);
	IIC_SCL_SET;
	vTaskDelay(2);
	IIC_SCL_CLR;
	vTaskDelay(2);
}
//������ACKӦ��		    
void HalI2cNAck(void)
{
	IIC_SCL_CLR;
	IIC_SDA_OUT;
	IIC_SDA_SET;
	vTaskDelay(2);
	IIC_SCL_SET;
	vTaskDelay(2);
	IIC_SCL_CLR;
}
void HalI2cWriteByte(u8 txd)
{
	u8 t;   
	IIC_SDA_OUT; 	    
	IIC_SCL_CLR;
	for(t=0;t<8;t++)
	{        
		if(txd & 0x80)
			IIC_SDA_SET;
		else
			IIC_SDA_CLR;
		txd<<=1; 	  
		vTaskDelay(2);
		IIC_SCL_SET;
		vTaskDelay(2); 
		IIC_SCL_CLR;	
		vTaskDelay(2);
	}
}
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 HalI2cReadByte(unsigned char ack)
{
	unsigned char i,receive=0;
	IIC_SDA_IN;
  for(i=0;i<8;i++ )
	{
		IIC_SCL_CLR; 
		vTaskDelay(2);
		IIC_SCL_SET;
		receive<<=1;
		if(IIC_SDA_DAT)receive++;   
		vTaskDelay(1); 
	}					 
	if (!ack)
			HalI2cNAck();
	else
			HalI2cAck();  
	return receive;
}
