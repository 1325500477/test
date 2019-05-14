
#include "parameter.h"


signed int parameter[MAX_PARAMETER_NUM];

//第一次使用控制板，自动初始化相关参数，由底层人员根据调试经验设置
void DefaultParameter(void)
{
    parameter[SM_CHANNAL_SPEED]  = 200;  		   //1通道电机速度
    parameter[WARM_TIME] 		 = 100;              //2加热时间
    parameter[COOL_TIME] 		 = 20;              //3冷却时间
    parameter[WARM_STEP] 		 = 250;             //4到热压工位补偿
    parameter[COOL_STEP] 		 = 1400;            //5到冷压工位补偿    
    parameter[BACK_STEP] 		 = 2500;           //6返回原点位补偿 
    parameter[CUT_CARD_POSITION] = 800;            //7到裁剪卡的位置
    
    parameter[PAR_BAK8]  = 11;
    parameter[PAR_BAK9]  = 12;
    parameter[PAR_BAK10] = 13; 
    parameter[PAR_BAK11] = 11;
    parameter[PAR_BAK12] = 12;
    parameter[PAR_BAK13] = 13;
    parameter[PAR_BAK14] = 14;
    parameter[PAR_BAK15] = 15;
    parameter[PAR_BAK16] = 16;
    parameter[PAR_BAK17] = 17;
    parameter[PAR_BAK18] = 18;
    parameter[PAR_BAK19] = 19;
    parameter[PAR_BAK20] = 20;
    
    parameter[PAR_BAK21] = 21;
    parameter[PAR_BAK22] = 22;
    parameter[PAR_BAK23] = 23;
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

}

