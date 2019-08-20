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
#define PULSE_TO_MILLIMETER(pul)   (pul*24) //把脉冲数转换为毫米，24为走1mm的脉冲数
/*************输入端口定义**************/

typedef enum
{
	ERROR_CHANNAL_LEAVE_ORG_FAIL  = 11001,  		//11001 抓卡小车离开原点失败
	ERROR_CHANNAL_RETURN_ORG_FAIL,  				//11002 抓卡小车返回原点失败
	ERROR_WARMING_DOWN_FAIL,      					//11003 加热下降失败
	ERROR_WARMING_RISE_FAIL,     		    		//11004 加热升起失败
	ERROR_COOLING_DOWN_FAIL,      					//11005 冷却下降失败
	ERROR_COOLING_RISE_FAIL,      					//11006 冷却升起失败
	ERROR_PRESS_DOWN_FAIL,      					//11007 冲压下降失败
	ERROR_PRESS_RISE_FAIL,      					//11008 冲压升起失败
	ERROR_RIGHT_CLAMP_CLOSE_FAIL,    				//11009 右边夹子关闭失败
	ERROR_RIGHT_CLAMP_OPEN_FAIL,     				//11010 右边夹子打开失败
	ERROR_LEFT_CLAMP_CLOSE_FAIL,			    	//11011 左边夹子关闭失败
	ERROR_LEFT_CLAMP_OPEN_FAIL,     				//11012 左边夹子打开失败
	ERROR_TROLLEY_CAR_POSITION_PARA_MAX,			//11013 抓卡小车上层传输的数据超出接收最大值
	ERROR_PRETARGETING_RETURN_ORIGIN_FAIL,			//11014 预定位返回原点失败
	ERROR_PRETARGETING_LEAVE_ORIGIN_FAIL,			//11015 预定位离开原点失败
	ERROR_WASTESTORAGE_RETURN_ORIGIN_FAIL,			//11016 废卡位返回原点失败
	ERROR_WASTESTORAGE_LEAVE_ORIGIN_FAIL,			//11017 废卡位离开原点失败	
	ERROR_PRETARGETING_NONE_CARD, 					//11018 预定位无卡
	ERROR_PRETARGETING_FROM_COOL_NONE_CARD, 		//11019 预定位到冷压位无卡
	ERROR_WARM_FROM_COOL_NONE_CARD, 				//11020 热压位到冷压位无卡
	ERROR_COOL_FROM_PRETARGETING_NONE_CARD, 		//11021 冷压位到预定位无卡
	ERROR_PRETARGETING_FROM_WASTE_NONE_CARD, 		//11022 预定位到废料位无卡
	ERROR_ELEVATOR_CAR_LEAVE_ORIGIN_FAIL ,   		//11023 读写检测小车离开原点失败
	ERROR_ELEVATOR_CAR_RETURN_ORIGIN_FAIL, 		    //11024 电梯小车返回原点失败
	ERROR_ELEVATOR_CAR_POSITION_PARA_MAX,   		//11025 电梯小车上层传输的数据超出接收最大值
	ERROR_CONNECTION_LEAVE_ORIGIN_FAIL,			    //11026 衔接模块离开原点失败
	ERROR_CONNECTION_RETURN_ORIGIN_FAIL,		    //11027 衔接模块返回原点失败
	ERROR_CONNECTION_TO_SENDCARD_LEAVE_ORIGIN_FAIL, //11028 衔接模块到发卡位离开原点失败
	ERROR_RWMODULE_NOT_CHECK_CARD,					//11029 读写模块没有检测到卡片
	ERROR_RWMODULE_NOT_IN_CARD,						//11030 读写模块没有卡片进入
	ERROR_CARD_SLOT_NOT_IN_CARD,					//11031 卡槽没有卡片进入
	ERROR_RECEIVE_CARD_POSITION_NOT_CARD,			//11032 电梯接卡位没有吸到卡
	ERROR_PRETARGETING_UP_LEAVE_ORIGIN_FAIL,		//11033 预定位升起时离开原点失败
	ERROR_WASTESTORAGE_DOWN_LEAVE_ORIGIN_FAIL,		//11034 废卡位下降离开原点失败

}ModulesErrorCode;

enum ENUM_CLAMPCARDCAR_LOCATION //抓卡小车位置
{
	ENUM_UNKNOW,	    	  //0位置未知		   
    ENUM_INSIDE_WARE,		  //1小车在热压工位	   
    ENUM_INSIDE_COOL,	      //2小车在冷压工位	     
	ENUM_INSIDE_PRETARGETING, //3小车在预定位工位   
	ENUM_INSIDE_CUT_CARD,     //4小车在剪卡工位 	
	ENUM_INSIDE_WASTE,	  	  //5小车在废料工位	    
    ENUM_INSIDE_ORIGIN,	   	  //6小车在原点		   	
};
enum ENUM_ELEVATOR_CAR_LOCATION //Elevator car position
{
	ENUM_ELEVATOR_UNKNOW,	  //0状态位置
    ENUM_RECEIVE_CARD,	      //1接卡位  			   		   
    ENUM_ELEVATOR_TO_RWMODULE,//2电梯到读写模块	  
    ENUM_RWMODULE_TO_ELEVATOR,//3读写模块到电梯(好卡出卡)
	ENUM_WASTE_CARD, 		  //4废卡位    	
	ENUM_OCR_CARD,	     	  //5OCR 检测位	
   	ENUM_ELEVATOR_CAR_ORG,	  //6电梯原点
};
void InitializeMachine(void);
void ResetAllSignal(void);
void StepCtrlMachine(void); 
// 其它变量的声明
extern unsigned char ParamBuf[];
extern unsigned char gnOperationNo;
extern void ProcessUpdateParameter(void); //更新工位定位参数

#endif
