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
    SM_CHANNAL_SPEED,                   //1通道电机速度
    WARM_TIME,                          //2加热时间
    COOL_TIME,                          //3冷却时间
    WARM_STEP,							//4到热压工位补偿
    COOL_STEP,							//5到冷压工位补偿   
    BACK_STEP,							//6返回原点位补偿 
    CUT_CARD_POSITION,                  //7裁剪卡位置补偿
    
	PAR_BAK8,				//8
    PAR_BAK9,				//9
    PAR_BAK10,				//10 
    PAR_BAK11,				//11
    PAR_BAK12,				//12
    PAR_BAK13,				//13
    PAR_BAK14,				//14
    PAR_BAK15,				//15
    PAR_BAK16,				//16
    PAR_BAK17,				//17
    PAR_BAK18,			    //18
    PAR_BAK19,				//19
    PAR_BAK20,				//20
    
    PAR_BAK21,				//21
    PAR_BAK22,				//22
    PAR_BAK23,				//23
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

enum ENUM_LOCATION
{
    ENUM_OUTSIDE = 1,	//夹卡片的夹子在外面
    ENUM_INSIDE_WARE,	//夹卡片的夹子在里面热压工位
    ENUM_INSIDE_COOL,	//夹卡片的夹子在里面冷压工位

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
