/*
 * @Description:
 * @Version: 2.0
 * @Autor: ruog__
 * @Date: 2022-05-28 14:11:04
 * @LastEditors: ruog__
 * @LastEditTime: 2022-05-30 11:47:47
 */
#ifndef __IOCTL_CMD__
#define __IOCTL_CMD__



//定义设备类型
#define IOC_MAGIC 'l'

//初始化设备
#define IOC_START    _IO(IOC_MAGIC,1)
#define IOC_PAUSE    _IO(IOC_MAGIC,2)
#define IOC_RESTART  _IO(IOC_MAGIC,3)
#define IOC_STOP     _IO(IOC_MAGIC,4)

#define IOC_PAUSE_1  _IO(IOC_MAGIC,5)
#define IOC_PAUSE_2  _IO(IOC_MAGIC,6)
#define IOC_START_1  _IO(IOC_MAGIC,7)
#define IOC_START_2  _IO(IOC_MAGIC,8)
#define IOC_STOP_1   _IO(IOC_MAGIC,9)
#define IOC_STOP_2   _IO(IOC_MAGIC,10)
#define IOC_RESTART_1 _IO(IOC_MAGIC,11)
#define IOC_RESTART_2 _IO(IOC_MAGIC,12)

//
#define IOCGREG _IOW(IOC_MAGIC,1,int)

#define IOCGREG _IOW(IOC_MAGIC,2,int)

struct msg
{
    int addr;
    unsigned int data;
};

#endif