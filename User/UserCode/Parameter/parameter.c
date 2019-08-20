#include "parameter.h"
#include "process.h"

signed short parameter[MAX_PARAMETER_NUM];

//第一次使用控制板，自动初始化相关参数，由底层人员根据调试经验设置
void DefaultParameter(void)
{
    parameter[PAR_WARM_STEP] 		           = 13;    //1 到热压工位补偿
    parameter[PAR_COOL_STEP] 		           = 165;   //2 到冷压工位补偿   
    parameter[PAR_PRETARGETING]                = 311;   //3 到预定位
    parameter[PAR_CUT_CARD_POSITION]           = 448;   //4 到剪卡位
    parameter[PAR_WASTE]                       = 580;   //5 到废料位
    parameter[PAR_SM_CHANNAL_SPEED]            = 200;    //6 小车速度
    parameter[PAR_PRETARGET_UP_POSITION]       = 600;    //7 预定位上升步数
    parameter[PAR_WASTESTORAGE_DN_POSTION] 	   = 600;    //8 废料场下降步数
    parameter[PAR_WARM_TIME]                   = 100; 	 //9 加热时间s
    parameter[PAR_COOL_TIME]                   = 40;     //10 冷却时间s
    parameter[PAR_ELEVATOR_CAR_SPEED] 		   = 100;    //11 电梯小车速度
    parameter[PAR_RECEIVE_CARD]                = 250;   //12 电梯小车接卡位
    parameter[PAR_ELEVATOR_TO_RWMODULE]        = 42;    //13 电梯到读写模块
    parameter[PAR_RWMODULE_TO_ELEVATOR] 	   = 195;   //14 读写模块到电梯 
    parameter[PAR_WASTE_CARD]                  = 177;   //15 电梯小车废卡位
    parameter[PAR_OCR_CARD] 		           = 13;    //16 电梯小车OCR检测位
    parameter[PAR_CONNECTION_TO_SEND_CARD] 	   = 330;   //17 衔接模块到发卡位 
    parameter[PAR_RWMODULE_SPEED]              = 100;   //18 读写模块的速度
    parameter[PAR_RWMODULE_OUT_CARD]           = 1450;  //19 读写模块出卡步数
    parameter[PAR_OUT_CARD]                    = 1000; 	//20 读写模块出好卡的步数 
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
    ProcessUpdateParameter(); //更新工位定位参数
}

