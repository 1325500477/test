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
	ERROR_RWCHANNEL_AND_LIFT_CARD_SLOT_ALL_HAVE_CARD = 11001,		//11001 下控制板复位时检测到读写通道与电梯卡槽同时有卡
	ERROR_RWCHANNEL_AND_LIFT_RECEIVE_ALL_HAVE_CARD,					//11002 下控制板复位时检测到读写通道与电梯接卡吸盘处都有卡
	ERROR_CHANNAL_LEAVE_ORG_FAIL,			  						//11003 抓卡小车离开原点失败
	ERROR_CHANNAL_RETURN_ORG_FAIL,  								//11004 抓卡小车返回原点失败
	ERROR_WARMING_DOWN_FAIL,      									//11005 加热下降失败
	ERROR_WARMING_RISE_FAIL,     		    						//11006 加热升起失败
	ERROR_COOLING_DOWN_FAIL,      									//11007 冷却下降失败
	ERROR_COOLING_RISE_FAIL,      									//11008 冷却升起失败
	ERROR_PRESS_DOWN_FAIL,      									//11009 冲压下降失败
	ERROR_PRESS_RISE_FAIL,      									//11010 冲压升起失败
	ERROR_RIGHT_CLAMP_CLOSE_FAIL,    								//11011 右边夹子关闭失败
	ERROR_RIGHT_CLAMP_OPEN_FAIL,     								//11012 右边夹子打开失败
	ERROR_LEFT_CLAMP_CLOSE_FAIL,			    					//11013 左边夹子关闭失败
	ERROR_LEFT_CLAMP_OPEN_FAIL,     								//11014 左边夹子打开失败
	ERROR_TROLLEY_CAR_POSITION_PARA_MAX,							//11015 抓卡小车上层传输的数据超出接收最大值
	ERROR_PRETARGETING_RETURN_ORIGIN_FAIL,							//11016 预定位返回原点失败
	ERROR_PRETARGETING_LEAVE_ORIGIN_FAIL,							//11017 预定位离开原点失败
	ERROR_PRETARGETING_UP_LEAVE_ORIGIN_FAIL,						//11018 预定位升起时离开原点失败	
	ERROR_WASTESTORAGE_RETURN_ORIGIN_FAIL,							//11019 废卡位返回原点失败
	ERROR_WASTESTORAGE_LEAVE_ORIGIN_FAIL,							//11020 废卡位离开原点失败
	ERROR_WASTESTORAGE_DOWN_LEAVE_ORIGIN_FAIL,						//11021 废卡位下降离开原点失败
	ERROR_PRETARGETING_NONE_CARD, 									//11022 预定位无卡
	ERROR_PRETARGETING_FROM_COOL_NONE_CARD, 						//11023 预定位到冷压位无卡
	ERROR_WARM_FROM_COOL_NONE_CARD, 								//11024 热压位到冷压位无卡
	ERROR_COOL_FROM_PRETARGETING_NONE_CARD, 						//11025 冷压位到预定位无卡
	ERROR_PRETARGETING_FROM_WASTE_NONE_CARD, 						//11026 预定位到废料位无卡
	ERROR_LIFT_CAR_LEAVE_ORIGIN_FAIL ,   							//11027 电梯小车离开原点失败
	ERROR_LIFT_CAR_RETURN_ORIGIN_FAIL, 		    					//11028 电梯小车返回原点失败
	ERROR_LIFT_CAR_POSITION_PARA_MAX,   							//11029 电梯小车上层传输的数据超出接收最大值
	ERROR_LIFT_CAR_LEAVE_HORIZON_ORIGIN_FAIL,	 					//11030 电梯小车离开水平原点失败
	ERROR_LIFT_CAR_RETURN_HORIZON_ORIGIN_FAIL,						//11031 电梯小车回水平原点失败
	ERROR_LIFT_CAR_HORIZON_POSITION_PARA_MAX, 						//11032 电梯水平移动接收参数错误
	ERROR_RWCHANNEL_HAVE_CARD_NOT_ENTER_CARD,	 					//11033 通道有卡不能进卡
	ERROR_RWCHANNEL_ENTER_CARD_FAIL,								//11034 读写模块没有卡片进入
	ERROR_RWCHANNEL_OUT_CARD_TO_LIFT_NO_CARD,						//11035	读写通道出卡到电梯无卡
	ERROR_RWCHANNEL_OUT_CARD_TO_LIFT_FAIL, 							//11036 读写通道出卡到电梯失败;
	ERROR_RECEIVE_CARD_POSITION_NOT_CARD,							//11037 电梯接卡位没有吸到卡
	ERROR_OCR_TO_RWCHANNEL_CARD_SLOT_NOT_CARD, 						//11038 OCR到读写通道卡槽无卡
	ERROR_RWCHANNEL_EXIT_CARD_NOT_CARD, 							//11039 读写通道出卡无卡
	ERROR_CARD_NOT_MOV_TO_PERFECT_CARD_POSITON, 					//11040 卡片未移动到好卡位
	ERROR_NONE_CARD_NOT_START_ENTER_CARD_ACTION,					//11041 无卡不能进行收卡动作
	ERROR_PERFECT_CARD_ENTER_FAIL,									//11042 好卡进卡失败
	ERROR_PERFECT_CARD_ENTER_TO_EXIT_CARD_SENSOR_FAIL,				//11043 好卡进卡到出卡口位置失败
	ERROR_LIFT_SLOT_TO_RWCH_SD_CARD_DETEC_HAV_CARD,					//11044 电梯槽往读写通道送卡时检测到读写通道已经有卡
	ERROR_LIFT_SLOT_NO_CARD_NO_SEND_CARD_TO_RWCH,					//11045 电梯槽无卡,无法送卡到读写通道
	ERROR_LIFT_SLOT_SEND_CARD_TO_RWCH_RETRY_FAIL,					//11046 电梯卡槽送卡到读写通道多次重试失败失败
	ERROR_RWCHANNEL_ENTER_CARD_TO_EXIT_CARD_SENSOR_FAIL,    		//11047 读写通道进卡到出卡传感器位置失败
	ERROR_LIFT_CARD_SLOT_HAVE_CARD_RWCHANNEL_NOT_SEND_CARD, 		//11048 电梯卡槽有卡,读写通道无法送卡到电梯卡槽
	ERROR_RWCHANNEL_NO_CARD_NO_EXIT_CARD_TO_LIFT_SLOT,				//11049 读写通道无卡，无法出卡到电梯卡槽
	ERROR_RWCHANNEL_EXIT_CARD_TO_LIFT_CARD_SLOT_FAIL,				//11050 读写通道出卡到电梯卡槽失败
	ERROR_LIFT_CARD_SLOT_FROM_RWCHANNEL_PULL_CARD_FAIL,				//11051 电梯卡槽从读写器通道拔卡失败
	ERROR_LIFT_CARD_NO_CARD_NO_EXIT_CARD_TO_RECYCLE_BOX,			//11052 电梯卡槽无卡,无法出卡到回收箱
	ERROR_LIFT_CARD_SLOT_SEND_CARD_TO_RECYCLE_CHANNEL_FAIL, 		//11053 电梯卡槽送卡到回收通道入口失败
	ERROR_LIFT_CARD_SLOT_TO_RECYCLE_BOX_FAIL,						//11054 电梯卡槽送卡到回收箱失败
	ERROR_RWCHANNEL_HAVE_MULTI_CARD,					    		//11055 读写通道有多张卡,或者传感器坏
	ERROR_RWCHANNEL_SEND_CARD_EIXT_CARD_FAIL,						//11056 读写通道送卡到出卡口传感器X128位置失败
	ERROR_RWCHANNEL_ENTER_CARD_SENSOR_TO_EXIT_CARD_SENSOR_FAIL,		//11057 读写通道进卡传感器X124位到出卡传感器X128位失败
	ERROR_RWCHANNLE_EXIT_CARD_SENSOR_TO_ENTER_CARD_SENSOR_FAIL, 	//11058 读写通道出卡传感器X128位到进卡传感器X124位失败
	ERROR_CARD_SLOT_AND_LIFT_RECEIVE_ALL_HAVE_CARD,					//11059 下控制板复位时检测到电梯卡槽与电梯接卡吸盘处都有卡

}ModulesErrorCode;

enum ENUM_CLAMPCARDCAR_LOCATION //抓卡小车位置
{
	ENUM_UNKNOW,	    	  	//0位置未知		   
    ENUM_INSIDE_WARE,		  	//1小车在热压工位	   
    ENUM_INSIDE_COOL,	      	//2小车在冷压工位	     
	ENUM_INSIDE_PRETARGETING, 	//3小车在预定位工位   
	ENUM_INSIDE_CUT_CARD,     	//4小车在剪卡工位 	
	ENUM_INSIDE_WASTE,	  	  	//5小车在废料工位	    
    ENUM_INSIDE_ORIGIN,	   	  	//6小车在原点		   	
};
enum ENUM_LIFT_UP_DN_LOCATION 	//电梯小车位置
{
	ENUM_LIFT_UP_DN_UNKNOW,	  			//0位置未知	
    ENUM_LIFT_SUCK_RECEIVE_CARD,		//1接卡位  			   		   
    ENUM_LIFT_SUCK_TO_RW_CHANNEL,		//2电梯吸卡到读写通道	  
    ENUM_LIFT_SLOT_GET_RW_CHANNEL,		//3电梯槽对读写通道
	ENUM_LIFT_SLOT_TO_RECYCLE_BOX, 		//4电梯槽对回收箱    	
	ENUM_LIFT_SLOT_TO_OCR,	     	  	//5电梯槽到OCR	
   	ENUM_LIFT_UP_DN_ORG,	  			//6电梯原点
};
enum ENUM_LIFT_HORIZON_LOCATION 		//电梯小车水平位置
{
	ENUM_LIFT_HORIZON_UNKNOW,  			//0位置未知	
    ENUM_LIFT_HORIZON_REC_AND_SEND,	    //1电梯水平接送卡位 			   		   
    ENUM_LIFT_HORIZON_OUT_TO_OCR,		//2电梯水平进卡到OCR	  
    ENUM_LIFT_HORIZON_GET_FROM_OCR,		//3OCR出卡到电梯水平
   	ENUM_LIFT_HORIZON_ORG,  			//4电梯小车水平原点
};

enum ENUM_STATION_HAVE_CARD
{
	STATION_HAS_CARD_NO = 0,
	STATION_HAS_CARD_YES,
	STATION_HAS_CARD_UNKNOW,
};

//工位有无卡状态描述
enum ENUM_STATION_HAVE_CARD_INDEX
{
	STATION_LOADING_PLATFORM_HAS_CARD= 0,			//1预定位有无卡
//	STATION_HOT_PRESS_OUT_SENSOR_HAS_CARD,			//出热压传感器检测有卡
//	STATION_COLD_PRESS_OUT_SENSOR_HAS_CARD,			//出冷压传感器检测有卡
	STATION_LIFT_SUCK_HAS_CARD,						//2电梯吸盘处吸住了卡
	STATION_LIFT_MID_HAS_CARD,						//3电梯有卡
	STATION_OUT_PORT_HAS_CARD,						//4出卡通道有卡
	STATION_RECYCLE_BOX_FULL,						//5回收箱满
	STATION_RECYCLE_BOX_IN_PLACE,					//6回收箱到位
	STATION_RECYCLE_BOX_HAS_CARD,					//7回收箱有卡
	STATION_WASTER_BOX_FULL,						//8废料箱满
	STATION_WASTER_BOX_IN_PLACE,					//9废料箱到位

	//...............
	MAX_STATION_HAS_CARD_NUM = 10,	   				//指令集通讯接口固定为20个字节的状态
};

void InitializeMachine(void);
void ResetAllSignal(void);
void StepCtrlMachine(void); 
void UpdateStatus(void);
// 其它变量的声明
extern unsigned char ParamBuf[];
extern unsigned char gnOperationNo;
extern void ProcessUpdateParameter(void); //更新工位定位参数
extern unsigned char CardStatus[10];

#endif
