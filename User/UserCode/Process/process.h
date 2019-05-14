#ifndef __PROCESS_H__
#define __PROCESS_H__

/*************状态定义**************/
#define STATUS_UNKNOWE       	   0
#define STATUS_RIGHT_CLAMP_LOOSE   1
#define STATUS_RIGHT_CLAMP_CLOSE   2
#define STATUS_LIFT_CLAMP_LOOSE    1
#define STATUS_LIFT_CLAMP_CLOSE    2

/*************输入端口定义**************/

typedef enum
{
	ERROR_CHANNAL_LEAVE_ORG_FAIL  = 10001,  		//10001通道离开原点失败
	ERROR_CHANNAL_RETURN_ORG_FAIL,  				//10002通道返回原点失败
	ERROR_CHANNAL_TONEXT_FAIL,    					//10003通道到达对侧传感器失败
	ERROR_CHANNAL_NOT_AT_OGG,    					//10004通道不在原点
	ERROR_WARMING_UPDOWN_ABNOMAL,  					//10005加热位置传感器异常
	ERROR_WARMING_DOWN_FAIL,      					//10006加热下降失败
	ERROR_WARMING_RISE_FAIL,     		    		//10007加热升起失败
	ERROR_COOLING_UPDOWN_ABNOMAL,  					//10008冷却位置传感器异常
	ERROR_COOLING_DOWN_FAIL,      					//10009冷却下降失败
	ERROR_COOLING_RISE_FAIL,      					//10010冷却升起失败
	ERROR_PRESS_UPDOWN_ABNOMAL, 					//10011冲压位置传感器异常
	ERROR_PRESS_DOWN_FAIL,      					//10012冲压下降失败
	ERROR_PRESS_RISE_FAIL,      					//10013冲压升起失败
	ERROR_RIGHT_CLAMP_CLOSE_FAIL_SERSOR_FAULT,    	//10014 右边传感器故障导致夹子关闭失败
	ERROR_RIGHT_CLAMP_CLOSE_FAIL_MOTOR_FAULT,     	//10015 右边电机故障导致夹子关闭失败	
	ERROR_RIGHT_CLAMP_OPEN_FAIL_SERSOR_FAULT,     	//10016 右边传感器故障导致夹子打开失败
	ERROR_RIGHT_CLAMP_OPEN_FAIL_MOTOR_FAULT,      	//10017 右边电机故障导致夹子打开失败	
	ERROR_LEFT_CLAMP_CLOSE_FAIL_SERSOR_FAULT,    	//10018 左边传感器故障导致夹子关闭失败
	ERROR_LEFT_CLAMP_CLOSE_FAIL_MOTOR_FAULT,     	//10019 左边电机故障导致夹子关闭失败	
	ERROR_LEFT_CLAMP_OPEN_FAIL_SERSOR_FAULT,     	//10020 左边传感器故障导致夹子打开失败
	ERROR_LEFT_CLAMP_OPEN_FAIL_MOTOR_FAULT,      	//10021 左边电机故障导致夹子打开失败
	ERROR_TROLLEY_STEP_MOTOR_FAULT,					//10022 小车电机损坏




}ModulesErrorCode;


void InitializeMachine(void);
void ResetAllSignal(void);
void StepCtrlMachine(void);

// 其它变量的声明
extern unsigned char ParamBuf[];
extern unsigned char gnOperationNo;
extern unsigned char gStartBottonPressed;
extern unsigned char gPauseBottonPressed;
extern unsigned char CardStatus[10];

#endif
