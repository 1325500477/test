#ifndef __IO_CFG__H__
#define __IO_CFG__H__

#ifdef __cplusplus
extern "C"
{
#endif

    //***** 输入端口定义****************************************************************
#define SN_PREPOSITION_ORG			IN1	    // 预定位原点
#define SN_PRETARGETING_CHECK_CARD	IN2	    // 预定位检卡传感器
#define SN_COOL_CHECK_CARD        	IN3	    // 冷压位检卡传感器
#define SN_WARM_CHECK_CARD			IN4	    // 热压位检卡传感器
#define SN_CAR_CHANNAL_ORG			IN5	    // 抓卡小车原点检测
#define SN_CLAMP_RIGHT_CLOSE		IN6	    // 右侧夹子关闭检测
#define SN_CLAMP_RIGHT_OPEN			IN7	    // 右侧夹子打开检测
#define SN_CLAMP_LEFT_CLOSE 		IN8	    // 左侧夹子关闭检测
#define SN_CLAMP_LEFT_OPEN			IN9	    // 左侧夹子打开检测
#define SN_WARM_UP					IN10    // 加热部分上升检测
#define SN_WARM_DOWN				IN11    // 加热部分下降检测
#define SN_COOL_UP					IN12    // 冷却部分上升检
#define SN_COOL_DOWN				IN13    // 冷却部分下降检测
#define SN_PUNCH_DOWN				IN14    // 冲压部分下降检
#define SN_PUNCH_UP					IN15    // 冲压部分上升检测
#define SN_WASTESTORAGE_ORG			IN16    // 废卡检测原点
#define SN_PRETARGETING_BAROMETER   IN17    // 预定位压力表
#define SN_SUCK_BAROMETER           IN18    // 吸盘压力表
#define SN_WASTER_CHECK_CARD        IN19    // 废卡位检测卡传感器
#define SN_SUCK_CARD_CAR_ORG  		IN20    // 吸卡小车原点传感
#define SN_SUCK_ORG				    IN21    // 吸盘原点传感
#define SN_BLANK_CARD_BOX_ORG		IN22    // 空白卡箱原点
#define SN_BLANK_CARD_BOX_CHECK     IN23    // 白卡箱检测有无卡传感器
#define SN_BLANK_CARD_BOX_UP        IN24    // 白卡箱顶部传感器
#define SN_SUCK_CHECK_CARD          IN25    // 吸盘检卡传感器



    //***** 输出端口定义（单向直流马达、电磁铁、指示灯...）*******************************
#define DM_BUTTON1					DM1		    // 按钮1指示灯
#define DM_BUTTON2					DM2		    // 按钮2指示灯
#define DM_BUTTON3					DM3		    // 按钮3指示灯
#define DM_BUTTON4					DM4		    // 按钮4指示灯
	
#define DM_WARM_CONTROL				DM5		    // 加热继电器5控制
#define DM_COOL_CONTROL				DM6		    // 冷却继电器6控制
#define DM_PRES_CONTROL				DM7		    // 冲压继电器7控制
#define DM_ALL_CONTROL				DM8		    // 总继电器8控制	
#define DM_PNEUMATIC_SWITCH			DM2		    // 总气开关
#define DM_PRETARGETING_VACUUMCUP	DM3		    // 预定位真空吸
#define DM_SUCK_VACUUMCUP	        DM4	        // 吸盘真空吸
#define DM_CLAMP_RIGHT				DM9		    // 右侧夹子电机
#define DM_CLAMP_LEFT				DM10	    // 左侧夹子电机

    //***** 步进电机编号定义***********************************************************
#define SM_CHANNEL              	SM_1        // 抓卡小车电机SM101
#define SM_WASTESTORAGE             SM_2        // 废料场电机SM102
#define SM_PRETARGETING             SM_3        // 预定位电机SM103
#define SM_SUCK_CARD_CAR            SM_4        // 吸卡小车电机SM104
#define SM_SUCK                     SM_5        // 吸盘电机SM105
#define SM_BLANK_CARD_BOX           SM_6        // 空白卡箱电机SM106

    //***** 步进电机转动方向定义********************************************************
#define DIR_SM_CHANNEL_BACKWARD         SM_CCW     // 通道电机返回 
#define DIR_SM_CHANNEL_FORWARD     	    SM_CW      // 通道电机前进 
#define DIR_SM_PRETARGETING_FORWARD     SM_CW      // 预定位向上
#define DIR_SM_PRETARGETING_BACKWARD    SM_CCW     // 预定位向下
#define DIR_SM_SUCK_CARD_CAR_FORWARD    SM_CW      // 吸卡小车前进
#define DIR_SM_SUCK_CARD_CAR_BACKWARD   SM_CCW     // 吸卡小车后退
#define DIR_SM_SUCK_UP    	            SM_CCW     // 吸盘上升
#define DIR_SM_SUCK_DOWN   	            SM_CW      // 吸盘下降
#define DIR_SM_BLANK_CARD_BOX_UP        SM_CCW     // 白卡箱上升
#define DIR_SM_BLANK_CARD_BOX_DOWN      SM_CW      // 白卡箱下降

    //***** 直流电机动作状态定义********************************************************
#define DM_CLAMP_OPEN           	    DM_CW       // 挡板控制电机顺时针转动
#define DM_CLAMP_CLOSE          	    DM_CCW      // 挡板控制电机逆时针转动
#define DMS_LIGHT_ON            	    DM_ON       // 按钮指示灯亮起
#define DMS_LIGHT_OFF           	    DM_OFF      // 按钮指示灯熄灭

    //***** 步进电机动作状态定义********************************************************
#define SMS_ENABLE              	    SM_ENABLE   // 使能电机
#define SMS_DISABLE             	    SM_DISABLE  // 去电机使能

    /****** 如果需要绝对坐标定义，请仔细核对*************************/
#define SM1_DIR_ABS_P	        	SM_CW		//绝对方向正向定义
#define SM2_DIR_ABS_P	        	SM_CCW		//绝对方向正向定义
#define SM3_DIR_ABS_P	       	 	SM_CCW		//绝对方向正向定义
#define SM4_DIR_ABS_P	        	SM_CW		//绝对方向正向定义
#define SM5_DIR_ABS_P	        	SM_CW		//绝对方向正向定义
#define SM6_DIR_ABS_P	        	SM_CCW		//绝对方向正向定义
#define SM7_DIR_ABS_P	        	SM_CW		//绝对方向正向定义
#define SM8_DIR_ABS_P	        	SM_CW		//绝对方向正向定义

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
