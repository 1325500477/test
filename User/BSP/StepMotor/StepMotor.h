#ifndef __STEP_MOTOR_H__
#define __STEP_MOTOR_H__

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct _step_Ctr
{
    unsigned char bAbsDir;				//�����˶�����
    unsigned char bBrakeFlag;			//����ֹͣ��־
    unsigned char bHaveDisabledFlag;    //֮ǰȥʹ�ܹ�
	unsigned char bMotorRunStatus;		//���״̬��0-��ʾ��ǰ���������ת��1-��ʾ���ת���У�
	unsigned char bPulseToggleFlag;		//����ȡ����־
    unsigned char bRunNoStop;			//Ҫ����һֱת��
    unsigned char bMotorType;           //0-��ʾ���������1-��ʾ�ŷ����

    unsigned int nAccSteps;				//���ٲ���
	unsigned int nDecSteps;				//���ٲ���
	unsigned long nConstantSteps;		//���ٶβ���
	unsigned int nSpeedTableIndex;		//���ٱ��±��ƶ�
	unsigned long nStepsHaveRun;		//�Ѿ����еĲ���
	unsigned int *pSpeedTable;			//ָ����ٱ��ָ��
    unsigned int nSpeedTableMaxStep;	//���ٱ���
	unsigned int nResetSpeed;			//��λ�ٶ�
	unsigned int nRunSpeed1;			//�����ٶ�1
	unsigned int nRunSpeed2;			//�����ٶ�2
	unsigned int nRunSpeed3;			//�����ٶ�3
	unsigned int nWaitStableTime; 		//ʹ��֮����ȶ�ʱ��
	signed long nAbsStepCount;			//�����˶������
	
	unsigned char bEnableFlag;			//���ʹ�ܱ�־
	unsigned char bFlagChangeSpeed;		//�Ƿ���Ҫ����
	unsigned int nTargetSpeed;			//Ŀ������ٶ� 
	unsigned int nNewChangeSpeed;		//����֮���Ŀ���ٶ�
}stStepCtr;

extern stStepCtr volatile StepMotorCtr[16];

void init_sm_ctrl(void);
void init_step_motor(void);

void StepMotorInitData(unsigned char sm_id,unsigned int max_speed, unsigned long Distance);
bool IsSmRunFinish(unsigned char sm_id);
void sm_run(unsigned char sm_id, unsigned char dir, unsigned int max_speed, unsigned long step); //����������г���
void sm_run_abs(unsigned char sm_id,unsigned int max_speed,signed long step);
void sm_stop(unsigned char sm_id);
void sm_SlowdownStop(uchar sm_id,uint step);

void sm_brake(unsigned char sm_id);
//void sm_break(unsigned char sm_id);		//��ȥ��
//void sm_SlowDown(unsigned char sm_id);	//��ȥ��

void sm_set_enable(unsigned char sm_id, bool enable_flag);
void sm_set_pul(unsigned char sm_id, unsigned char status);
void sm_set_dir(unsigned char sm_id, unsigned char status);

void sm_set_abs(unsigned char sm_id, signed long absStep);
void sm_set_abs_zero(unsigned char sm_id);	//��ͬ��sm_set_abs(sm_id, 0)
signed long sm_get_abs(unsigned char sm_id);
signed long sm_get_abs_step(unsigned char sm_id);


#ifdef __cplusplus
}
#endif

#endif
