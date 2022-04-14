#include <linux/fs.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/init.h>
#include <linux/dmi.h>
#include <asm/pci_x86.h>

static int first_drv_open(struct inode *inodep, struct file *filep)
{
    printk("open\n");
    return 0;
}

static ssize_t first_drv_write(struct file *filep, const char __user *buf,size_t len, loff_t *ppos)
{
    printk("write\n");
    return 0;
}

static const struct file_operations first_drv_file_operation = {
    .owner = THIS_MODULE,
    .open = first_drv_open,
    .write = first_drv_write,
};

static int __init first_drv_init(void)
{
    printk("first_drv_init\n");

    if (!request_region(0xCF8, 8, "PCI conf1"))
    {
        printk("PCI get i/o error\n");
    }


    release_region(0xCF8, 8);
    return 0;
}

static void __exit first_drv_exit(void)
{
    printk("first_drv_exit\n");
}

module_init(first_drv_init);
module_exit(first_drv_exit);
MODULE_LICENSE("GPL");