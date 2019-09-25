//Tab size: 4;	Indent size: 4
#ifndef __PARAMETER_H__
#define __PARAMETER_H__

#ifdef __cplusplus
extern "C"
{
#endif

#define true 1
#define false 0


enum ENUM_PARAMETER
{
    PAR_WARM_STEP,							//1 ����ѹ��λ
    PAR_COOL_STEP,							//2 ����ѹ��λ   
    PAR_PRETARGETING,                       //3 ��Ԥ����λ
    PAR_CUT_CARD_POSITION,                  //4 �ü���λ��
    PAR_WASTE,                              //5 �����Ϲ�λ
    PAR_SM_CHANNAL_SPEED,                   //6 ץ��С���ٶ�
    PAR_PRETARGET_UP_POSITION,              //7 Ԥ��λ�����������
    PAR_WASTESTORAGE_DN_POSTION,            //8 ���ϳ�����½�����
    PAR_WARM_TIME,							//9 ��ѹʱ�� 
    PAR_COOL_TIME,							//10 ��ѹʱ��
    PAR_LIFT_UP_DN_SPEED,					//11 ����С�����ٶ�s
    PAR_RECEIVE_CARD,                       //12 ����С���ӿ�λ    
    PAR_ELEVATOR_TO_RWMODULE,               //13 ���ݵ���дģ��
    PAR_RWMODULE_TO_ELEVATOR,				//14 ��дģ�鵽����
    PAR_WASTE_CARD,                         //15 ����С���Ͽ�λ
    PAR_OCR_CARD,	        				//16 ����С��OCRλ   
    PAR_CONNECTION_NORMA,			        //17 �ν�ģ������λ�� 
    PAR_RWMODULE_SPEED,				        //18 ��дģ���ٶ�
    PAR_RWMODULE_OUT_CARD,				    //19 ��дģ���������
    PAR_OUT_CARD,           				//20 ��дģ����ÿ��Ĳ���
    PAR_CONNECTION_ENTER_OCR,				//21 �ν�ģ�������OCR
    PAR_CONNECTION_EXIT_OCR,			    //22 �ν�ģ�������OCR
    PAR_BAK23,				//23
    PAR_BAK24,			    //24
    PAR_BAK25,				//25
    PAR_BAK26,				//26
    PAR_BAK27,				//27
    PAR_BAK28,				//28
    PAR_BAK29,				//29
    PAR_BAK30,			    //30

    PAR_BAK31,				//31
    PAR_BAK32,				//32
    PAR_BAK33,				//33
    PAR_BAK34,				//34
    PAR_BAK35,				//35
    PAR_BAK36,				//36
    PAR_BAK37,				//37
    PAR_BAK38,			    //38
    PAR_BAK39,				//39
    PAR_BAK40,				//40
    
    PAR_BAK41,				//41
    PAR_BAK42,				//42
    PAR_BAK43,				//43
    PAR_BAK44,				//44
    PAR_BAK45,				//45
    PAR_BAK46,				//46
    PAR_BAK47,				//47
    PAR_BAK48,			    //48

    MAX_PARAMETER_NUM,
};

extern short parameter[];
extern unsigned int txlen;
extern unsigned char comm_send_buf[];
extern unsigned char comm_recv_buf[];

void DefaultParameter(void);

#ifdef __cplusplus
}
#endif

#endif
