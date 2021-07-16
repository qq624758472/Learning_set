#include <stdio.h>  
#include <unistd.h>  
#include <sys/ioctl.h>  
#include <fcntl.h>  
#include <string.h>  
  
#define IGPIO_IOCTL_MAGIC       0XF12  
#define IGPIO_IOCTL_RESET       _IO(IGPIO_IOCTL_MAGIC, 0x0)  
#define IGPIO_IOCTL_SET         _IO(IGPIO_IOCTL_MAGIC, 0x1)  
  
int main(int argc, char **argv)  
{  
    unsigned int id = 0;  
    unsigned int value = 0;  
    int fd = -1;  
    int ret;  
  
    if(argc <4){  
        printf("Usage:./app -s 194 0\n");  
        return -1;  
    }  
  
    sscanf(argv[2], "%d", &id);  
    sscanf(argv[3], "%x", &value);  
  
    fd = open("/dev/igpioDrv", O_RDWR, 0);  
    if (fd < 0) {  
        printf("open fail\n");  
        return -1;  
    }  
  
    if (0 == strncmp(argv[1], "-s", strlen(argv[1])))  
    {  
        if(value == 0)  
            ret = ioctl(fd, IGPIO_IOCTL_RESET, id);  
        else   
            ret = ioctl(fd, IGPIO_IOCTL_SET, id);  
        if (ret < 0) {  
            perror("ioctl fail\n");  
        }  
    }  
  
    close(fd);  
    return 0;  
}  