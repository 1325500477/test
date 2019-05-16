#ifndef __IO_CFG__H__
#define __IO_CFG__H__

#ifdef __cplusplus
extern "C"
{
#endif

    //***** ����˿ڶ���****************************************************************
#define SN_BUTTON1					IN1			// ��ť1
#define SN_BUTTON2					IN2			// �ǽ�λ��⴫����
#define SN_TROLLEY_UP				IN3			// ����ѹλ��⴫����
#define SN_TROLLEY_DOWN				IN4			// �Źرռ�⴫����
	
#define SN_CHANNAL_ORG				IN5			// ͨ��ԭ����
#define SN_CLAMP_RIGHT_CLOSE		IN6			// �Ҳ���ӹرռ��
#define SN_CLAMP_RIGHT_OPEN			IN7			// �Ҳ���Ӵ򿪼��
#define SN_CLAMP_LEFT_CLOSE 		IN8			// �����ӹرռ��
#define SN_CLAMP_LEFT_OPEN			IN9			// �����Ӵ򿪼��
#define SN_WARM_UP					IN10		// ���Ȳ����������
#define SN_WARM_DOWN				IN11		// ���Ȳ����½����
#define SN_COOL_UP					IN12		// ��ȴ����������
#define SN_COOL_DOWN				IN13		// ��ȴ�����½����
#define SN_PUNCH_DOWN				IN14		// ��ѹ�����½���
#define SN_PUNCH_UP					IN15		// ��ѹ�����������
#define SN_CHANNAL_RIGHT			IN16		// ͨ���Ҳ���


    //***** ����˿ڶ��壨����ֱ�����������ָʾ��...��*******************************
#define DM_BUTTON1					DM1		// ��ť1ָʾ��
#define DM_BUTTON2					DM2		// ��ť2ָʾ��
#define DM_BUTTON3					DM3		// ��ť3ָʾ��
#define DM_BUTTON4					DM4		// ��ť4ָʾ��
	
#define DM_WARM_CONTROL				DM5		// ���ȼ̵���5����
#define DM_COOL_CONTROL				DM6		// ��ȴ�̵���6����
#define DM_PRES_CONTROL				DM7		// ��ѹ�̵���7����
#define DM_ALL_CONTROL				DM8		// �̵ܼ���8����
	
#define DM_CLAMP_RIGHT				DM9		// �Ҳ���ӵ��
#define DM_CLAMP_LEFT				DM10	// �����ӵ��


    //***** ���������Ŷ���***********************************************************
#define SM_CHANNEL              	SM_1        // ͨ�����Ƶ��SM101
#define SM_TROLLEY                	SM_2        // С�����SM102
#define SM_BELT                 	SM_4        // ���ʹ����Ƶ��SM104



    //***** �������ת��������********************************************************
#define DIR_SM_CHANNEL_BACKWARD     SM_CW       //ͨ��������� 
#define DIR_SM_CHANNEL_FORWARD     	SM_CCW      //ͨ�����ǰ�� 

#define DIR_SM_TROLLEY_FORWARD     	SM_CW       // ���ʹ����͵����ǰת������
#define DIR_SM_TROLLEY_BACKWARD    	SM_CCW      // ���ʹ����͵������ת������

    //***** ֱ���������״̬����********************************************************
#define DM_CLAMP_OPEN           	DM_CW       // ������Ƶ��˳ʱ��ת��
#define DM_CLAMP_CLOSE          	DM_CCW      // ������Ƶ����ʱ��ת��
#define DMS_LIGHT_ON            	DM_ON       // ��ťָʾ������
#define DMS_LIGHT_OFF           	DM_OFF      // ��ťָʾ��Ϩ��

    //***** �����������״̬����********************************************************
#define SMS_ENABLE              	SM_ENABLE   // ʹ�ܵ��
#define SMS_DISABLE             	SM_DISABLE  // ȥ���ʹ��


    /****** �����Ҫ�������궨�壬����ϸ�˶�*************************/
#define SM1_DIR_ABS_P	        	SM_CW		//���Է���������
#define SM2_DIR_ABS_P	        	SM_CCW		//���Է���������
#define SM3_DIR_ABS_P	       	 	SM_CCW		//���Է���������
#define SM4_DIR_ABS_P	        	SM_CW		//���Է���������
#define SM5_DIR_ABS_P	        	SM_CW		//���Է���������
#define SM6_DIR_ABS_P	        	SM_CCW		//���Է���������
#define SM7_DIR_ABS_P	        	SM_CW		//���Է���������
#define SM8_DIR_ABS_P	        	SM_CW		//���Է���������

enum _enum_motor_type
{
    STEP_MOTOR  = 0,     //�������
    SERVO_MOTOR = 1,    //�ŷ����
};

#define MOTOR_TYPE_SM1  STEP_MOTOR  //�������
#define MOTOR_TYPE_SM2  STEP_MOTOR  //�������
#define MOTOR_TYPE_SM3  STEP_MOTOR  //�������
#define MOTOR_TYPE_SM4  STEP_MOTOR  //�������
#define MOTOR_TYPE_SM5  STEP_MOTOR  //�������
#define MOTOR_TYPE_SM6  STEP_MOTOR  //�������
#define MOTOR_TYPE_SM7  STEP_MOTOR  //�������
#define MOTOR_TYPE_SM8  SERVO_MOTOR //�ŷ����

#define RELAY5_ON    1    //�̵���5��
#define RELAY5_OFF   0    //�̵���5��
#define RELAY6_ON    1    //�̵���6��
#define RELAY6_OFF   0    //�̵���6��
#define RELAY7_ON    1    //�̵���7��
#define RELAY7_OFF   0    //�̵���7��
#define RELAY8_ON    1    //�̵���8��
#define RELAY8_OFF   0    //�̵���8��



#ifdef __cplusplus
}
#endif

#endif
