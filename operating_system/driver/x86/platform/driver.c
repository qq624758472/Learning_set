#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/cdev.h>
#include <linux/fs.h>

/*
 * Yao.GUET
 * http://blog.csdn.net/Yao_GUET
 * a simple platform character driver
 */

MODULE_LICENSE("Dual BSD/GPL");

static int chrdev_open(struct inode *inode, struct file *file)
{
    printk(KERN_ALERT "chrdev open!\n");
    return 0;
}

static int chrdev_release(struct inode *inode, struct file *file)
{
    printk(KERN_ALERT "chrdev release!\n");
    return 0;
}

//新旧版本的两种ioctl
static long chrdev_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
// static long chrdev_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)
{

    printk(KERN_ALERT "chrdev release!\n");
    return 0;
}

// Kernel interface
static struct file_operations chrdev_fops = {
    .owner = THIS_MODULE,
    //.ioctl      =   chrdev_ioctl,
    .unlocked_ioctl = chrdev_ioctl,
    .open = chrdev_open,
    .release = chrdev_release,
};

#define CHRDEV_NAME "chrdev"
// driver interface
static struct class *chrdev_class = NULL;
static struct device *chrdev_device = NULL;
static dev_t chrdev_devno;
static struct cdev chrdev_cdev;

static int chrdev_probe(struct platform_device *dev)
{
    int ret = 0, err = 0;

    printk(KERN_ALERT "chrdev probe!\n");

    // alloc character device number
    ret = alloc_chrdev_region(&chrdev_devno, 0, 1, CHRDEV_NAME);
    if (ret)
    {
        printk(KERN_ALERT " alloc_chrdev_region failed!\n");
        goto PROBE_ERR;
    }
    printk(KERN_ALERT " major:%d minor:%d\n", MAJOR(chrdev_devno), MINOR(chrdev_devno));

    cdev_init(&chrdev_cdev, &chrdev_fops);
    chrdev_cdev.owner = THIS_MODULE;
    // add a character device
    err = cdev_add(&chrdev_cdev, chrdev_devno, 1);
    if (err)
    {
        printk(KERN_ALERT " cdev_add failed!\n");
        goto PROBE_ERR;
    }

    // create the device class
    chrdev_class = class_create(THIS_MODULE, CHRDEV_NAME);
    if (IS_ERR(chrdev_class))
    {
        printk(KERN_ALERT " class_create failed!\n");
        goto PROBE_ERR;
    }

    // create the device node in /dev
    chrdev_device = device_create(chrdev_class, NULL, chrdev_devno,
                                  NULL, CHRDEV_NAME);
    if (NULL == chrdev_device)
    {
        printk(KERN_ALERT " device_create failed!\n");
        goto PROBE_ERR;
    }

    printk(KERN_ALERT " chrdev probe ok!\n");
    return 0;

PROBE_ERR:
    if (err)
        cdev_del(&chrdev_cdev);
    if (ret)
        unregister_chrdev_region(chrdev_devno, 1);
    return -1;
}

static int chrdev_remove(struct platform_device *dev)
{
    printk(KERN_ALERT " chrdev remove!\n");

    cdev_del(&chrdev_cdev);
    unregister_chrdev_region(chrdev_devno, 1);

    device_destroy(chrdev_class, chrdev_devno);
    class_destroy(chrdev_class);
    return 0;
}

// platform_device and platform_driver must has a same name!
// or it will not work normally
static struct platform_driver chrdev_platform_driver = {
    .probe = chrdev_probe,
    .remove = chrdev_remove,
    .driver = {
        .name = CHRDEV_NAME,
        .owner = THIS_MODULE,
    },
};

static struct platform_device chrdev_platform_device = {
    .name = CHRDEV_NAME,
    .id = 0,
    .dev = {}};

static __init int chrdev_init(void)
{
    int ret = 0;
    printk(KERN_ALERT "chrdev init!\n");

    ret = platform_device_register(&chrdev_platform_device);
    if (ret)
    {
        printk(KERN_ALERT " platform_device_register failed!\n");
        return ret;
    }

    ret = platform_driver_register(&chrdev_platform_driver);
    if (ret)
    {
        printk(KERN_ALERT " platform_driver_register failed!\n");
        return ret;
    }
    printk(KERN_ALERT " chrdev_init ok!\n");
    return ret;
}

static __exit void chrdev_exit(void)
{
    printk(KERN_ALERT "chrdev exit!\n");
    platform_driver_unregister(&chrdev_platform_driver);
}

module_init(chrdev_init);
module_exit(chrdev_exit);