#include "parameter.h"
#include "process.h"

signed short parameter[MAX_PARAMETER_NUM];

//��һ��ʹ�ÿ��ư壬�Զ���ʼ����ز������ɵײ���Ա���ݵ��Ծ�������
void DefaultParameter(void)
{
    parameter[PAR_WARM_STEP] 		           = 13;    //1 ����ѹ��λ����
    parameter[PAR_COOL_STEP] 		           = 165;   //2 ����ѹ��λ����   
    parameter[PAR_PRETARGETING]                = 311;   //3 ��Ԥ��λ
    parameter[PAR_CUT_CARD_POSITION]           = 448;   //4 ������λ
    parameter[PAR_WASTE]                       = 580;   //5 ������λ
    parameter[PAR_SM_CHANNAL_SPEED]            = 200;    //6 С���ٶ�
    parameter[PAR_PRETARGET_UP_POSITION]       = 600;    //7 Ԥ��λ��������
    parameter[PAR_WASTESTORAGE_DN_POSTION] 	   = 600;    //8 ���ϳ��½�����
    parameter[PAR_WARM_TIME]                   = 100; 	 //9 ����ʱ��s
    parameter[PAR_COOL_TIME]                   = 40;     //10 ��ȴʱ��s
    parameter[PAR_ELEVATOR_CAR_SPEED] 		   = 100;    //11 ����С���ٶ�
    parameter[PAR_RECEIVE_CARD]                = 250;   //12 ����С���ӿ�λ
    parameter[PAR_ELEVATOR_TO_RWMODULE]        = 42;    //13 ���ݵ���дģ��
    parameter[PAR_RWMODULE_TO_ELEVATOR] 	   = 195;   //14 ��дģ�鵽���� 
    parameter[PAR_WASTE_CARD]                  = 177;   //15 ����С���Ͽ�λ
    parameter[PAR_OCR_CARD] 		           = 13;    //16 ����С��OCR���λ
    parameter[PAR_CONNECTION_TO_SEND_CARD] 	   = 330;   //17 �ν�ģ�鵽����λ 
    parameter[PAR_RWMODULE_SPEED]              = 100;   //18 ��дģ����ٶ�
    parameter[PAR_RWMODULE_OUT_CARD]           = 1450;  //19 ��дģ���������
    parameter[PAR_OUT_CARD]                    = 1000; 	//20 ��дģ����ÿ��Ĳ��� 
    parameter[PAR_BAK21] = 820;    //21 
    parameter[PAR_BAK22] = 1530;   //22 
    parameter[PAR_BAK23] = 270;    //23             
    parameter[PAR_BAK24] = 500;    //24 
    parameter[PAR_BAK25] = 1000;   //25 
    parameter[PAR_BAK26] = 80;     //26
    parameter[PAR_BAK27] = 1530;   //27 
    parameter[PAR_BAK28] = 270;    //28             
    parameter[PAR_BAK29] = 500;    //29 
    parameter[PAR_BAK30] = 1000;   //30 

    parameter[PAR_BAK31] = 80;     //31
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

