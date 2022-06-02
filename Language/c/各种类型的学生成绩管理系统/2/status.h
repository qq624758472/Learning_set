#ifndef STATUS_H
#define STATUS_H

typedef enum
{
	EXCEP =  1, // 执行异常
    RIGHT =  0, // 执行成功
    ERROR = -1, // 执行错误
    PRNIL = -2, // 参数为空
    SPILL = -3, // 参数过界
    IDERR = -4, // 帐号错误
    PDERR = -5  // 密码错误
}Status;

#endif// STATUS_H
