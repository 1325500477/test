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
#define PULSE_TO_MILLIMETER(pul)   (pul*24) //��������ת��Ϊ���ף�24Ϊ��1mm��������
/*************����˿ڶ���**************/

typedef enum
{
	ERROR_CHANNAL_LEAVE_ORG_FAIL  = 11001,  		//11001 ץ��С���뿪ԭ��ʧ��
	ERROR_CHANNAL_RETURN_ORG_FAIL,  				//11002 ץ��С������ԭ��ʧ��
	ERROR_WARMING_DOWN_FAIL,      					//11003 �����½�ʧ��
	ERROR_WARMING_RISE_FAIL,     		    		//11004 ��������ʧ��
	ERROR_COOLING_DOWN_FAIL,      					//11005 ��ȴ�½�ʧ��
	ERROR_COOLING_RISE_FAIL,      					//11006 ��ȴ����ʧ��
	ERROR_PRESS_DOWN_FAIL,      					//11007 ��ѹ�½�ʧ��
	ERROR_PRESS_RISE_FAIL,      					//11008 ��ѹ����ʧ��
	ERROR_RIGHT_CLAMP_CLOSE_FAIL,    				//11009 �ұ߼��ӹر�ʧ��
	ERROR_RIGHT_CLAMP_OPEN_FAIL,     				//11010 �ұ߼��Ӵ�ʧ��
	ERROR_LEFT_CLAMP_CLOSE_FAIL,			    	//11011 ��߼��ӹر�ʧ��
	ERROR_LEFT_CLAMP_OPEN_FAIL,     				//11012 ��߼��Ӵ�ʧ��
	ERROR_TROLLEY_CAR_POSITION_PARA_MAX,			//11013 ץ��С���ϲ㴫������ݳ����������ֵ
	ERROR_PRETARGETING_RETURN_ORIGIN_FAIL,			//11014 Ԥ��λ����ԭ��ʧ��
	ERROR_PRETARGETING_LEAVE_ORIGIN_FAIL,			//11015 Ԥ��λ�뿪ԭ��ʧ��
	ERROR_WASTESTORAGE_RETURN_ORIGIN_FAIL,			//11016 �Ͽ�λ����ԭ��ʧ��
	ERROR_WASTESTORAGE_LEAVE_ORIGIN_FAIL,			//11017 �Ͽ�λ�뿪ԭ��ʧ��	
	ERROR_PRETARGETING_NONE_CARD, 					//11018 Ԥ��λ�޿�
	ERROR_PRETARGETING_FROM_COOL_NONE_CARD, 		//11019 Ԥ��λ����ѹλ�޿�
	ERROR_WARM_FROM_COOL_NONE_CARD, 				//11020 ��ѹλ����ѹλ�޿�
	ERROR_COOL_FROM_PRETARGETING_NONE_CARD, 		//11021 ��ѹλ��Ԥ��λ�޿�
	ERROR_PRETARGETING_FROM_WASTE_NONE_CARD, 		//11022 Ԥ��λ������λ�޿�
	ERROR_ELEVATOR_CAR_LEAVE_ORIGIN_FAIL ,   		//11023 ��д���С���뿪ԭ��ʧ��
	ERROR_ELEVATOR_CAR_RETURN_ORIGIN_FAIL, 		    //11024 ����С������ԭ��ʧ��
	ERROR_ELEVATOR_CAR_POSITION_PARA_MAX,   		//11025 ����С���ϲ㴫������ݳ����������ֵ
	ERROR_CONNECTION_LEAVE_ORIGIN_FAIL,			    //11026 �ν�ģ���뿪ԭ��ʧ��
	ERROR_CONNECTION_RETURN_ORIGIN_FAIL,		    //11027 �ν�ģ�鷵��ԭ��ʧ��
	ERROR_CONNECTION_TO_SENDCARD_LEAVE_ORIGIN_FAIL, //11028 �ν�ģ�鵽����λ�뿪ԭ��ʧ��
	ERROR_RWMODULE_NOT_CHECK_CARD,					//11029 ��дģ��û�м�⵽��Ƭ
	ERROR_RWMODULE_NOT_IN_CARD,						//11030 ��дģ��û�п�Ƭ����
	ERROR_CARD_SLOT_NOT_IN_CARD,					//11031 ����û�п�Ƭ����
	ERROR_RECEIVE_CARD_POSITION_NOT_CARD,			//11032 ���ݽӿ�λû��������
	ERROR_PRETARGETING_UP_LEAVE_ORIGIN_FAIL,		//11033 Ԥ��λ����ʱ�뿪ԭ��ʧ��
	ERROR_WASTESTORAGE_DOWN_LEAVE_ORIGIN_FAIL,		//11034 �Ͽ�λ�½��뿪ԭ��ʧ��

}ModulesErrorCode;

enum ENUM_CLAMPCARDCAR_LOCATION //ץ��С��λ��
{
	ENUM_UNKNOW,	    	  //0λ��δ֪		   
    ENUM_INSIDE_WARE,		  //1С������ѹ��λ	   
    ENUM_INSIDE_COOL,	      //2С������ѹ��λ	     
	ENUM_INSIDE_PRETARGETING, //3С����Ԥ��λ��λ   
	ENUM_INSIDE_CUT_CARD,     //4С���ڼ�����λ 	
	ENUM_INSIDE_WASTE,	  	  //5С���ڷ��Ϲ�λ	    
    ENUM_INSIDE_ORIGIN,	   	  //6С����ԭ��		   	
};
enum ENUM_ELEVATOR_CAR_LOCATION //Elevator car position
{
	ENUM_ELEVATOR_UNKNOW,	  //0״̬λ��
    ENUM_RECEIVE_CARD,	      //1�ӿ�λ  			   		   
    ENUM_ELEVATOR_TO_RWMODULE,//2���ݵ���дģ��	  
    ENUM_RWMODULE_TO_ELEVATOR,//3��дģ�鵽����(�ÿ�����)
	ENUM_WASTE_CARD, 		  //4�Ͽ�λ    	
	ENUM_OCR_CARD,	     	  //5OCR ���λ	
   	ENUM_ELEVATOR_CAR_ORG,	  //6����ԭ��
};
void InitializeMachine(void);
void ResetAllSignal(void);
void StepCtrlMachine(void); 
// ��������������
extern unsigned char ParamBuf[];
extern unsigned char gnOperationNo;
extern void ProcessUpdateParameter(void); //���¹�λ��λ����

#endif
