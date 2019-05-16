#ifndef __IO_CFG__H__
#define __IO_CFG__H__

#ifdef __cplusplus
extern "C"
{
#endif

    //***** 输入端口定义****************************************************************
#define SN_BUTTON1					IN1			// 按钮1
#define SN_BUTTON2					IN2			// 非接位检测传感器
#define SN_TROLLEY_UP				IN3			// 门下压位检测传感器
#define SN_TROLLEY_DOWN				IN4			// 门关闭检测传感器
	
#define SN_CHANNAL_ORG				IN5			// 通道原点检测
#define SN_CLAMP_RIGHT_CLOSE		IN6			// 右侧夹子关闭检测
#define SN_CLAMP_RIGHT_OPEN			IN7			// 右侧夹子打开检测
#define SN_CLAMP_LEFT_CLOSE 		IN8			// 左侧夹子关闭检测
#define SN_CLAMP_LEFT_OPEN			IN9			// 左侧夹子打开检测
#define SN_WARM_UP					IN10		// 加热部分上升检测
#define SN_WARM_DOWN				IN11		// 加热部分下降检测
#define SN_COOL_UP					IN12		// 冷却部分上升检
#define SN_COOL_DOWN				IN13		// 冷却部分下降检测
#define SN_PUNCH_DOWN				IN14		// 冲压部分下降检
#define SN_PUNCH_UP					IN15		// 冲压部分上升检测
#define SN_CHANNAL_RIGHT			IN16		// 通道右侧检测


    //***** 输出端口定义（单向直流马达、电磁铁、指示灯...）*******************************
#define DM_BUTTON1					DM1		// 按钮1指示灯
#define DM_BUTTON2					DM2		// 按钮2指示灯
#define DM_BUTTON3					DM3		// 按钮3指示灯
#define DM_BUTTON4					DM4		// 按钮4指示灯
	
#define DM_WARM_CONTROL				DM5		// 加热继电器5控制
#define DM_COOL_CONTROL				DM6		// 冷却继电器6控制
#define DM_PRES_CONTROL				DM7		// 冲压继电器7控制
#define DM_ALL_CONTROL				DM8		// 总继电器8控制
	
#define DM_CLAMP_RIGHT				DM9		// 右侧夹子电机
#define DM_CLAMP_LEFT				DM10	// 左侧夹子电机


    //***** 步进电机编号定义***********************************************************
#define SM_CHANNEL              	SM_1        // 通道控制电机SM101
#define SM_TROLLEY                	SM_2        // 小车电机SM102
#define SM_BELT                 	SM_4        // 传送带控制电机SM104



    //***** 步进电机转动方向定义********************************************************
#define DIR_SM_CHANNEL_BACKWARD     SM_CW       //通道电机返回 
#define DIR_SM_CHANNEL_FORWARD     	SM_CCW      //通道电机前进 

#define DIR_SM_TROLLEY_FORWARD     	SM_CW       // 传送带传送电机向前转动方向
#define DIR_SM_TROLLEY_BACKWARD    	SM_CCW      // 传送带传送电机后退转动方向

    //***** 直流电机动作状态定义********************************************************
#define DM_CLAMP_OPEN           	DM_CW       // 挡板控制电机顺时针转动
#define DM_CLAMP_CLOSE          	DM_CCW      // 挡板控制电机逆时针转动
#define DMS_LIGHT_ON            	DM_ON       // 按钮指示灯亮起
#define DMS_LIGHT_OFF           	DM_OFF      // 按钮指示灯熄灭

    //***** 步进电机动作状态定义********************************************************
#define SMS_ENABLE              	SM_ENABLE   // 使能电机
#define SMS_DISABLE             	SM_DISABLE  // 去电机使能


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

#define RELAY5_ON    1    //继电器5开
#define RELAY5_OFF   0    //继电器5关
#define RELAY6_ON    1    //继电器6开
#define RELAY6_OFF   0    //继电器6关
#define RELAY7_ON    1    //继电器7开
#define RELAY7_OFF   0    //继电器7关
#define RELAY8_ON    1    //继电器8开
#define RELAY8_OFF   0    //继电器8关



#ifdef __cplusplus
}
#endif

#endif
