#include "parameter.h"
#include "process.h"

signed short parameter[MAX_PARAMETER_NUM];

//��һ��ʹ�ÿ��ư壬�Զ���ʼ����ز������ɵײ���Ա���ݵ��Ծ�������
void DefaultParameter(void)
{
    parameter[PAR_WARM_STEP] 		           = 13;    //1 ����ѹ��λ����
    parameter[PAR_COOL_STEP] 		           = 165;   //2 ����ѹ��λ����   
    parameter[PAR_PRETARGETING]                = 311;   //3 ��Ԥ��λ
    parameter[PAR_CUT_CARD_POSITION]           = 449;   //4 ������λ
    parameter[PAR_WASTE]                       = 580;   //5 ������λ
    parameter[PAR_SM_CHANNAL_SPEED]            = 350;   //6 С���ٶ�
    parameter[PAR_PRETARGET_UP_POSITION]       = 660;   //7 Ԥ��λ��������
    parameter[PAR_WASTESTORAGE_DN_POSTION] 	   = 600;   //8 ���ϳ��½�����
    parameter[PAR_WARM_TIME]                   = 100; 	//9 ����ʱ��s
    parameter[PAR_COOL_TIME]                   = 40;    //10 ��ȴʱ��s
    parameter[PAR_LIFT_UP_DN_SPEED] 		   = 250;   //11 ����С���ٶ�
    parameter[PAR_RECEIVE_CARD]                = 245;   //12 ����С���ӿ�λ
    parameter[PAR_ELEVATOR_TO_RWMODULE]        = 41;    //13 ������������дͨ��	
    parameter[PAR_RWMODULE_TO_ELEVATOR] 	   = 195;   //14 ���ݲ۶Զ�дͨ��
    parameter[PAR_WASTE_CARD]                  = 176;   //15 ���ݲ۶Ի����� 
    parameter[PAR_OCR_CARD] 		           = 12;    //16 ���ݲ۵�OCR
    parameter[PAR_CONNECTION_NORMA] 	       = 470;   //17 ���Ϳ�λ  
    parameter[PAR_RWMODULE_SPEED]              = 100;   //18 ��дģ����ٶ�
    parameter[PAR_RWMODULE_OUT_CARD]           = 1450;  //19 ��дģ���������
    parameter[PAR_OUT_CARD]                    = 2200; 	//20 ��дģ����ÿ��Ĳ��� 
    parameter[PAR_CONNECTION_ENTER_OCR]        = 600;   //21 ����ˮƽ������OCR
    parameter[PAR_CONNECTION_EXIT_OCR]         = 280;   //22 OCR����������ˮƽ
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

