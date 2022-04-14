#include <linux/module.h>
#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/pci.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <asm/io.h>
#include <linux/ioport.h>
#include <linux/interrupt.h>
#include <linux/irq.h>

//设备相关
#define MY_VENDOR_ID 0x168c  //厂商号
#define MY_DEVICE_ID 0x002a  //设备号
#define MY_PCI_NAME "MYPCIE" //自己起的设备名
static int debug = 1;
module_param(debug, int, S_IRUGO);
#define DBG(msg...)      \
    do                   \
    {                    \
        if (debug)       \
            printk(msg); \
    } while (0)

struct pcie_card
{
    //端口读写变量
    int io;
    long range, flags;
    void __iomem *ioaddr;
    int irq;
};
/* 设备中断服务*/
static irqreturn_t mypci_interrupt(int irq, void *dev_id)
{
    struct pcie_card *mypci = (struct pcie_card *)dev_id;
    printk("irq = %d,mypci_irq = %d\n", irq, mypci->irq);
    return IRQ_HANDLED;
}

/* 探测PCI设备*/
static int __init mypci_probe(struct pci_dev *dev, const struct pci_device_id *ent)
{
    int retval = 0; //, intport, intmask;
    struct pcie_card *mypci;

    if (pci_enable_device(dev))
    {
        printk(KERN_ERR "IO Error.\n");
        return -EIO;
    }
    /*分配设备结构*/
    mypci = kmalloc(sizeof(struct pcie_card), GFP_KERNEL);
    if (!mypci)
    {
        printk("In %s,kmalloc err!", __func__);
        return -ENOMEM;
    }
    /*设定端口地址及其范围,指定中断IRQ*/
    mypci->irq = dev->irq;
    if (mypci->irq < 0)
    {
        printk("IRQ is %d, it's invalid!\n", mypci->irq);
        goto out_mypci;
    }
    mypci->io = pci_resource_start(dev, 0);
    mypci->range = pci_resource_end(dev, 0) - mypci->io;
    mypci->flags = pci_resource_flags(dev, 0);
    DBG("PCI base addr 0 is io%s.\n", (mypci->flags & IORESOURCE_MEM) ? "mem" : "port");
    /*检查申请IO端口*/
    retval = check_region(mypci->io, mypci->range);
    if (retval)
    {
        printk(KERN_ERR "I/O %d is not free.\n", mypci->io);
        goto out_mypci;
    }
    // request_region(mypci->io,mypci->range, MY_PCI_NAME);
    retval = pci_request_regions(dev, MY_PCI_NAME);
    if (retval)
    {
        printk("PCI request regions err!\n");
        goto out_mypci;
    }
    mypci->ioaddr = ioremap(mypci->io, mypci->range);
    if (!mypci->ioaddr)
    {
        printk("ioremap err!\n");
        retval = -ENOMEM;
        goto out_regions;
    }
    //申请中断IRQ并设定中断服务子函数
    retval = request_irq(mypci->irq, mypci_interrupt, IRQF_SHARED, MY_PCI_NAME, mypci);
    if (retval)
    {
        printk(KERN_ERR "Can't get assigned IRQ %d.\n", mypci->irq);
        goto out_iounmap;
    }
    pci_set_drvdata(dev, mypci);
    DBG("Probe succeeds.PCIE ioport addr start at %X, mypci->ioaddr is 0x%p,interrupt No. %d.\n", mypci->io, mypci->ioaddr, mypci->irq);
    return 0;

out_iounmap:
    iounmap(mypci->ioaddr);
out_regions:
    pci_release_regions(dev);
out_mypci:
    kfree(mypci);
    return retval;
}

/* 移除PCI设备 */
//static void __devexit mypci_remove(struct pci_dev *dev)
static void mypci_remove(struct pci_dev *dev)
{
    struct pcie_card *mypci = pci_get_drvdata(dev);
    free_irq(mypci->irq, mypci);
    iounmap(mypci->ioaddr);
    // release_region(mypci->io,mypci->range);
    pci_release_regions(dev);
    kfree(mypci);
    DBG("Device is removed successfully.\n");
}

/* 指明驱动程序适用的PCI设备ID */
static struct pci_device_id mypci_table[] __initdata =
    {
        {
            MY_VENDOR_ID, //厂商ID
            MY_DEVICE_ID, //设备ID
            PCI_ANY_ID,   //子厂商ID
            PCI_ANY_ID,   //子设备ID
        },
        {
            0,
        },
};
MODULE_DEVICE_TABLE(pci, mypci_table);

/* 设备模块信息 */
static struct pci_driver mypci_driver_ops =
    {
        name : MY_PCI_NAME,     //设备模块名称
        id_table : mypci_table, //驱动设备表
        probe : mypci_probe,    //查找并初始化设备
        remove : mypci_remove   //卸载设备模块
    };

static int __init mypci_init(void)
{
    //注册硬件驱动程序
    if (pci_register_driver(&mypci_driver_ops))
    {
        printk(KERN_ERR "Can't register driver!\n");
        return -ENODEV;
    }
    return 0;
}

static void __exit mypci_exit(void)
{
    pci_unregister_driver(&mypci_driver_ops);
}

module_init(mypci_init);
module_exit(mypci_exit);
MODULE_LICENSE("GPL");