#include "parameter.h"
#include "process.h"

signed int parameter[MAX_PARAMETER_NUM];

//��һ��ʹ�ÿ��ư壬�Զ���ʼ����ز������ɵײ���Ա���ݵ��Ծ�������
void DefaultParameter(void)
{
    parameter[PAR_WARM_STEP] 		           = 140;    //1����ѹ��λ����
    parameter[PAR_COOL_STEP] 		           = 1830;   //2����ѹ��λ����  
    parameter[PAR_WASTE]                       = 3500;   //3������λ
    parameter[PAR_PRETARGETING]                = 4800;   //4��Ԥ��λ
    parameter[PAR_CUT_CARD_POSITION]           = 6200;   //5������λ
    parameter[PAR_BACK_STEP] 		           = 25;     //6Ԥ��
    parameter[PAR_SM_CHANNAL_SPEED]            = 150;    //7С���ٶ�
    parameter[PAR_PRETARGET_UP_POSITION]       = 600;    //8Ԥ��λ��������
    parameter[PAR_WASTESTORAGE_DN_POSTION] 	   = 1100;   //9���ϳ��½�����
    parameter[PAR_WARM_TIME]                   = 100; 	 //10����ʱ��s
    parameter[PAR_COOL_TIME]                   = 40;     //11��ȴʱ��s
    parameter[PAR_SUCK_CARD_CAR_ACCESSORY]     = 12;     //12 ����С������λ
    parameter[PAR_SUCK_CARD_CAR_PRETARGETING]  = 13;     //13 ����С��Ԥ��λ
    parameter[PAR_SUCK_CARD_CAR_TEMPORARY]     = 14;     //14 ����С���ݷſ�λ
    parameter[PAR_SUCK_CARD_CAR_NEW_ACCESSORY] = 15;     //15 ����С���¸���λ
    parameter[PAR_SUCK_CARD_CAR_PRINT]         = 16;     //16 ����С����ӡλ
    parameter[PAR_SUCK_CARD_CAR_BLANK]         = 17;     //17 ����С���հ׿�λ
    parameter[PAR_SUCK_ACCESSORY]              = 18;     //18 ���̸���λ  
    parameter[PAR_SUCK_PRETARGETING]           = 19;     //19 ����Ԥ��λ
    parameter[PAR_SUCK_TEMPORARY]              = 20;     //20 �����ݷſ�λ
    parameter[PAR_SUCK_NEW_ACCESSORY]          = 21;     //21 �����¸���λ
    parameter[PAR_SUCK_PRINT]                  = 22;     //22 ���̴�ӡλ
    parameter[PAR_SUCK_BLANK]                  = 23;     //23 ���̿հ׿�λ
    parameter[PAR_BAK24] = 24;
    parameter[PAR_BAK25] = 25;
    parameter[PAR_BAK26] = 26;
    parameter[PAR_BAK27] = 27;
    parameter[PAR_BAK28] = 28;
    parameter[PAR_BAK29] = 29;
    parameter[PAR_BAK30] = 30;
    
    parameter[PAR_BAK31] = 31;
    parameter[PAR_BAK32] = 32;
    parameter[PAR_BAK33] = 33;
    parameter[PAR_BAK34] = 34;
    parameter[PAR_BAK35] = 35;
    parameter[PAR_BAK36] = 36;
    parameter[PAR_BAK37] = 37;
    parameter[PAR_BAK38] = 38;
    parameter[PAR_BAK39] = 39;
    parameter[PAR_BAK40] = 40;
    
    parameter[PAR_BAK41] = 41;
    parameter[PAR_BAK42] = 42;
    parameter[PAR_BAK43] = 43;
    parameter[PAR_BAK44] = 44;
    parameter[PAR_BAK45] = 45;
    parameter[PAR_BAK46] = 46;
    parameter[PAR_BAK47] = 47;
    parameter[PAR_BAK48] = 48;   
    ProcessUpdateParameter(); //���¹�λ��λ����
}

