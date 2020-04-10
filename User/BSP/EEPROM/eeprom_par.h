/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __EEPROM_PAR_H
#define __EEPROM_PAR_H

/* Includes ------------------------------------------------------------------*/
#include "./Parameter/parameter.h"


#ifdef __cplusplus
extern "C"
{
#endif

#define PAR_CRC_DATA                (MAX_PARAMETER_NUM + 0)     //CRC����У��
#define PAR_SAVE_FLAG               (MAX_PARAMETER_NUM + 1)		//�����־
#define MAX_PARAMATER_NUM_FINAL		(MAX_PARAMETER_NUM + 2)

#define EEPROM_NUM_VALID_DATA		(MAX_PARAMETER_NUM + 150)	//???
#define EEPROM_POS_CRC_DATA      	(EEPROM_NUM_VALID_DATA + 0)    	//CRC����У��
#define EEPROM_POS_SAVE_FLAG       	(EEPROM_NUM_VALID_DATA + 1)		//�����־
#define EEPROM_NUM_FINAL			(EEPROM_NUM_VALID_DATA + 2)
#define EEPROM_SAVE_FLAG            0xAA55
	

	
extern unsigned char comm_send_buf[];
extern unsigned char comm_recv_buf[];
	
void init_parameter(void);		//���ز���
void save_parameter(void);		//�������

void set_parameter(void);
unsigned int get_parameter(void);
void Get_Default_Value(void);
void Set_Default_Value(void);


#ifdef __cplusplus
}
#endif

#endif
