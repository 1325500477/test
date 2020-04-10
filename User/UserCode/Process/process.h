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
	ERROR_RWCHANNEL_AND_LIFT_CARD_SLOT_ALL_HAVE_CARD = 11001,		//11001 �¿��ư帴λʱ��⵽��дͨ������ݿ���ͬʱ�п�
	ERROR_RWCHANNEL_AND_LIFT_RECEIVE_ALL_HAVE_CARD,					//11002 �¿��ư帴λʱ��⵽��дͨ������ݽӿ����̴����п�
	ERROR_CHANNAL_LEAVE_ORG_FAIL,			  						//11003 ץ��С���뿪ԭ��ʧ��
	ERROR_CHANNAL_RETURN_ORG_FAIL,  								//11004 ץ��С������ԭ��ʧ��
	ERROR_WARMING_DOWN_FAIL,      									//11005 �����½�ʧ��
	ERROR_WARMING_RISE_FAIL,     		    						//11006 ��������ʧ��
	ERROR_COOLING_DOWN_FAIL,      									//11007 ��ȴ�½�ʧ��
	ERROR_COOLING_RISE_FAIL,      									//11008 ��ȴ����ʧ��
	ERROR_PRESS_DOWN_FAIL,      									//11009 ��ѹ�½�ʧ��
	ERROR_PRESS_RISE_FAIL,      									//11010 ��ѹ����ʧ��
	ERROR_RIGHT_CLAMP_CLOSE_FAIL,    								//11011 �ұ߼��ӹر�ʧ��
	ERROR_RIGHT_CLAMP_OPEN_FAIL,     								//11012 �ұ߼��Ӵ�ʧ��
	ERROR_LEFT_CLAMP_CLOSE_FAIL,			    					//11013 ��߼��ӹر�ʧ��
	ERROR_LEFT_CLAMP_OPEN_FAIL,     								//11014 ��߼��Ӵ�ʧ��
	ERROR_TROLLEY_CAR_POSITION_PARA_MAX,							//11015 ץ��С���ϲ㴫������ݳ����������ֵ
	ERROR_PRETARGETING_RETURN_ORIGIN_FAIL,							//11016 Ԥ��λ����ԭ��ʧ��
	ERROR_PRETARGETING_LEAVE_ORIGIN_FAIL,							//11017 Ԥ��λ�뿪ԭ��ʧ��
	ERROR_PRETARGETING_UP_LEAVE_ORIGIN_FAIL,						//11018 Ԥ��λ����ʱ�뿪ԭ��ʧ��	
	ERROR_WASTESTORAGE_RETURN_ORIGIN_FAIL,							//11019 �Ͽ�λ����ԭ��ʧ��
	ERROR_WASTESTORAGE_LEAVE_ORIGIN_FAIL,							//11020 �Ͽ�λ�뿪ԭ��ʧ��
	ERROR_WASTESTORAGE_DOWN_LEAVE_ORIGIN_FAIL,						//11021 �Ͽ�λ�½��뿪ԭ��ʧ��
	ERROR_PRETARGETING_NONE_CARD, 									//11022 Ԥ��λ�޿�
	ERROR_PRETARGETING_FROM_COOL_NONE_CARD, 						//11023 Ԥ��λ����ѹλ�޿�
	ERROR_WARM_FROM_COOL_NONE_CARD, 								//11024 ��ѹλ����ѹλ�޿�
	ERROR_COOL_FROM_PRETARGETING_NONE_CARD, 						//11025 ��ѹλ��Ԥ��λ�޿�
	ERROR_PRETARGETING_FROM_WASTE_NONE_CARD, 						//11026 Ԥ��λ������λ�޿�
	ERROR_LIFT_CAR_LEAVE_ORIGIN_FAIL ,   							//11027 ����С���뿪ԭ��ʧ��
	ERROR_LIFT_CAR_RETURN_ORIGIN_FAIL, 		    					//11028 ����С������ԭ��ʧ��
	ERROR_LIFT_CAR_POSITION_PARA_MAX,   							//11029 ����С���ϲ㴫������ݳ����������ֵ
	ERROR_LIFT_CAR_LEAVE_HORIZON_ORIGIN_FAIL,	 					//11030 ����С���뿪ˮƽԭ��ʧ��
	ERROR_LIFT_CAR_RETURN_HORIZON_ORIGIN_FAIL,						//11031 ����С����ˮƽԭ��ʧ��
	ERROR_LIFT_CAR_HORIZON_POSITION_PARA_MAX, 						//11032 ����ˮƽ�ƶ����ղ�������
	ERROR_RWCHANNEL_HAVE_CARD_NOT_ENTER_CARD,	 					//11033 ͨ���п����ܽ���
	ERROR_RWCHANNEL_ENTER_CARD_FAIL,								//11034 ��дģ��û�п�Ƭ����
	ERROR_RWCHANNEL_OUT_CARD_TO_LIFT_NO_CARD,						//11035	��дͨ�������������޿�
	ERROR_RWCHANNEL_OUT_CARD_TO_LIFT_FAIL, 							//11036 ��дͨ������������ʧ��;
	ERROR_RECEIVE_CARD_POSITION_NOT_CARD,							//11037 ���ݽӿ�λû��������
	ERROR_OCR_TO_RWCHANNEL_CARD_SLOT_NOT_CARD, 						//11038 OCR����дͨ�������޿�
	ERROR_RWCHANNEL_EXIT_CARD_NOT_CARD, 							//11039 ��дͨ�������޿�
	ERROR_CARD_NOT_MOV_TO_PERFECT_CARD_POSITON, 					//11040 ��Ƭδ�ƶ����ÿ�λ
	ERROR_NONE_CARD_NOT_START_ENTER_CARD_ACTION,					//11041 �޿����ܽ����տ�����
	ERROR_PERFECT_CARD_ENTER_FAIL,									//11042 �ÿ�����ʧ��
	ERROR_PERFECT_CARD_ENTER_TO_EXIT_CARD_SENSOR_FAIL,				//11043 �ÿ�������������λ��ʧ��
	ERROR_LIFT_SLOT_TO_RWCH_SD_CARD_DETEC_HAV_CARD,					//11044 ���ݲ�����дͨ���Ϳ�ʱ��⵽��дͨ���Ѿ��п�
	ERROR_LIFT_SLOT_NO_CARD_NO_SEND_CARD_TO_RWCH,					//11045 ���ݲ��޿�,�޷��Ϳ�����дͨ��
	ERROR_LIFT_SLOT_SEND_CARD_TO_RWCH_RETRY_FAIL,					//11046 ���ݿ����Ϳ�����дͨ���������ʧ��ʧ��
	ERROR_RWCHANNEL_ENTER_CARD_TO_EXIT_CARD_SENSOR_FAIL,    		//11047 ��дͨ������������������λ��ʧ��
	ERROR_LIFT_CARD_SLOT_HAVE_CARD_RWCHANNEL_NOT_SEND_CARD, 		//11048 ���ݿ����п�,��дͨ���޷��Ϳ������ݿ���
	ERROR_RWCHANNEL_NO_CARD_NO_EXIT_CARD_TO_LIFT_SLOT,				//11049 ��дͨ���޿����޷����������ݿ���
	ERROR_RWCHANNEL_EXIT_CARD_TO_LIFT_CARD_SLOT_FAIL,				//11050 ��дͨ�����������ݿ���ʧ��
	ERROR_LIFT_CARD_SLOT_FROM_RWCHANNEL_PULL_CARD_FAIL,				//11051 ���ݿ��۴Ӷ�д��ͨ���ο�ʧ��
	ERROR_LIFT_CARD_NO_CARD_NO_EXIT_CARD_TO_RECYCLE_BOX,			//11052 ���ݿ����޿�,�޷�������������
	ERROR_LIFT_CARD_SLOT_SEND_CARD_TO_RECYCLE_CHANNEL_FAIL, 		//11053 ���ݿ����Ϳ�������ͨ�����ʧ��
	ERROR_LIFT_CARD_SLOT_TO_RECYCLE_BOX_FAIL,						//11054 ���ݿ����Ϳ���������ʧ��
	ERROR_RWCHANNEL_HAVE_MULTI_CARD,					    		//11055 ��дͨ���ж��ſ�,���ߴ�������
	ERROR_RWCHANNEL_SEND_CARD_EIXT_CARD_FAIL,						//11056 ��дͨ���Ϳ��������ڴ�����X128λ��ʧ��
	ERROR_RWCHANNEL_ENTER_CARD_SENSOR_TO_EXIT_CARD_SENSOR_FAIL,		//11057 ��дͨ������������X124λ������������X128λʧ��
	ERROR_RWCHANNLE_EXIT_CARD_SENSOR_TO_ENTER_CARD_SENSOR_FAIL, 	//11058 ��дͨ������������X128λ������������X124λʧ��
	ERROR_CARD_SLOT_AND_LIFT_RECEIVE_ALL_HAVE_CARD,					//11059 �¿��ư帴λʱ��⵽���ݿ�������ݽӿ����̴����п�

}ModulesErrorCode;

enum ENUM_CLAMPCARDCAR_LOCATION //ץ��С��λ��
{
	ENUM_UNKNOW,	    	  	//0λ��δ֪		   
    ENUM_INSIDE_WARE,		  	//1С������ѹ��λ	   
    ENUM_INSIDE_COOL,	      	//2С������ѹ��λ	     
	ENUM_INSIDE_PRETARGETING, 	//3С����Ԥ��λ��λ   
	ENUM_INSIDE_CUT_CARD,     	//4С���ڼ�����λ 	
	ENUM_INSIDE_WASTE,	  	  	//5С���ڷ��Ϲ�λ	    
    ENUM_INSIDE_ORIGIN,	   	  	//6С����ԭ��		   	
};
enum ENUM_LIFT_UP_DN_LOCATION 	//����С��λ��
{
	ENUM_LIFT_UP_DN_UNKNOW,	  			//0λ��δ֪	
    ENUM_LIFT_SUCK_RECEIVE_CARD,		//1�ӿ�λ  			   		   
    ENUM_LIFT_SUCK_TO_RW_CHANNEL,		//2������������дͨ��	  
    ENUM_LIFT_SLOT_GET_RW_CHANNEL,		//3���ݲ۶Զ�дͨ��
	ENUM_LIFT_SLOT_TO_RECYCLE_BOX, 		//4���ݲ۶Ի�����    	
	ENUM_LIFT_SLOT_TO_OCR,	     	  	//5���ݲ۵�OCR	
   	ENUM_LIFT_UP_DN_ORG,	  			//6����ԭ��
};
enum ENUM_LIFT_HORIZON_LOCATION 		//����С��ˮƽλ��
{
	ENUM_LIFT_HORIZON_UNKNOW,  			//0λ��δ֪	
    ENUM_LIFT_HORIZON_REC_AND_SEND,	    //1����ˮƽ���Ϳ�λ 			   		   
    ENUM_LIFT_HORIZON_OUT_TO_OCR,		//2����ˮƽ������OCR	  
    ENUM_LIFT_HORIZON_GET_FROM_OCR,		//3OCR����������ˮƽ
   	ENUM_LIFT_HORIZON_ORG,  			//4����С��ˮƽԭ��
};

enum ENUM_STATION_HAVE_CARD
{
	STATION_HAS_CARD_NO = 0,
	STATION_HAS_CARD_YES,
	STATION_HAS_CARD_UNKNOW,
};

//��λ���޿�״̬����
enum ENUM_STATION_HAVE_CARD_INDEX
{
	STATION_LOADING_PLATFORM_HAS_CARD= 0,			//1Ԥ��λ���޿�
//	STATION_HOT_PRESS_OUT_SENSOR_HAS_CARD,			//����ѹ����������п�
//	STATION_COLD_PRESS_OUT_SENSOR_HAS_CARD,			//����ѹ����������п�
	STATION_LIFT_SUCK_HAS_CARD,						//2�������̴���ס�˿�
	STATION_LIFT_MID_HAS_CARD,						//3�����п�
	STATION_OUT_PORT_HAS_CARD,						//4����ͨ���п�
	STATION_RECYCLE_BOX_FULL,						//5��������
	STATION_RECYCLE_BOX_IN_PLACE,					//6�����䵽λ
	STATION_RECYCLE_BOX_HAS_CARD,					//7�������п�
	STATION_WASTER_BOX_FULL,						//8��������
	STATION_WASTER_BOX_IN_PLACE,					//9�����䵽λ

	//...............
	MAX_STATION_HAS_CARD_NUM = 10,	   				//ָ�ͨѶ�ӿڹ̶�Ϊ20���ֽڵ�״̬
};

void InitializeMachine(void);
void ResetAllSignal(void);
void StepCtrlMachine(void); 
void UpdateStatus(void);
// ��������������
extern unsigned char ParamBuf[];
extern unsigned char gnOperationNo;
extern void ProcessUpdateParameter(void); //���¹�λ��λ����
extern unsigned char CardStatus[10];

#endif
