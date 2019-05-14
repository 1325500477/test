#ifndef __PROCESS_H__
#define __PROCESS_H__

/*************״̬����**************/
#define STATUS_UNKNOWE       	   0
#define STATUS_RIGHT_CLAMP_LOOSE   1
#define STATUS_RIGHT_CLAMP_CLOSE   2
#define STATUS_LIFT_CLAMP_LOOSE    1
#define STATUS_LIFT_CLAMP_CLOSE    2

/*************����˿ڶ���**************/

typedef enum
{
	ERROR_CHANNAL_LEAVE_ORG_FAIL  = 10001,  		//10001ͨ���뿪ԭ��ʧ��
	ERROR_CHANNAL_RETURN_ORG_FAIL,  				//10002ͨ������ԭ��ʧ��
	ERROR_CHANNAL_TONEXT_FAIL,    					//10003ͨ������Բഫ����ʧ��
	ERROR_CHANNAL_NOT_AT_OGG,    					//10004ͨ������ԭ��
	ERROR_WARMING_UPDOWN_ABNOMAL,  					//10005����λ�ô������쳣
	ERROR_WARMING_DOWN_FAIL,      					//10006�����½�ʧ��
	ERROR_WARMING_RISE_FAIL,     		    		//10007��������ʧ��
	ERROR_COOLING_UPDOWN_ABNOMAL,  					//10008��ȴλ�ô������쳣
	ERROR_COOLING_DOWN_FAIL,      					//10009��ȴ�½�ʧ��
	ERROR_COOLING_RISE_FAIL,      					//10010��ȴ����ʧ��
	ERROR_PRESS_UPDOWN_ABNOMAL, 					//10011��ѹλ�ô������쳣
	ERROR_PRESS_DOWN_FAIL,      					//10012��ѹ�½�ʧ��
	ERROR_PRESS_RISE_FAIL,      					//10013��ѹ����ʧ��
	ERROR_RIGHT_CLAMP_CLOSE_FAIL_SERSOR_FAULT,    	//10014 �ұߴ��������ϵ��¼��ӹر�ʧ��
	ERROR_RIGHT_CLAMP_CLOSE_FAIL_MOTOR_FAULT,     	//10015 �ұߵ�����ϵ��¼��ӹر�ʧ��	
	ERROR_RIGHT_CLAMP_OPEN_FAIL_SERSOR_FAULT,     	//10016 �ұߴ��������ϵ��¼��Ӵ�ʧ��
	ERROR_RIGHT_CLAMP_OPEN_FAIL_MOTOR_FAULT,      	//10017 �ұߵ�����ϵ��¼��Ӵ�ʧ��	
	ERROR_LEFT_CLAMP_CLOSE_FAIL_SERSOR_FAULT,    	//10018 ��ߴ��������ϵ��¼��ӹر�ʧ��
	ERROR_LEFT_CLAMP_CLOSE_FAIL_MOTOR_FAULT,     	//10019 ��ߵ�����ϵ��¼��ӹر�ʧ��	
	ERROR_LEFT_CLAMP_OPEN_FAIL_SERSOR_FAULT,     	//10020 ��ߴ��������ϵ��¼��Ӵ�ʧ��
	ERROR_LEFT_CLAMP_OPEN_FAIL_MOTOR_FAULT,      	//10021 ��ߵ�����ϵ��¼��Ӵ�ʧ��
	ERROR_TROLLEY_STEP_MOTOR_FAULT,					//10022 С�������




}ModulesErrorCode;


void InitializeMachine(void);
void ResetAllSignal(void);
void StepCtrlMachine(void);

// ��������������
extern unsigned char ParamBuf[];
extern unsigned char gnOperationNo;
extern unsigned char gStartBottonPressed;
extern unsigned char gPauseBottonPressed;
extern unsigned char CardStatus[10];

#endif
