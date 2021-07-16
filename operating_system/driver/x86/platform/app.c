#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
 
#define PWM_IOCTL_SET_FREQ      1
#define PWM_IOCTL_STOP          0
main()
{
	int fd;
 
	fd = open("/dev/pwm",O_RDWR);
	if(fd<0)
	{
		perror("open fail \n");
		return ;
	}
 
	ioctl(fd,PWM_IOCTL_SET_FREQ,100);
	sleep(10);
	ioctl(fd,PWM_IOCTL_STOP);	
 
	close(fd);
}