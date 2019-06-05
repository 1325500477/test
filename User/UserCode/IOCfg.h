#ifndef __IO_CFG__H__
#define __IO_CFG__H__

#ifdef __cplusplus
extern "C"
{
#endif

    //***** ����˿ڶ���****************************************************************
#define SN_PREPOSITION_ORG			IN1			// Ԥ��λԭ��
#define SN_BUTTON2					IN2			// �ǽ�λ��⴫����

#define SN_SUCK_CARD_CAR_ORG  		IN3			// ����С��ԭ�㴫����
#define SN_SUCK_ORG				    IN4			// ����ԭ�㴫����
#define SN_CAR_CHANNAL_ORG			IN5			// ͨ��ԭ����
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
#define SN_WASTESTORAGE_ORG			IN16		// �Ͽ����

#define SN_TEST                     IN21
    //***** ����˿ڶ��壨����ֱ�����������ָʾ��...��*******************************
#define DM_BUTTON1					DM1		// ��ť1ָʾ��
#define DM_BUTTON2					DM2		// ��ť2ָʾ��
#define DM_BUTTON3					DM3		// ��ť3ָʾ��
#define DM_BUTTON4					DM4		// ��ť4ָʾ��
	
#define DM_WARM_CONTROL				DM5		// ���ȼ̵���5����
#define DM_COOL_CONTROL				DM6		// ��ȴ�̵���6����
#define DM_PRES_CONTROL				DM7		// ��ѹ�̵���7����
#define DM_ALL_CONTROL				DM8		// �̵ܼ���8����	
#define DM_PNEUMATIC_SWITCH			DM2		// ��������
#define DM_PRETARGETING_VACUUMCUP	DM3		// Ԥ��λ�����
#define DM_WASTESTORAGE_VACUUMCUP	DM4		// ���ϳ������
#define DM_CLAMP_RIGHT				DM9		// �Ҳ���ӵ��
#define DM_CLAMP_LEFT				DM10	// �����ӵ��


    //***** ���������Ŷ���***********************************************************
#define SM_CHANNEL              	SM_1        // ץ��С�����SM101
#define SM_WASTESTORAGE             SM_2        // ���ϳ����SM102
#define SM_PRETARGETING             SM_3        // Ԥ��λ���SM103
#define SM_SUCK_CARD_CAR            SM_4        // ����С�����SM104
#define SM_SUCK                     SM_5        // ���̵��SM105



    //***** �������ת��������********************************************************
#define DIR_SM_CHANNEL_BACKWARD     SM_CCW       //ͨ��������� 
#define DIR_SM_CHANNEL_FORWARD     	SM_CW        //ͨ�����ǰ�� 
#define DIR_SM_PRETARGETING_FORWARD  SM_CW       // Ԥ��λ����
#define DIR_SM_PRETARGETING_BACKWARD SM_CCW      // Ԥ��λ����
#define DIR_SM_SUCK_CARD_CAR_FORWARD  SM_CW      //����С��ǰ��
#define DIR_SM_SUCK_CARD_CAR_BACKWARD SM_CCW     //����С������
#define DIR_SM_SUCK_UP    	        SM_CW        //��������
#define DIR_SM_SUCK_DOWN   	        SM_CCW       //�����½�

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

#define RELAY_ON    1    //�̵�����
#define RELAY_OFF   0    //�̵�����

#ifdef __cplusplus
}
#endif

#endif
