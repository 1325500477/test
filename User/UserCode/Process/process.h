#ifndef __PROCESS_H__
#define __PROCESS_H__

/*************状态定义**************/
#define STATUS_UNKNOW       	   0 //未知状态
#define STATUS_ORG				   1 //在原点
#define STATUS_POSITION			   2 //运行位
#define STATUS_RIGHT_CLAMP_LOOSE   1 //右边夹子打开状态
#define STATUS_RIGHT_CLAMP_CLOSE   2 //右边夹子关闭状态
#define STATUS_LIFT_CLAMP_LOOSE    1 //左边夹子打开状态
#define STATUS_LIFT_CLAMP_CLOSE    2 //左边夹子关闭状态

/*************输入端口定义**************/

typedef enum
{
	ERROR_CHANNAL_LEAVE_ORG_FAIL  = 10001,  		//10001 抓卡小车离开原点失败
	ERROR_CHANNAL_RETURN_ORG_FAIL,  				//10002 抓卡小车返回原点失败
	ERROR_WARMING_DOWN_FAIL,      					//10003 加热下降失败
	ERROR_WARMING_RISE_FAIL,     		    		//10004 加热升起失败
	ERROR_COOLING_DOWN_FAIL,      					//10005 冷却下降失败
	ERROR_COOLING_RISE_FAIL,      					//10006 冷却升起失败
	ERROR_PRESS_DOWN_FAIL,      					//10007 冲压下降失败
	ERROR_PRESS_RISE_FAIL,      					//10008 冲压升起失败
	ERROR_RIGHT_CLAMP_CLOSE_FAIL,    				//10009 右边夹子关闭失败
	ERROR_RIGHT_CLAMP_OPEN_FAIL,     				//10010 右边夹子打开失败
	ERROR_LEFT_CLAMP_CLOSE_FAIL,			    	//10011 左边夹子关闭失败
	ERROR_LEFT_CLAMP_OPEN_FAIL,     				//10012 左边夹子打开失败
	ERROR_TROLLEY_CAR_POSITION_PARA_MAX,			//10013 抓卡小车上层传输的数据超出接收最大值
	ERROR_PRETARGETING_RETURN_ORIGIN_FAIL,			//10014 预定位返回原点失败
	ERROR_PRETARGETING_LEAVE_ORIGIN_FAIL,			//10015 预定位离开原点失败
	ERROR_WASTESTORAGE_RETURN_ORIGIN_FAIL,			//10016 废卡位返回原点失败
	ERROR_WASTESTORAGE_LEAVE_ORIGIN_FAIL,			//10017 废卡位离开原点失败	
	//吸卡小车
	ERROR_SUCK_CARD_CAR_POSITION_PARA_MAX,          //10018 吸卡小车上层传输的数据超出接收最大值
	ERROR_SUCK_CARD_CAR_LEAVE_ORIGIN_FAIL,          //10019 吸卡小车离开原点失败
	ERROR_SUCK_CARD_CAR_RETURN_ORIGIN_FAIL,         //10020 吸卡小车返回原点失败
	ERROR_SUCK_RETURN_ORIGIN_FAIL,         			//10021 吸盘返回原点失败
	ERROR_SUCK_LEAVE_ORIGIN_FAIL,         			//10022 吸盘离开原点失败
	//吸盘
	ERROR_SUCK_POSITION_PARA_MAX,					//10023 吸盘上层传输的数据超出接收最大值
	//白卡箱
	ERROR_BLANK_CARD_BOX_RESET_FAIL,				//10024 白卡箱复位失败
	ERROR_BLANK_CARD_BOX_LEAVE_ORIGIN_FAIL,			//10025 白卡箱离开原点失败
	ERROR_BLANK_CARD_BOX_EMPTY,						//10026 白卡箱空
	ERROR_SUCK_VACUUM_CARD_FAIL,					//10027 吸盘真空吸卡失败
	ERROR_PRETARGETING_NONE_CARD, 					//10028 预定位无卡
	ERROR_PRETARGETING_FROM_COOL_NONE_CARD, 		//10029 预定位到冷压位无卡
	ERROR_WARM_FROM_COOL_NONE_CARD, 				//10030 热压位到冷压位无卡
	ERROR_COOL_FROM_PRETARGETING_NONE_CARD, 		//10031 冷压位到预定位无卡
	ERROR_PRETARGETING_FROM_WASTE_NONE_CARD, 		//10032 预定位到废料位无卡
	ERROR_WASTE_FROM_PRESS_NONE_CARD, 				//10033 废料位到剪卡位无卡
	ERROR_SUCK_FREE_CARD_FAIL, 						//10034 吸盘释放卡失败
	ERROR_PRETARGETING_SUCK_VACUUM_CARD_FAIL,		//10035 预定位吸卡失败
	ERROR_ACCESSORY_SUCK_FREE_CARD_FAIL,			//10036 辅料位吸盘释放卡失败
	ERROR_RETURN_CARD_SUCK_CARD_FAIL,				//10037 吸盘还卡吸卡失败
	ERROR_RETURN_CARD_SUCK_FREE_CARD_FAIL,			//10038 吸盘还卡释放卡失败
	ERROR_SUCK_CARD_DROP_OUT,						//10039 吸盘运行中卡片掉落

}ModulesErrorCode;

enum ENUM_CLAMPCARDCAR_LOCATION //抓卡小车位置
{
	ENUM_UNKNOW,	    	  //位置未知		   //0
    ENUM_INSIDE_WARE,		  //小车在热压工位	   //1
    ENUM_INSIDE_COOL,	      //小车在冷压工位	   //2
	ENUM_INSIDE_WASTE,	  	  //小车在废料工位	   //3
	ENUM_INSIDE_PRETARGETING, //小车在预定位工位   //4
	ENUM_INSIDE_CUT_CARD,     //小车在剪卡工位 	   //5
    ENUM_INSIDE_ORIGIN,	   	  //小车在原点		   //6	
};
enum ENUM_SUCK_CARD_CAR_LOCATION //吸卡小车位置
{
	ENUM_SUCK_UNKNOW,	      //位置未知		   		   //0
    ENUM_ACCESSORY,	  		  //小车或者吸盘在辅料工位	    //1
	ENUM_PRETARGETING, 		  //小车或者吸盘在预定位工位    //2	
    ENUM_TEMPORARY,	          //小车或者吸盘在临时放卡工位  //3
	ENUM_NEW_ACCESSORY,	      //小车或者吸盘在新辅料工位	//4
	ENUM_PRINT,     		  //小车或者吸盘在打印工位 	    //5
	ENUM_BLANK,     		  //小车或者吸盘在空白卡工位    //7
    ENUM_SUCK_ORIGIN,		  //小车或者吸盘在原点		   //8	
};



void InitializeMachine(void);
void ResetAllSignal(void);
void StepCtrlMachine(void);

// 其它变量的声明
extern unsigned char ParamBuf[];
extern unsigned char gnOperationNo;
extern unsigned char gStartBottonPressed;
extern unsigned char gPauseBottonPressed;
extern void ProcessUpdateParameter(void); //更新工位定位参数

#endif
