#ifndef __STEP_MOTOR_H__
#define __STEP_MOTOR_H__

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct _step_Ctr
{
    unsigned char bAbsDir;				//绝对运动方向
    unsigned char bBrakeFlag;			//减速停止标志
    unsigned char bHaveDisabledFlag;    //之前去使能过
	unsigned char bMotorRunStatus;		//电机状态（0-表示当前电机动作不转；1-表示电机转动中）
	unsigned char bPulseToggleFlag;		//脉冲取反标志
    unsigned char bRunNoStop;			//要求电机一直转动
    unsigned char bMotorType;           //0-表示步进电机；1-表示伺服电机

    unsigned int nAccSteps;				//加速步数
	unsigned int nDecSteps;				//减速步数
	unsigned long nConstantSteps;		//匀速段步数
	unsigned int nSpeedTableIndex;		//加速表下标移动
	unsigned long nStepsHaveRun;		//已经运行的步数
	unsigned int *pSpeedTable;			//指向加速表的指针
    unsigned int nSpeedTableMaxStep;	//加速表长度
	unsigned int nResetSpeed;			//复位速度
	unsigned int nRunSpeed1;			//运行速度1
	unsigned int nRunSpeed2;			//运行速度2
	unsigned int nRunSpeed3;			//运行速度3
	unsigned int nWaitStableTime; 		//使能之后的稳定时间
	signed long nAbsStepCount;			//绝对运动坐标点
	
	unsigned char bEnableFlag;			//电机使能标志
	unsigned char bFlagChangeSpeed;		//是否需要变速
	unsigned int nTargetSpeed;			//目标最大速度 
	unsigned int nNewChangeSpeed;		//变速之后的目标速度
}stStepCtr;

extern stStepCtr volatile StepMotorCtr[16];

void init_sm_ctrl(void);
void init_step_motor(void);

void StepMotorInitData(unsigned char sm_id,unsigned int max_speed, unsigned long Distance);
bool IsSmRunFinish(unsigned char sm_id);
void sm_run(unsigned char sm_id, unsigned char dir, unsigned int max_speed, unsigned long step); //步进电机运行程序
void sm_run_abs(unsigned char sm_id,unsigned int max_speed,signed long step);
void sm_stop(unsigned char sm_id);
void sm_SlowdownStop(uchar sm_id,uint step);

void sm_brake(unsigned char sm_id);
//void sm_break(unsigned char sm_id);		//可去掉
//void sm_SlowDown(unsigned char sm_id);	//可去掉

void sm_set_enable(unsigned char sm_id, bool enable_flag);
void sm_set_pul(unsigned char sm_id, unsigned char status);
void sm_set_dir(unsigned char sm_id, unsigned char status);

void sm_set_abs(unsigned char sm_id, signed long absStep);
void sm_set_abs_zero(unsigned char sm_id);	//等同于sm_set_abs(sm_id, 0)
signed long sm_get_abs(unsigned char sm_id);
signed long sm_get_abs_step(unsigned char sm_id);


#ifdef __cplusplus
}
#endif

#endif
