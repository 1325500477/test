#include "parameter.h"
#include "process.h"

signed int parameter[MAX_PARAMETER_NUM];

//第一次使用控制板，自动初始化相关参数，由底层人员根据调试经验设置
void DefaultParameter(void)
{
    parameter[PAR_WARM_STEP] 		           = 140;    //1到热压工位补偿
    parameter[PAR_COOL_STEP] 		           = 1830;   //2到冷压工位补偿  
    parameter[PAR_WASTE]                       = 3500;   //3到废料位
    parameter[PAR_PRETARGETING]                = 4800;   //4到预定位
    parameter[PAR_CUT_CARD_POSITION]           = 6200;   //5到剪卡位
    parameter[PAR_BACK_STEP] 		           = 25;     //6预留
    parameter[PAR_SM_CHANNAL_SPEED]            = 150;    //7小车速度
    parameter[PAR_PRETARGET_UP_POSITION]       = 600;    //8预定位上升步数
    parameter[PAR_WASTESTORAGE_DN_POSTION] 	   = 1100;   //9废料场下降步数
    parameter[PAR_WARM_TIME]                   = 100; 	 //10加热时间s
    parameter[PAR_COOL_TIME]                   = 40;     //11冷却时间s
    parameter[PAR_SUCK_CARD_CAR_ACCESSORY]     = 12;     //12 吸卡小车辅料位
    parameter[PAR_SUCK_CARD_CAR_PRETARGETING]  = 13;     //13 吸卡小车预定位
    parameter[PAR_SUCK_CARD_CAR_TEMPORARY]     = 14;     //14 吸卡小车暂放卡位
    parameter[PAR_SUCK_CARD_CAR_NEW_ACCESSORY] = 15;     //15 吸卡小车新辅料位
    parameter[PAR_SUCK_CARD_CAR_PRINT]         = 16;     //16 吸卡小车打印位
    parameter[PAR_SUCK_CARD_CAR_BLANK]         = 17;     //17 吸卡小车空白卡位
    parameter[PAR_SUCK_ACCESSORY]              = 18;     //18 吸盘辅料位  
    parameter[PAR_SUCK_PRETARGETING]           = 19;     //19 吸盘预定位
    parameter[PAR_SUCK_TEMPORARY]              = 20;     //20 吸盘暂放卡位
    parameter[PAR_SUCK_NEW_ACCESSORY]          = 21;     //21 吸盘新辅料位
    parameter[PAR_SUCK_PRINT]                  = 22;     //22 吸盘打印位
    parameter[PAR_SUCK_BLANK]                  = 23;     //23 吸盘空白卡位
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
    ProcessUpdateParameter(); //更新工位定位参数
}

