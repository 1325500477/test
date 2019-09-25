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
    PAR_WARM_STEP,							//1 到热压工位
    PAR_COOL_STEP,							//2 到冷压工位   
    PAR_PRETARGETING,                       //3 到预定工位
    PAR_CUT_CARD_POSITION,                  //4 裁剪卡位置
    PAR_WASTE,                              //5 到废料工位
    PAR_SM_CHANNAL_SPEED,                   //6 抓卡小车速度
    PAR_PRETARGET_UP_POSITION,              //7 预定位电机上升步数
    PAR_WASTESTORAGE_DN_POSTION,            //8 废料场电机下降步数
    PAR_WARM_TIME,							//9 热压时间 
    PAR_COOL_TIME,							//10 冷压时间
    PAR_LIFT_UP_DN_SPEED,					//11 电梯小车的速度s
    PAR_RECEIVE_CARD,                       //12 电梯小车接卡位    
    PAR_ELEVATOR_TO_RWMODULE,               //13 电梯到读写模块
    PAR_RWMODULE_TO_ELEVATOR,				//14 读写模块到电梯
    PAR_WASTE_CARD,                         //15 电梯小车废卡位
    PAR_OCR_CARD,	        				//16 电梯小车OCR位   
    PAR_CONNECTION_NORMA,			        //17 衔接模块正常位置 
    PAR_RWMODULE_SPEED,				        //18 读写模块速度
    PAR_RWMODULE_OUT_CARD,				    //19 读写模块出卡步数
    PAR_OUT_CARD,           				//20 读写模块出好卡的步数
    PAR_CONNECTION_ENTER_OCR,				//21 衔接模块进卡到OCR
    PAR_CONNECTION_EXIT_OCR,			    //22 衔接模块出卡到OCR
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
