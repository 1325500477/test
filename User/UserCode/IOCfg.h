#ifndef __IO_CFG__H__
#define __IO_CFG__H__

#ifdef __cplusplus
extern "C"
{
#endif

//***** ����˿ڶ���****************************************************************
    #define SN_CAR_CHANNAL_ORG			IN1	   // ץ��С��ԭ����
    #define SN_PRETARGETING_CHECK_CARD	IN2	   // Ԥ��λ�쿨������
    #define SN_WARM_CHECK_CARD			IN3	   // ��ѹλ�쿨������
    #define SN_COOL_CHECK_CARD        	IN4	   // ��ѹλ�쿨������
    #define SN_PREPOSITION_ORG			IN5	   // Ԥ��λԭ�㴫����
    #define SN_CLAMP_RIGHT_CLOSE		IN6	   // �Ҳ���ӹرռ��
    #define SN_CLAMP_RIGHT_OPEN			IN7	   // �Ҳ���Ӵ򿪼��
    #define SN_CLAMP_LEFT_CLOSE 		IN8	   // �����ӹرռ��
    #define SN_CLAMP_LEFT_OPEN			IN9	   // �����Ӵ򿪼��
    #define SN_COOL_UP					IN10   // ��ȴ�����������
    #define SN_COOL_DOWN				IN11   // ��ȴ�����½����   
    #define SN_WARM_UP					IN12   // ���Ȳ����������
    #define SN_WARM_DOWN				IN13   // ���Ȳ����½����
    #define SN_PUNCH_DOWN				IN14   // ��ѹ�����½����
    #define SN_PUNCH_UP					IN15   // ��ѹ�����������
    #define SN_WASTESTORAGE_ORG			IN16   // ����λԭ�㴫����
    #define SN_PRETARGETING_BAROMETER   IN17   // Ԥ��λ��ѹ�����Ƿ�������
    #define SN_WASTE_BOX_CHECK          IN18   // �����䵽λ���
    #define SN_WASTE_CHECK_CARD         IN19   // ����λ���������Ƿ��з���
    #define SN_LIFT_HORIZON_ORG     	IN20   // ����С��ˮƽԭ��
    #define SN_RECEIVE_CARD        	    IN21   // �ӿ���ѹ�����Ƿ��������к�Ŀ�
    #define SN_CARD_SLOT                IN22   // ���ݿ��ۼ���Ƿ��п�
    #define SN_LIFT_UP_DN_ORG         	IN23   // ����С��ԭ�� 
    #define SN_RW_CH_NEAR_LIFT	    	IN24   // ��дģ�����ͨ��/�Ͽ�����ͨ��������
    #define SN_WASTER_CARD_BOX_FULL     IN25   // �Ͽ�����������
    #define SN_WASTER_CARD_BOX_EMPTY    IN26   // �Ͽ���մ�����
    #define SN_WASTER_CARD_BOX_CHECK    IN27   // �Ͽ��䵽λ��⴫����
    #define SN_RW_CH_NEAR_OUTSIDE       IN28   // �����ڴ�����
    #define SN_WASTE_BOX_FULL           IN29   // ��������������
    //***** ����˿ڶ��壨����ֱ�����������ָʾ��...��*******************************
    #define DM_WARM_CONTROL				DM1	   // ���ȼ̵�������
    #define DM_COOL_CONTROL				DM2	   // ��ȴ�̵�������
    #define DM_PRES_CONTROL				DM3	   // ��ѹ�̵�������
    #define DM_CLAMP_RIGHT				DM4	   // �Ҳ���ӵ��
    #define DM_CLAMP_LEFT				DM5    // �����ӵ��
    #define DM_ALL_CONTROL				DM6	   // ��Һѹ����
    #define DM_PNEUMATIC_SWITCH			DM7	   // ��������
    #define DM_HEATER_POWER             DM8    // ���ȹܿ���
    #define DM_RECEIVE_CARD				DM9	   // �ӿ���տ���
    #define DM_PRETARGETING_VACUUMCUP	DM10   // Ԥ��λ�����
    #define DM_HYDRULIC_POWER           DM11   // Һѹ������ 


    //***** ���������Ŷ���***********************************************************
    #define SM_CHANNEL              	SM_1   // ץ��С�����SM101
    #define SM_PRETARGETING             SM_2   // Ԥ��λ���SM102   
    #define SM_WASTESTORAGE             SM_3   // ���ϳ����SM103
    #define SM_RWMODULE                 SM_4   // ��дģ����SM104
    #define SM_LIFT_HORIZON_MOVE        SM_5   // ����С��ˮƽ�ƶ����
    #define SM_LIFT_UP_DN             	SM_6   // ����С��SM106
    //***** �������ת��������********************************************************
    #define DIR_SM_CHANNEL_FORWARD     	    SM_CW  // ͨ�����ǰ�� 
    #define DIR_SM_CHANNEL_BACKWARD         SM_CCW // ͨ���������
    #define DIR_SM_PRETARGETING_FORWARD     SM_CW // Ԥ��λ����
    #define DIR_SM_PRETARGETING_BACKWARD    SM_CCW  // Ԥ��λ����
    #define DIR_SM_LIFT_UP          		SM_CCW // ����С������
    #define DIR_SM_LIFT_DN        			SM_CW  // ����С���½�
    #define DIR_SM_LIFT_HORIZON_BACK        SM_CCW // ����ˮƽ����
    #define DIR_SM_LIFT_HORIZON_FRONT       SM_CW  // ����ˮƽǰ��
    #define DIR_SM_RW_CH_IN_FROM_LIFT       SM_CCW // ��дģ�����
    #define DIR_SM_RW_CH_OUT_LIFT        	SM_CW  // ��дģ����� 
    #define DIR_SM_ENTER_RECYCLE_BOX        SM_CW  // �������� 
    //***** ֱ���������״̬����********************************************************
    #define DM_CLAMP_OPEN           	DM_CW  // ������Ƶ��˳ʱ��ת��
    #define DM_CLAMP_CLOSE          	DM_CCW // ������Ƶ����ʱ��ת��
    #define DMS_LIGHT_ON            	DM_ON  // ��ťָʾ������
    #define DMS_LIGHT_OFF           	DM_OFF // ��ťָʾ��Ϩ��

    //***** �����������״̬����********************************************************
    #define SMS_ENABLE              	SM_ENABLE  // ʹ�ܵ��
    #define SMS_DISABLE             	SM_DISABLE // ȥ���ʹ��

    /****** �����Ҫ�������궨�壬����ϸ�˶�*************************/
    #define SM1_DIR_ABS_P	        	SM_CW	//���Է���������
    #define SM2_DIR_ABS_P	        	SM_CCW	//���Է���������
    #define SM3_DIR_ABS_P	       	 	SM_CCW	//���Է���������
    #define SM4_DIR_ABS_P	        	SM_CW	//���Է���������
    #define SM5_DIR_ABS_P	        	SM_CW	//���Է���������
    #define SM6_DIR_ABS_P	        	SM_CCW	//���Է���������
    #define SM7_DIR_ABS_P	        	SM_CW	//���Է���������
    #define SM8_DIR_ABS_P	        	SM_CW	//���Է���������

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
