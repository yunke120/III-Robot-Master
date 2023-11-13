#include <QtCore>

#define FrameHeader 0xAA
#define FrameTail 0xDD

struct UInt16Parts {
    uint8_t lowByte;
    uint8_t highByte;
};

// 定义一个联合体，用于存储16位整数和结构
union UInt16Union {
    short value;
    struct UInt16Parts parts;
};

typedef enum
{
    DEVNone = 0x00,
    Robot,
    PTZ,
    RoboticArm,
    Battery,
    IMU,
    Sensor,
} eDEVICE;

typedef enum
{
    GetVoltage = 0x01,
} eBattery;

typedef enum
{
    RobotVelocity = 0x20,
    HandShake = 0xFF,
} eRobot;

typedef enum
{
    AngularVelocity = 0x01, /* 角速度 */
    Posture,				/* 姿态 */
    Quaternion,				/* 四元数 */
}eIMU;

typedef enum
{
    GetMQ2 = 0x01,
}eSensor;

typedef struct
{
    uint8_t buf[12];
    uint8_t user_len;
}send_msg;









