#include <QtCore>

#define FrameHeader 0xAA
#define FrameTail 0xDD

#define PROTOCOL_MSG_LEN 12

struct UInt16Parts {
    uint8_t lowByte;
    uint8_t highByte;
};

union UInt16Union {
    short value;
    struct UInt16Parts parts;
};

typedef enum {
    DEVNone = 0x00,
    Robot,
    PTZ,
    RoboticArm,
    Battery,
    IMU,
    Sensor,
} eDEVICE;

typedef enum {
    Robot_Stop,         /* 停止 	*/
    Robot_Foreward,     /* 前进     */
    Robot_Backward,     /* 后退     */
    Robot_Right_Up,     /* 右上 	*/
    Robot_Left_Up,      /* 左上 	*/
    Robot_Right_Down,   /* 右下 	*/
    Robot_Left_Down,    /* 左下 	*/
    Robot_Shift_Right,  /* 右移 · 	*/
    Robot_Shift_Left,   /* 左移·  	*/
    Robot_Rotate_Right, /* 原地右旋转 */
    Robot_Rotate_Left,  /* 原地左旋转 */
    RobotVelocity = 0x20,
    HandShake = 0xFF,
} eRobot;
