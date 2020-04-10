#ifndef __STANDARD_H__
#define __STANDARD_H__

#ifdef __cplusplus
extern "C"
{
#endif

typedef unsigned char		BOOL;
typedef unsigned char		BYTE;
typedef unsigned short		WORD;
typedef unsigned long		DWORD;
typedef unsigned char		UCHAR;
typedef unsigned int		UINT;
typedef unsigned long		ULONG;
typedef unsigned char		uchar;
typedef unsigned int		uint;
typedef unsigned long		ulong;

typedef unsigned char		U8;
typedef unsigned short		U16;
typedef unsigned long		U32;
typedef signed char			S8;
typedef signed int			S16;
typedef signed long			S32;

typedef unsigned char		uint8;
typedef unsigned short		uint16;
typedef unsigned long		uint32;
typedef signed char			int8;
typedef signed int			int16;
typedef signed long			int32;

#define TRUE	1
#define FALSE	0

#define PROCESS_FAIL    2
#define PROCESS_SUCCESS 1
#define PROCESS_CLEAR	0

#define ON		1
#define OFF		0

//#define ENABLE	1
//#define DISABLE	0


#define BUSY	1
#define FREE	0

// 传感器类型（反射式传感器/对射式传感器）
#define SENSOR_TYPE_REFLECT		0		// 反射式传感器
#define SENSOR_TYPE_BEAM		1		// 对射式传感器
//对射式感应器，检测到物体，则sensor被阻挡不受光，此时CPU检测到为高电平（有物体为高，无物体为低)
#define SENSOR_TYPE_BEAM_ON		1		//检测到有物体
#define SENSOR_TYPE_BEAM_OFF	0		//无物体（板子灯亮）

//反射式感应器，检测到物体，则sensor被反射而受光，此时CPU检测到为低电平（有物体为低，无物体为高)
#define SENSOR_TYPE_REFLECT_ON  0		//检测到有物体（板子灯亮）
#define SENSOR_TYPE_REFLECT_OFF 1		//无物体
// 微动式开关检测传感器
#define SENSOR_TYPE_MICRO_ON    0		// 检测到双卡
#define SENSOR_TYPE_MICRO_OFF   1		// 未检测到双卡

// 真空检测传感器
#define SENSOR_TYPE_VACUUM_ON   0		// 真空状态（已经吸到卡片）
#define SENSOR_TYPE_VACUUM_OFF  1		// 非真空状态

// 按键开关宏定义
#define BUTTON_TYPE_PRESSED     0		// 按下按键
#define BUTTON_TYPE_UNPRESSED   1		// 松开按键

// 限位开关宏定义
#define SENSOR_TYPE_LIMIT_ON    0		// 限位开关被触发
#define SENSOR_TYPE_LIMIT_OFF   1		// 限位开关未被触发

//磁感应传感器
#define SENSOR_TYPE_MAGNET_ON	0		//检测到有物体
#define SENSOR_TYPE_MAGNET_OFF	1		//无物体

//开电，5灯全亮
//初始化中，3灯全亮
//初始化完成，三色灯黄灯，暂停灯
//调试，三色灯黄灯，绿色
//运行，三色灯绿色，启动灯
//暂停，三色灯黄灯，暂停灯
//故障，三色灯红色，暂停灯
//离线，5灯全亮	
#define DEVICE_STATUS_UNKNOW            0	//未知
#define DEVICE_STATUS_INITIAL           1	//初始化
#define DEVICE_STATUS_RESET_FINISHED    2	//初始化完成
#define DEVICE_STATUS_DEBUG				3	//调试中	
#define DEVICE_STATUS_RUN				4	//运行
#define DEVICE_STATUS_PAUSE				5	//暂停
#define DEVICE_STATUS_ERROR				6	//错误
#define DEVICE_STATUS_DISCONNECT        7	//断开连接

//(1-复位，2-运行，3-暂停，4-恢复，5-停止，6-调试)
#define WORK_MODE_UNKNOW	0
#define	WORK_MODE_RESET		1
#define WORK_MODE_RUN		2
#define WORK_MODE_PAUSE		3
#define WORK_MODE_RESUME	4
#define WORK_MODE_STOP		5
#define WORK_MODE_DEBUG		6

#define WORK_MODE_SPECIAL 0xFF

/* macro for input binary */
#define LongToBin(n) \
(\
((n >> 21) & 0x80) | \
((n >> 18) & 0x40) | \
((n >> 15) & 0x20) | \
((n >> 12) & 0x10) | \
((n >> 9) & 0x08) | \
((n >> 6) & 0x04) | \
((n >> 3) & 0x02) | \
((n ) & 0x01) \
)

#define Bin(n) LongToBin(0x##n##l)

union data_bit
{
	uchar ucData;
	struct
	{
		uchar ucData_0:1;
		uchar ucData_1:1;
		uchar ucData_2:1;
		uchar ucData_3:1;
		uchar ucData_4:1;
		uchar ucData_5:1;
		uchar ucData_6:1;
		uchar ucData_7:1;
	} ucData_bit;
};
enum _opration{
	START=0,
	STEP1,
	STEP2,
	STEP3,
	STEP4,
	STEP5,
	STEP6,
	STEP7,
	STEP8,
	STEP9,
	STEP10,
	STEP11,
	STEP12,
	STEP13,
	STEP14,
	STEP15,
	STEP16,
	STEP17,
	STEP18,
	STEP19,
	STEP20,
	STEP21,
	STEP22,
	STEP23,
	STEP24,
	STEP25,
	STEP26,
	STEP27,
	STEP28,
	STEP29,
	STEP30,
	STEP31,
	STEP32,
	STEP33,
	STEP34,
	STEP35,
	STEP36,
	STEP37,
	STEP38,
	STEP39,

	STEP40,
	STEP41,
	STEP42,
	STEP43,
	STEP44,
	STEP45,
	STEP46,
	STEP47,
	STEP48,
	STEP49,
	STEP50,
	STEPEX01,   
	STEPEX02,
	STEPEX03,
	STEPEX04,
	STEPEX05,
	STEPEX06,
	STEPEX07,
	STEPEX08,

	STEP_FAIL = 254,
	STEP_SUCCESS = 255,
};

#ifdef __cplusplus
}
#endif

#endif
