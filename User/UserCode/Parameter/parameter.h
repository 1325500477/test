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
    PAR_WASTE,                              //3 �����Ϲ�λ
    PAR_PRETARGETING,                       //4 ��Ԥ��λ
    PAR_CUT_CARD_POSITION,                  //5 �ü���λ��
    PAR_BACK_STEP,							//6 Ԥ��
    PAR_SM_CHANNAL_SPEED,                   //7 С���ٶ�
    PAR_PRETARGET_UP_POSITION,              //8 Ԥ��λ�����������
    PAR_WASTESTORAGE_DN_POSTION,            //9 ���ϳ�����½�����
    PAR_WARM_TIME,							//10 ��ѹʱ�� 
    PAR_COOL_TIME,							//11 ��ѹʱ��
    PAR_SUCK_CARD_CAR_ACCESSORY,            //12 ����С������λ
    PAR_SUCK_CARD_CAR_PRETARGETING,		    //13 ����С��Ԥ��λ
    PAR_SUCK_CARD_CAR_TEMPORARY,			//14 ����С���ݷſ�λ
    PAR_SUCK_CARD_CAR_NEW_ACCESSORY,		//15 ����С���¸���λ
    PAR_SUCK_CARD_CAR_PRINT,				//16 ����С����ӡλ
    PAR_SUCK_CARD_CAR_BLANK,				//17 ����С���հ׿�λ
    PAR_SUCK_ACCESSORY,			            //18 ���̸���λ  
    PAR_SUCK_PRETARGETING,				    //19 ����Ԥ��λ
    PAR_SUCK_TEMPORARY,				        //20 �����ݷſ�λ
    PAR_SUCK_NEW_ACCESSORY,				    //21 �����¸���λ
    PAR_SUCK_PRINT,				            //22 ���̴�ӡλ
    PAR_SUCK_BLANK,				            //23 ���̿հ׿�λ
    PAR_BAK24,				//24
    PAR_BAK25,				//25
    PAR_BAK26,				//26
    PAR_BAK27,				//27
    PAR_BAK28,			    //28
    PAR_BAK29,				//29
    PAR_BAK30,				//30
    
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

extern int parameter[];
extern unsigned int txlen;
extern unsigned char comm_send_buf[];
extern unsigned char comm_recv_buf[];

void DefaultParameter(void);

#ifdef __cplusplus
}
#endif

#endif
