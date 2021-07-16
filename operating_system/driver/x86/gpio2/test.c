#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/ioctl.h>
#include <linux/delay.h>
#include <linux/bcd.h>
#include <linux/capability.h>
#include <linux/rtc.h>
#include <linux/cdev.h>
#include <linux/miscdevice.h>
#include <linux/gpio.h>
#include <linux/slab.h>

#include <../arch/arm/mach-mx28/mx28_pins.h>
#include "gpio_driver.h"


struct gpio_info {
	u32  	 	   pin;
	char 	 	   pin_name[20];
	struct miscdevice *pmiscdev;
};

static struct gpio_info *gpio_info_file[255];

static struct gpio_info *all_gpios_info;

static struct gpio_info all_gpios_info_283[] ={
        {PINID_AUART0_CTS,   "gpio-DURX",       NULL},
        {PINID_AUART0_RTS,   "gpio-DUTX",       NULL},
        {PINID_AUART0_RX,    "gpio-URX0",       NULL},
        {PINID_AUART0_TX,    "gpio-UTX0",       NULL},
        {PINID_AUART1_RX,    "gpio-URX1",       NULL},
        {PINID_AUART1_TX,    "gpio-UTX1",       NULL},
        {PINID_SSP2_SCK,     "gpio-URX2",       NULL},
        {PINID_SSP2_MOSI,    "gpio-UTX2",       NULL},
        {PINID_SSP2_MISO,    "gpio-URX3",       NULL},
        {PINID_SSP2_SS0,     "gpio-UTX3",       NULL},
        {PINID_SAIF0_BITCLK, "gpio-URX4",       NULL},
        {PINID_SAIF0_SDATA0, "gpio-UTX4",       NULL},
        /* modify by luozhizhuo*/
        {PINID_GPMI_RDY3,    "gpio-CRX0", 	NULL},
        {PINID_GPMI_RDY2,    "gpio-CTX0", 	NULL},
        {PINID_GPMI_CE3N,    "gpio-CRX1", 	NULL},
        {PINID_GPMI_CE2N,    "gpio-CTX1", 	NULL},
        {PINID_LCD_D22,      "gpio-RUN",        NULL},
        {PINID_LCD_D23,      "gpio-ERR",    	NULL},
        /*end modify*/
        {PINID_SSP3_MISO,    "gpio-MISO",       NULL},
        {PINID_SSP3_MOSI,    "gpio-MOSI",       NULL},
        {PINID_SSP3_SCK,     "gpio-CLK",        NULL},
        {PINID_SSP3_SS0,     "gpio-CS",         NULL},
        {PINID_PWM1,         "gpio-SDA",        NULL},
        {PINID_PWM0,         "gpio-SCL",        NULL},
        {PINID_LCD_D17,      "gpio-P1.17",      NULL},
        {PINID_LCD_D18,      "gpio-P1.18",      NULL},
        {PINID_SSP0_DATA4,   "gpio-P2.4",       NULL},
        {PINID_SSP0_DATA5,   "gpio-P2.5",       NULL},
        {PINID_SSP0_DATA6,   "gpio-P2.6",       NULL},
        {PINID_SSP0_DATA7,   "gpio-P2.7",       NULL},
        {PINID_SSP1_SCK,     "gpio-P2.12",      NULL},
        {PINID_SSP1_CMD,     "gpio-P2.13",      NULL},
        {PINID_SSP1_DATA0,   "gpio-P2.14",      NULL},
        {PINID_SSP1_DATA3,   "gpio-P2.15",      NULL},
        {PINID_SAIF0_MCLK,   "gpio-P3.20",      NULL},
        {PINID_SAIF0_LRCLK,  "gpio-P3.21",      NULL},
        {PINID_SAIF1_SDATA0, "gpio-P3.26",      NULL},
        {PINID_SPDIF,        "gpio-P3.27",      NULL},
        {0,                "",          NULL},   //the end
};


/*--------------------------------------------------------------------------------------------------------
*/
static int gpio_open(struct inode *inode, struct file *filp);
static int  gpio_release(struct inode *inode, struct file *filp);
ssize_t gpio_write(struct file *filp, const char __user *buf, size_t count,
                loff_t *f_pos);
static int gpio_ioctl(struct inode *inode,struct file *flip,unsigned int command,unsigned long arg);
static int gpio_init(void);
static void gpio_exit(void);

/*--------------------------------------------------------------------------------------------------------
*/

static int gpio_open(struct inode *inode, struct file *filp)
{
	struct gpio_info *gpio_info_tmp;
	u32 minor = iminor(inode);

	gpio_info_tmp = gpio_info_file[minor];

	gpio_free(MXS_PIN_TO_GPIO(gpio_info_tmp->pin));
	if (gpio_request(MXS_PIN_TO_GPIO(gpio_info_tmp->pin), gpio_info_tmp->pin_name)) {
		printk("request %s gpio faile \n", gpio_info_tmp->pin_name);
		return -1;
	}

	filp->private_data = gpio_info_file[minor];

	return 0;
}

static int  gpio_release(struct inode *inode, struct file *filp)
{
	struct gpio_info *gpio_info_tmp = (struct gpio_info *)filp->private_data;

	gpio_free(MXS_PIN_TO_GPIO(gpio_info_tmp->pin));

	return 0;
}


ssize_t gpio_write(struct file *filp, const char __user *buf, size_t count,
                loff_t *f_pos)
{
	struct gpio_info *gpio_info_tmp = (struct gpio_info *)filp->private_data;
	char data[2];

	//printk("make: %s \n", gpio_info_tmp->pin_name);
	copy_from_user(data, buf, 2);

	data[0] = data[0] - '0';

        if (data[0] == 1 || data[0] == 0) {
                gpio_direction_output(MXS_PIN_TO_GPIO(gpio_info_tmp->pin), data[0]);
        }



	return count;
}


static ssize_t gpio_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
	struct gpio_info *gpio_info_tmp = (struct gpio_info *)file->private_data;
	int  value = 0;
	char data[3];
	static int flg	= 0;

	if (flg == 1) {
		flg = 0;
		return 0;
	}

	gpio_direction_input(MXS_PIN_TO_GPIO(gpio_info_tmp->pin));
	value = gpio_get_value(MXS_PIN_TO_GPIO(gpio_info_tmp->pin));
	data[0] = value ? 1 : 0;

	data[0] = data[0] + '0';
	data[1] = '\n';
	data[3] = -1;

	copy_to_user(buf, data, 2);
	
	flg = 1;
        return 3;
}

static int gpio_ioctl(struct inode *inode,struct file *flip,unsigned int command,unsigned long arg)
{
	struct gpio_info *gpio_info_tmp = (struct gpio_info *)flip->private_data;
	int  data = 0;
	
	switch (command) {
	case SET_GPIO_HIGHT: 
		gpio_direction_output(MXS_PIN_TO_GPIO(gpio_info_tmp->pin), 1);
		break;
	case SET_GPIO_LOW:
		gpio_direction_output(MXS_PIN_TO_GPIO(gpio_info_tmp->pin), 0);
		break;
	case GET_GPIO_VALUE:
		gpio_direction_input(MXS_PIN_TO_GPIO(gpio_info_tmp->pin));
		data = gpio_get_value(MXS_PIN_TO_GPIO(gpio_info_tmp->pin));
		data = data ? 1 : 0;
		copy_to_user((void *)arg, (void *)(&data), sizeof(int));
		break;
	default:
		printk("cmd error \n");
		
		return -1;
	}

	return 0;
}


static struct file_operations gpio_fops={
	.owner		= THIS_MODULE,
	.open 		= gpio_open,
	.write		= gpio_write,
	.read		= gpio_read,
	.release	= gpio_release,
	.ioctl		= gpio_ioctl,
};

	

static int __init gpio_init(void)
{
	int i = 0;
	int ret = 0;

	all_gpios_info = all_gpios_info_283;

	for (i = 0; all_gpios_info[i].pin != 0; i++) {
		all_gpios_info[i].pmiscdev = kmalloc(sizeof(struct miscdevice), GFP_KERNEL);
		if (all_gpios_info[i].pmiscdev == NULL) {
			printk("unable to malloc memory \n");
			return -1;
		}

		
		memset(all_gpios_info[i].pmiscdev, 0, sizeof(struct miscdevice));
		all_gpios_info[i].pmiscdev->name  = all_gpios_info[i].pin_name;
		all_gpios_info[i].pmiscdev->fops  = &gpio_fops;	
		all_gpios_info[i].pmiscdev->minor = MISC_DYNAMIC_MINOR;

		ret = misc_register(all_gpios_info[i].pmiscdev);
		if (ret) {
			printk("misc regist faile \n");
			return -1;
		}

		gpio_info_file[all_gpios_info[i].pmiscdev->minor] = &(all_gpios_info[i]);
		
		printk("build device i:%d dev:/dev/%s \n", i, all_gpios_info[i].pmiscdev->name);
	}
		printk("zlg EasyARM-imx283 gpio driver up. \n");


	return 0;
}

static void __exit gpio_exit(void)
{
	int i = 0;

	for (i = 0; all_gpios_info[i].pin != 0; i++) {
		misc_deregister(all_gpios_info[i].pmiscdev);
	}
	printk("zlg EasyARM-imx28xx gpio driver down.\n");
}

module_init(gpio_init);
module_exit(gpio_exit);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("zhuguojun, ZhiYuan Electronics Co, Ltd.");
MODULE_DESCRIPTION("GPIO DRIVER FOR MAGICARM270.");

