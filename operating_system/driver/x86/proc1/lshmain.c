/*
 * @Description:
 * @Version: 2.0
 * @Autor: ruog__
 * @Date: 2022-06-20 09:41:05
 * @LastEditors: ruog__
 * @LastEditTime: 2022-06-20 14:42:55
 */
#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/sched/signal.h>
#include <linux/kthread.h>
#include <linux/blkdev.h>
#include <linux/badblocks.h>
#include <linux/sysctl.h>
#include <linux/seq_file.h>
#include <linux/fs.h>
#include <linux/poll.h>
#include <linux/ctype.h>
#include <linux/string.h>
#include <linux/hdreg.h>
#include <linux/proc_fs.h>
#include <linux/random.h>
#include <linux/module.h>
#include <linux/reboot.h>
#include <linux/file.h>
#include <linux/compat.h>
#include <linux/delay.h>
#include <linux/raid/md_p.h>
#include <linux/raid/md_u.h>
#include <linux/slab.h>
#include <linux/percpu-refcount.h>

#include <trace/events/block.h>

static struct proc_dir_entry *proc_parent;

static int seq_file_demo_show(struct seq_file *seq, void *v)
{
    seq_printf(seq, "Hello World\n");
    return 0;
}

static int seq_file_demo_open(struct inode *inode, struct file *file)
{
    return single_open(file, &seq_file_demo_show, NULL);
}

static const struct file_operations md_seq_fops = {
    .owner = THIS_MODULE,
    .open = seq_file_demo_open,
    .read = seq_read,
    .llseek = seq_lseek,
    .release = single_release,
};

static __init int chrdev_init(void)
{
    proc_parent = proc_mkdir("hello", NULL);
    if (!proc_parent)
    {
        printk("proc mkdir error\n");
    }
    proc_create("lshmain", 0644, proc_parent, &md_seq_fops);
}

static __exit void chrdev_exit(void)
{
}

module_init(chrdev_init);
module_exit(chrdev_exit);
MODULE_LICENSE("Dual BSD/GPL");