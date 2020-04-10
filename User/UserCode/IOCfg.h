#ifndef __IO_CFG__H__
#define __IO_CFG__H__

#ifdef __cplusplus
extern "C"
{
#endif

//***** 输入端口定义****************************************************************
    #define SN_CAR_CHANNAL_ORG			IN1	   // 抓卡小车原点检测
    #define SN_PRETARGETING_CHECK_CARD	IN2	   // 预定位检卡传感器
    #define SN_WARM_CHECK_CARD			IN3	   // 热压位检卡传感器
    #define SN_COOL_CHECK_CARD        	IN4	   // 冷压位检卡传感器
    #define SN_PREPOSITION_ORG			IN5	   // 预定位原点传感器
    #define SN_CLAMP_RIGHT_CLOSE		IN6	   // 右侧夹子关闭检测
    #define SN_CLAMP_RIGHT_OPEN			IN7	   // 右侧夹子打开检测
    #define SN_CLAMP_LEFT_CLOSE 		IN8	   // 左侧夹子关闭检测
    #define SN_CLAMP_LEFT_OPEN			IN9	   // 左侧夹子打开检测
    #define SN_COOL_UP					IN10   // 冷却部分上升检测
    #define SN_WARM_UP					IN12   // 加热部分上升检测

    #define SN_COOL_DOWN				IN11   // 冷却部分下降检测 
    #define SN_WARM_DOWN				IN13   // 加热部分下降检测

    #define SN_PUNCH_DOWN				IN14   // 冲压部分下降检测
    #define SN_PUNCH_UP					IN15   // 冲压部分上升检测
    #define SN_WASTESTORAGE_ORG			IN16   // 废料位原点传感器
    #define SN_PRETARGETING_BAROMETER   IN17   // 预定位负压表检测是否吸到卡
    #define SN_WASTE_BOX_CHECK          IN18   // 废料箱到位检测
    #define SN_WASTE_CHECK_CARD         IN19   // 废料位检测夹子上是否有废料
    #define SN_LIFT_HORIZON_ORG     	IN20   // 电梯小车水平原点
    #define SN_RECEIVE_CARD        	    IN21   // 电梯接卡负压表检测是否吸到剪切后的卡
    #define SN_CARD_SLOT                IN22   // 电梯卡槽检测是否有卡
    #define SN_LIFT_UP_DN_ORG         	IN23   // 电梯小车原点 
    #define SN_RW_CH_NEAR_LIFT	    	IN24   // 读写通道和废卡通道电梯侧进卡通道传感器
    #define SN_WASTER_CARD_BOX_FULL     IN25   // 废卡箱满传感器
    #define SN_WASTER_CARD_BOX_EMPTY    IN26   // 废卡箱空传感器
    #define SN_WASTER_CARD_BOX_CHECK    IN27   // 废卡箱到位检测传感器
    #define SN_RW_CH_NEAR_OUTSIDE       IN28   // 出卡口传感器
    #define SN_WASTE_BOX_FULL           IN29   // 废料箱满传感器
    #define SN_RW_CH_LIFT_ENTER_CARD    IN30   // 读写通道电梯侧进卡通道传感器
    //***** 输出端口定义（单向直流马达、电磁铁、指示灯...）*******************************
    #define DM_PUNCH				    DM1	   // 切卡电机
    // #define DM_RWCHANNEL				DM2	   // 读写通道电机
    // #define DM_PRES_CONTROL		        DM3	   // 冲压继电器控制
    #define DM_CLAMP_RIGHT				DM4	   // 右侧夹子电机
    #define DM_CLAMP_LEFT				DM5    // 左侧夹子电机
    // #define DM_ALL_CONTROL			    DM6	   // 总液压开关
    #define DM_PNEUMATIC_SWITCH			DM7	   // 总气开关
    #define DM_HEATER_POWER             DM8    // 加热管开关
    #define DM_RECEIVE_CARD				DM9	   // 接卡真空控制
    #define DM_PRETARGETING_VACUUMCUP	DM10   // 预定位真空吸
    // #define DM_HYDRULIC_POWER           DM11   // 液压机控制 


    //***** 步进电机编号定义***********************************************************
    #define SM_CHANNEL              	SM_1   // 抓卡小车电机SM101
    #define SM_PRETARGETING             SM_2   // 预定位电机SM102   
    #define SM_WARM_PRESS               SM_3   // 热压场电机SM103
    #define SM_COOL_PRESS               SM_4   // 冷压电机SM104

    #define SM_RWMODULE                 SM_4   // 读写模块电机SM104
    
    #define SM_LIFT_HORIZON_MOVE        SM_5   // 电梯小车水平移动电机SM105
    #define SM_LIFT_UP_DN             	SM_6   // 电梯小车电机SM106

    //***** 步进电机转动方向定义********************************************************
    #define DIR_SM_CHANNEL_FORWARD     	    SM_CW  // 通道电机前进 
    #define DIR_SM_CHANNEL_BACKWARD         SM_CCW // 通道电机返回
    #define DIR_SM_PRETARGETING_FORWARD     SM_CW  // 预定位向上
    #define DIR_SM_PRETARGETING_BACKWARD    SM_CCW // 预定位向下
    #define DIR_SM_LIFT_UP          		SM_CCW // 电梯小车上升
    #define DIR_SM_LIFT_DN        			SM_CW  // 电梯小车下降
    #define DIR_SM_LIFT_HORIZON_BACK        SM_CCW // 电梯水平后退
    #define DIR_SM_LIFT_HORIZON_FRONT       SM_CW  // 电梯水平前进
    #define DIR_SM_RW_CH_IN_FROM_LIFT       SM_CCW // 读写模块从电梯进卡
    #define DIR_SM_RW_CH_OUT_LIFT        	SM_CW  // 读写模块出卡到电梯 
    #define DIR_SM_ENTER_RECYCLE_BOX        SM_CW  // 进回收箱 
    #define DIR_SM_PRESS_UP                 SM_CW  // 热冷压升起 
    #define DIR_SM_PRESS_DN                 SM_CCW // 热冷压下降
    //***** 直流电机动作状态定义********************************************************
    #define DIR_DM_PUNCH_UP                 DM_CW  // 切卡升起 
    #define DIR_DM_PUNCH_DN                 DM_CCW // 切卡下降  
    #define DIR_DM_WASTER_UP                 DM_CW  // 废料升起 
    #define DIR_DM_WASTER_DN                 DM_CCW // 废料下降  

    //***** 步进电机动作状态定义********************************************************
    #define SMS_ENABLE              	SM_ENABLE  // 使能电机
    #define SMS_DISABLE             	SM_DISABLE // 去电机使能

    /****** 如果需要绝对坐标定义，请仔细核对*************************/
    #define SM1_DIR_ABS_P	        	SM_CW	//绝对方向正向定义
    #define SM2_DIR_ABS_P	        	SM_CCW	//绝对方向正向定义
    #define SM3_DIR_ABS_P	       	 	SM_CCW	//绝对方向正向定义
    #define SM4_DIR_ABS_P	        	SM_CW	//绝对方向正向定义
    #define SM5_DIR_ABS_P	        	SM_CW	//绝对方向正向定义
    #define SM6_DIR_ABS_P	        	SM_CCW	//绝对方向正向定义
    #define SM7_DIR_ABS_P	        	SM_CW	//绝对方向正向定义
    #define SM8_DIR_ABS_P	        	SM_CW	//绝对方向正向定义

    enum _enum_motor_type
    {
        STEP_MOTOR  = 0,     //步进电机
        SERVO_MOTOR = 1,    //伺服电机
    };

    #define MOTOR_TYPE_SM1  STEP_MOTOR  //步进电机
    #define MOTOR_TYPE_SM2  STEP_MOTOR  //步进电机
    #define MOTOR_TYPE_SM3  STEP_MOTOR  //步进电机
    #define MOTOR_TYPE_SM4  STEP_MOTOR  //步进电机
    #define MOTOR_TYPE_SM5  STEP_MOTOR  //步进电机
    #define MOTOR_TYPE_SM6  STEP_MOTOR  //步进电机
    #define MOTOR_TYPE_SM7  STEP_MOTOR  //步进电机
    #define MOTOR_TYPE_SM8  SERVO_MOTOR //伺服电机

    #define RELAY_ON    1    //继电器开
    #define RELAY_OFF   0    //继电器关

#ifdef __cplusplus
}
#endif

#endif
