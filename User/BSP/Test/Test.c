#include <string.h>
#include "stm32f10x.h"
#include "bsp.h"

//IN1:0x0120;	IN2:0x0122; 	IN3:0x0124;	IN4:0x0126
//	写地址	1-开始计数	0-清零
//	读地址	获取计数值
void board_test(void)
{
//测试FPGA自带的计数器功能
	volatile unsigned short nValue = 0;
	unsigned short nStatus = 0;
	//unsigned int i = 0;
	unsigned char bPinStatusBefore = 0;
	unsigned char bPinStatusAfter = 1;

	nStatus = get_fpga_version();
	if(nStatus)
	{
	
	}
	else
	{
	
	}

	nValue = fpga_read(0x0100);
	nValue = fpga_read(0x0102);

	fpga_write(0x0120, 0);
	nValue = fpga_read(0x0120);

	bPinStatusBefore = input_get_one(1);
	fpga_write(0x0120, 1);

 	sm_run(1,1,990,65000);
//	for(i = 0; i < 60000; i++)
//	{
//		GPIO_WriteBit(GPIOE, GPIO_Pin_5,Bit_SET);
//		//delay(1);
//		GPIO_WriteBit(GPIOE, GPIO_Pin_5,Bit_RESET);
//		//delay(1);
//		nValue = fpga_read(0x0120);
//	}
	delay(1);

	while(StepMotorCtr[0].bMotorRunStatus != FREE)
		;
	bPinStatusAfter = input_get_one(1);
	nValue = fpga_read(0x0120);		  	
	if((bPinStatusBefore != bPinStatusAfter) && bPinStatusBefore)		//有变化，且起始电平为高电平
		nValue += 1;
	else
		nValue = fpga_read(0x0120);

	bPinStatusAfter = input_get_one(1);

//	while(nValue < 0x55BB)
//	{
//		nValue = fpga_read(0x0120);
//		delay(1);
//	}

/*
	static unsigned long baseTime; 	
	static unsigned char operation = STEP1;
	static bool flag_dir = 0;
	unsigned char bBusy = 1;
	unsigned char i = 0;
//	unsigned short j = 0;
//	unsigned char str_info[10];
//	static unsigned char sm_id;
	static unsigned char dm_id;
	switch(operation)
	{
		case STEP1:
//		   	if(0 == input_get_one(1))
//				sm_id = 1;
//			else if(0 == input_get_one(2))
//				sm_id = 2;
//			else if(0 == input_get_one(3))
//				sm_id = 3;
//			else if(0 == input_get_one(4))
//				sm_id = 4;
//			else if(0 == input_get_one(5))
//				sm_id = 5;
//			else if(0 == input_get_one(6))
//				sm_id = 6;
//			else if(0 == input_get_one(7))
//				sm_id = 7;
//			else if(0 == input_get_one(8))
//				sm_id = 8;
//			else
//				sm_id = 1;
			flag_dir = !flag_dir;
			for(i = 1; i <= 8; i++)
				sm_run(i,flag_dir,20,640);

			dm_id++;
			if(dm_id > 12)
				dm_id = 1;
			dm_ctrl_one(dm_id, DM_CW);
			
			baseTime = systemTime;
			operation = STEP2;
			break;
		case STEP2:
			bBusy = 0;
			for(i = 1; i <= 8; i++)
			{
				if(m_SM_Status[i-1] != FREE)
				{
					bBusy = 1;
					break;
				}
			}
			if(!bBusy)
				operation = STEP3;
			break;
		case STEP3:
			flag_dir = !flag_dir;
			//sm_run(sm_id,flag_dir,50,6400);
			for(i = 1; i <= 8; i++)
				sm_run(i,flag_dir,20,640);
			dm_ctrl_one(dm_id, DM_CCW);
			baseTime = systemTime;
			operation = STEP4;
			break;
		case STEP4:
			bBusy = 0;
			for(i = 1; i <= 8; i++)
			{
				if(m_SM_Status[i-1] != FREE)
				{
					bBusy = 1;
					break;
				}
			}
			if(!bBusy)
			{
				dm_ctrl_one(dm_id, DM_FREE);
				operation = STEP9;
			}
			break;
        case STEP9:
			for(i = 1; i <= 8; i++)
            	sm_set_enable(i,0);
            baseTime = systemTime;
            operation = STEP10;
            break;
        case STEP10:
            if(check_time_delay(baseTime,10))
            {
                operation = STEP1;
            }
            break;
		default:
			break;
	}
	*/
}


void fpga_test()
{
	char cBuf[100];
	unsigned char i = 0;
	unsigned short int read_data = 0;
//输入
//10  	IN16~IN1
//12	IN32~IN17
//14	8'h0, IN40~IN33
//18	h5aa5

//80	tph_finish
//82	error_cod

	read_data =  fpga_read(0x10);
	memset(cBuf, 0, sizeof(cBuf));
	sprintf(cBuf, "IN16~IN1 : 0x%x\n", read_data);
	debug_info(cBuf);
	read_data =  fpga_read(0x12);
	memset(cBuf, 0, sizeof(cBuf));
	sprintf(cBuf, "IN32~IN17 : 0x%x\n", read_data);
	debug_info(cBuf);
	read_data =  fpga_read(0x14);
	memset(cBuf, 0, sizeof(cBuf));
	sprintf(cBuf, "IN40~IN33 : 0x%x\n", read_data);
	debug_info(cBuf);
	read_data =  fpga_read(0x18);
	memset(cBuf, 0, sizeof(cBuf));
	sprintf(cBuf, "TestData : 0x%x\n", read_data);
	debug_info(cBuf);

	read_data =  fpga_read(0x82);
	memset(cBuf, 0, sizeof(cBuf));
	sprintf(cBuf, "ErrorCode1 : 0x%x\n", read_data);
	debug_info(cBuf);

//输出
//10	sm10_dir
//12	sm11_dir
//14	sm12_dir
//18	sm13_dir

//20	sm10_ena
//22	sm11_ena
//24	sm12_ena
//28	sm13_ena

//40	buzzer_ena
//42	buzzer_freq

//80	tph_begin
//82	clear
//84	data

	//step_motor 10~13 enable & dir
	fpga_write(0x10, 1);
	fpga_write(0x12, 1);
	fpga_write(0x14, 1);
	fpga_write(0x18, 1);
	fpga_write(0x20, 1);
	fpga_write(0x22, 1);
	fpga_write(0x24, 1);
	fpga_write(0x28, 1);

	fpga_write(0x40, 1);
	fpga_write(0x42, 100);

	fpga_write(0x82, 1);	//clear
	for(i = 0; i < 42; i++)	//连续写入42个数据
		fpga_write(0x84, 0x5555);

	read_data = fpga_read(0x80);
	memset(cBuf, 0, sizeof(cBuf));
	sprintf(cBuf, "ErrorCode2 : 0x%x\n", read_data);
	debug_info(cBuf);

	fpga_write(0x80, 1);	//begin

	read_data =  fpga_read(0x82);
	memset(cBuf, 0, sizeof(cBuf));
	sprintf(cBuf, "ErrorCode2 : 0x%x\n", read_data);
	debug_info(cBuf);

	while(1)
	{
		read_data = fpga_read(0x80);
		memset(cBuf, 0, sizeof(cBuf));
		sprintf(cBuf, "TPH Status : 0x%x\n", read_data);
		debug_info(cBuf);
		if(1 == read_data)
			break;
	}
	fpga_write(0x80, 0);	//clear start

	//step_motor 10~13 enable & dir
	fpga_write(0x10, 0);
	fpga_write(0x12, 0);
	fpga_write(0x14, 0);
	fpga_write(0x18, 0);
	fpga_write(0x20, 0);
	fpga_write(0x22, 0);
	fpga_write(0x24, 0);
	fpga_write(0x28, 0);
}	 


void ram_test(void)
{
	uint32_t addr;
	unsigned char bFlag = FALSE;

	static unsigned char index = 0;
	char cBuf[32];
	debug_info("SRAM Test....\n");
		
//	memset(test_data, 0 ,sizeof(test_data));
//	for(addr = 0; addr < sizeof(test_data); addr ++)
//	{
//		test_data[addr] = (unsigned char)addr + index;
//	}
	index ++;
	//内存访问注意8bit模式和16bit模式的配置
	for( addr = SRAM_BLANK1_START_ADDR; addr < SRAM_BLANK1_END_ADDR; addr ++ )
	{
	    *(unsigned char *)addr = (addr + index);
	}
	
	for( addr = SRAM_BLANK1_START_ADDR; addr < SRAM_BLANK1_END_ADDR; addr ++ )
	{
		if ((*(unsigned char *)addr) != ((addr + index) & 0xFF))
		{
			debug_info("SRAM Test error. Address = 0x%08X, Read = 0x%08X, Expected = 0x%08X \r\n", addr, *(uint32_t *)addr, addr);			
			bFlag = TRUE;
			break;
		}
	}

	for( addr = SRAM_BLANK1_START_ADDR; addr < SRAM_BLANK1_END_ADDR; addr +=2 )
	{
	    *(unsigned short int *)addr = ((addr + index) & 0xFFFF);
	}
	for( addr = SRAM_BLANK1_START_ADDR; addr < SRAM_BLANK1_END_ADDR; addr +=2 )
	{
		if ((*(unsigned short int *)addr) != ((addr + index) & 0xFFFF))
		{
			debug_info("SRAM Test error. Address = 0x%08X, Read = 0x%08X, Expected = 0x%08X \r\n", addr, *(uint32_t *)addr, addr);			
			bFlag = TRUE;
			break;
		}
	}
//	
//	for( addr = SRAM_BLANK1_START_ADDR; addr < SRAM_BLANK1_END_ADDR; addr +=4 )
//	{
//	    *(unsigned int *)addr = (addr + index);
//	}
//	for( addr = SRAM_BLANK1_START_ADDR; addr < SRAM_BLANK1_END_ADDR; addr +=4 )
//	{
//		if ((*(unsigned int *)addr) != (addr + index))
//		{
//			debug_info("SRAM Test error. Address = 0x%08X, Read = 0x%08X, Expected = 0x%08X \r\n", addr, *(uint32_t *)addr, addr);			
//			break;
//		}
//	}
	if(!bFlag)
	{
		memset(cBuf, 0, sizeof(cBuf));
		//sprintf(cBuf, "SRAM Test [%d] Success!\n", index);
		debug_info(cBuf);
	}
	else
	{
		memset(cBuf, 0, sizeof(cBuf));
		//sprintf(cBuf, "SRAM Test [%d] Failed!\n", index);
		debug_info(cBuf);	
	}				
}
