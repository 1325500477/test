#ifndef __PROCESS_H__
#define __PROCESS_H__

/*************״̬����**************/
#define STATUS_UNKNOW       	   0 //δ֪״̬
#define STATUS_ORG				   1 //��ԭ��
#define STATUS_POSITION			   2 //����λ
#define STATUS_RIGHT_CLAMP_LOOSE   1 //�ұ߼��Ӵ�״̬
#define STATUS_RIGHT_CLAMP_CLOSE   2 //�ұ߼��ӹر�״̬
#define STATUS_LIFT_CLAMP_LOOSE    1 //��߼��Ӵ�״̬
#define STATUS_LIFT_CLAMP_CLOSE    2 //��߼��ӹر�״̬

/*************����˿ڶ���**************/

typedef enum
{
	ERROR_CHANNAL_LEAVE_ORG_FAIL  = 10001,  		//10001 ץ��С���뿪ԭ��ʧ��
	ERROR_CHANNAL_RETURN_ORG_FAIL,  				//10002 ץ��С������ԭ��ʧ��
	ERROR_WARMING_DOWN_FAIL,      					//10003 �����½�ʧ��
	ERROR_WARMING_RISE_FAIL,     		    		//10004 ��������ʧ��
	ERROR_COOLING_DOWN_FAIL,      					//10005 ��ȴ�½�ʧ��
	ERROR_COOLING_RISE_FAIL,      					//10006 ��ȴ����ʧ��
	ERROR_PRESS_DOWN_FAIL,      					//10007 ��ѹ�½�ʧ��
	ERROR_PRESS_RISE_FAIL,      					//10008 ��ѹ����ʧ��
	ERROR_RIGHT_CLAMP_CLOSE_FAIL,    				//10009 �ұ߼��ӹر�ʧ��
	ERROR_RIGHT_CLAMP_OPEN_FAIL,     				//10010 �ұ߼��Ӵ�ʧ��
	ERROR_LEFT_CLAMP_CLOSE_FAIL,			    	//10011 ��߼��ӹر�ʧ��
	ERROR_LEFT_CLAMP_OPEN_FAIL,     				//10012 ��߼��Ӵ�ʧ��
	ERROR_TROLLEY_CAR_POSITION_PARA_MAX,			//10013 ץ��С���ϲ㴫������ݳ����������ֵ
	ERROR_PRETARGETING_RETURN_ORIGIN_FAIL,			//10014 Ԥ��λ����ԭ��ʧ��
	ERROR_PRETARGETING_LEAVE_ORIGIN_FAIL,			//10015 Ԥ��λ�뿪ԭ��ʧ��
	ERROR_WASTESTORAGE_RETURN_ORIGIN_FAIL,			//10016 �Ͽ�λ����ԭ��ʧ��
	ERROR_WASTESTORAGE_LEAVE_ORIGIN_FAIL,			//10017 �Ͽ�λ�뿪ԭ��ʧ��	
	//����С��
	ERROR_SUCK_CARD_CAR_POSITION_PARA_MAX,          //10018 ����С���ϲ㴫������ݳ����������ֵ
	ERROR_SUCK_CARD_CAR_LEAVE_ORIGIN_FAIL,          //10019 ����С���뿪ԭ��ʧ��
	ERROR_SUCK_CARD_CAR_RETURN_ORIGIN_FAIL,         //10020 ����С������ԭ��ʧ��
	ERROR_SUCK_RETURN_ORIGIN_FAIL,         			//10021 ���̷���ԭ��ʧ��
	ERROR_SUCK_LEAVE_ORIGIN_FAIL,         			//10022 �����뿪ԭ��ʧ��
	//����
	ERROR_SUCK_POSITION_PARA_MAX,					//10023 �����ϲ㴫������ݳ����������ֵ
	//�׿���
	ERROR_BLANK_CARD_BOX_RESET_FAIL,				//10024 �׿��临λʧ��
	ERROR_BLANK_CARD_BOX_LEAVE_ORIGIN_FAIL,			//10025 �׿����뿪ԭ��ʧ��
	ERROR_BLANK_CARD_BOX_EMPTY,						//10026 �׿����
	ERROR_SUCK_VACUUM_CARD_FAIL,					//10027 �����������ʧ��
	ERROR_PRETARGETING_NONE_CARD, 					//10028 Ԥ��λ�޿�
	ERROR_PRETARGETING_FROM_COOL_NONE_CARD, 		//10029 Ԥ��λ����ѹλ�޿�
	ERROR_WARM_FROM_COOL_NONE_CARD, 				//10030 ��ѹλ����ѹλ�޿�
	ERROR_COOL_FROM_PRETARGETING_NONE_CARD, 		//10031 ��ѹλ��Ԥ��λ�޿�
	ERROR_PRETARGETING_FROM_WASTE_NONE_CARD, 		//10032 Ԥ��λ������λ�޿�
	ERROR_WASTE_FROM_PRESS_NONE_CARD, 				//10033 ����λ������λ�޿�
	ERROR_SUCK_FREE_CARD_FAIL, 						//10034 �����ͷſ�ʧ��
	ERROR_PRETARGETING_SUCK_VACUUM_CARD_FAIL,		//10035 Ԥ��λ����ʧ��
	ERROR_ACCESSORY_SUCK_FREE_CARD_FAIL,			//10036 ����λ�����ͷſ�ʧ��
	ERROR_RETURN_CARD_SUCK_CARD_FAIL,				//10037 ���̻�������ʧ��
	ERROR_RETURN_CARD_SUCK_FREE_CARD_FAIL,			//10038 ���̻����ͷſ�ʧ��
	ERROR_SUCK_CARD_DROP_OUT,						//10039 ���������п�Ƭ����

}ModulesErrorCode;

enum ENUM_CLAMPCARDCAR_LOCATION //ץ��С��λ��
{
	ENUM_UNKNOW,	    	  //λ��δ֪		   //0
    ENUM_INSIDE_WARE,		  //С������ѹ��λ	   //1
    ENUM_INSIDE_COOL,	      //С������ѹ��λ	   //2
	ENUM_INSIDE_WASTE,	  	  //С���ڷ��Ϲ�λ	   //3
	ENUM_INSIDE_PRETARGETING, //С����Ԥ��λ��λ   //4
	ENUM_INSIDE_CUT_CARD,     //С���ڼ�����λ 	   //5
    ENUM_INSIDE_ORIGIN,	   	  //С����ԭ��		   //6	
};
enum ENUM_SUCK_CARD_CAR_LOCATION //����С��λ��
{
	ENUM_SUCK_UNKNOW,	      //λ��δ֪		   		   //0
    ENUM_ACCESSORY,	  		  //С�����������ڸ��Ϲ�λ	    //1
	ENUM_PRETARGETING, 		  //С������������Ԥ��λ��λ    //2	
    ENUM_TEMPORARY,	          //С��������������ʱ�ſ���λ  //3
	ENUM_NEW_ACCESSORY,	      //С�������������¸��Ϲ�λ	//4
	ENUM_PRINT,     		  //С�����������ڴ�ӡ��λ 	    //5
	ENUM_BLANK,     		  //С�����������ڿհ׿���λ    //7
    ENUM_SUCK_ORIGIN,		  //С������������ԭ��		   //8	
};



void InitializeMachine(void);
void ResetAllSignal(void);
void StepCtrlMachine(void);

// ��������������
extern unsigned char ParamBuf[];
extern unsigned char gnOperationNo;
extern unsigned char gStartBottonPressed;
extern unsigned char gPauseBottonPressed;
extern void ProcessUpdateParameter(void); //���¹�λ��λ����

#endif
