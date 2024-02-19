/*
 * @Description: 
 * @Version: 2.0
 * @Autor: ruog__
 * @Date: 2023-03-21 09:46:49
 * @LastEditors: ruog__
 * @LastEditTime: 2024-02-19 11:09:38
 */
#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>
static int irq;
static char *interface;
// MODULE_PARM_DESC(interface,"A network interface");  2.4内核中该宏的用法
molule_parm(interface, charp, 0644); // 2.6内核中的宏
// MODULE_PARM_DESC(irq,"The IRQ of the network interface");
module_param(irq, int, 0644);

static irqreturn_t myinterrupt(int irq, void *dev_id, struct pt_regs *regs)
{
    static int mycount = 0;
    if (mycount < 10)
    {
        printk("Interrupt !\n");
        mycount++;
    }
    return IRQ_NONE;
}
static int __init myirqtest_init(void)
{
    printk("My module worked !11111\n");
    if (request_irq(irq, &myinterrupt, SA_SHIRQ, interface, &irq))
    {
        printk(KERN_ERR "myirqtest: cannot register IRQ % d\n", irq);
        return -EIO;
    }
    printk("% s Request on IRQ % d succeeded\n", interface, irq);
    return 0;
}
static void __exit myirqtest_exit(void)
{
    printk("Unloading my module.\n");
    free_irq(irq, &irq);
    printk("Freeing IRQ % d\n", irq);
    return;
}
module_init(myirqtest_init);
module_exit(myirqtest_exit);
MODULE_LICENSE("GPL");