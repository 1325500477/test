/* Includes ------------------------------------------------------------------*/
#include "eeprom_par.h"
#include "Bsp.h"
#include "process.h"

uint16_t  eeprom_parameter[MAX_PARAMETER_NUM+2];

FLASH_Status FlashStatus;
uint16_t VarValue = 0;
uint16_t VirtAddVarTab[MAX_PARAMATER_NUM_FINAL];	// = {0x5555, 0x6666, 0x7777};


void Set_Default_Value(void)
{

}

void Get_Default_Value(void)
{

}

void init_parameter(void)
{
	unsigned char i = 0;
	unsigned short int u16_data;
    unsigned short int u16_crc;
    unsigned char bFlagDataErr = 0;

	/* Unlock the Flash Program Erase controller */
	FLASH_Unlock();
	/* EEPROM Init */
	EE_Init();
	FLASH_Lock();	

	for(i = 0; i < MAX_PARAMATER_NUM_FINAL; i++)
	{
		VirtAddVarTab[i] = 0x1000+i;
	}

	for(i = 0; i < MAX_PARAMATER_NUM_FINAL; i++)
	{
		EE_ReadVariable(VirtAddVarTab[i], &u16_data);
		//eeprom_parameter[i] = (unsigned char)(u16_data & 0xFF);
        eeprom_parameter[i] = u16_data;
	}

	if((EEPROM_SAVE_FLAG == eeprom_parameter[PAR_SAVE_FLAG]))
	{
        u16_crc = 0;
		for(i = 0; i < MAX_PARAMETER_NUM; i++)
		{
			parameter[i] = eeprom_parameter[i];
            u16_crc +=  eeprom_parameter[i];
		}
        if(u16_crc != eeprom_parameter[PAR_CRC_DATA])           //校验值不正确
        {
            bFlagDataErr = 1;
        }
	}
    else
    {
        bFlagDataErr = 1;
    }
	
    if(1 == bFlagDataErr)
	{
		//Get_Default_Value();
        DefaultParameter();
        u16_crc = 0;
		for(i = 0; i < MAX_PARAMETER_NUM; i++)
		{
			eeprom_parameter[i] = parameter[i];
            u16_crc += parameter[i];
		}
        eeprom_parameter[PAR_CRC_DATA] = u16_crc;               //校验值
		eeprom_parameter[PAR_SAVE_FLAG] = EEPROM_SAVE_FLAG;     //保存标志

		FLASH_Unlock();
		for (i = 0; i < MAX_PARAMATER_NUM_FINAL ; i++)
		{
			u16_data = eeprom_parameter[i];
			EE_WriteVariable(VirtAddVarTab[i], u16_data);
		}
		FLASH_Lock();
	}		   
	ProcessUpdateParameter(); //更新工位定位参数
}

/****************************************************************************************
  * 函数名称： set_parameter
  * 函数描述： 设置参数
  * 其它说明：
****************************************************************************************/
void set_parameter(void)
{
	unsigned char i = 10;
	short int nTempValueL8;
	short int nTempValueH8;
	short int nTempValue;

	for(i=0; i<MAX_PARAMETER_NUM; i++)
	{
		//注意，现传送低8位，后传送高8位
		nTempValueL8 = comm_recv_buf[(i*2) + 10]; 		//L8
		nTempValueH8 = comm_recv_buf[(i*2) + 1 + 10];	//H8
		//注意，现传送高8位，后传送低8位
        //nTempValueH8 = comm_recv_buf[(i*2) + 10]; 	//H8 
        //nTempValueL8 = comm_recv_buf[(i*2) + 1 + 10];	//L8
		nTempValue = nTempValueH8 << 8;
		nTempValue += nTempValueL8;	
		parameter[i] = nTempValue;	
	}
   	save_parameter();
}

/*********************************************************************************************
  * 函数名称： get_parameter
  * 函数描述： 获取参数
  * 其它说明：（1）获取EEPROM中保存的参数
*********************************************************************************************/
unsigned int get_parameter(void)
{
	unsigned int nLen = 0;
	unsigned int i = 0;

	for(i=0; i<MAX_PARAMETER_NUM; i++)
	{
		//注意，现传送低8位，后传送高8位
		comm_send_buf[10+(i*2)]   = parameter[i] & 0xFF;  //L8
		comm_send_buf[10+(i*2)+1] = parameter[i] >> 8;	  //H8
        //注意，现传送高8位，后传送低8位
//        comm_send_buf[10+(i*2)] = parameter[i] >> 8;	    //H8
//        comm_send_buf[10+(i*2)+1] = parameter[i] & 0xFF; 	//L8
	}
	nLen = i*2;
	ProcessUpdateParameter(); //更新工位定位参数
	return nLen;
}

void save_parameter(void)
{
	unsigned char i = 0;
	unsigned short int u16_data;
    unsigned short int u16_crc;

    u16_crc = 0;
	for(i = 0; i < MAX_PARAMETER_NUM; i++)
	{
		eeprom_parameter[i] = parameter[i];
        u16_crc += parameter[i];
	}
    eeprom_parameter[PAR_CRC_DATA] = u16_crc;               //校验值
	eeprom_parameter[PAR_SAVE_FLAG] = EEPROM_SAVE_FLAG;     //保存标志

	FLASH_Unlock();	
	for (i = 0; i < MAX_PARAMETER_NUM+2; i++)
	{
		u16_data = eeprom_parameter[i];
		EE_WriteVariable(VirtAddVarTab[i], u16_data);
	}
	FLASH_Lock();
	ProcessUpdateParameter(); //更新工位定位参数
}
