#include <linux/init.h>
#include <linux/module.h>
#include <linux/kobject.h>
#include <linux/string.h>
#include <linux/stat.h>
#include <linux/slab.h>
#include <linux/sysfs.h>
#include <linux/device.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
MODULE_LICENSE("GPL v2");
 
#define MY_PROC "my_proc"
 
static struct proc_dir_entry * my_proc_entry=NULL;
static DEFINE_MUTEX(my_proc_lock);
static int my_proc_1_data=1;
static int my_proc_2_data=2;
 
 
static int my_proc_1_show(struct seq_file *s, void *v)
{
    int * value = s->private;
    seq_printf(s, "proc_1 value:%d\n", *value);
    return 0;
}
 
static int my_proc_2_show(struct seq_file *s, void *v)
{
    int * value = s->private;
    seq_printf(s, "proc_2 value:%d\n", *value);
    return 0;
}
 
static int __init my_proc_init(void)
{
   my_proc_entry = proc_mkdir(MY_PROC, NULL);
   if (NULL == my_proc_entry)
   {
       return -ENODEV;
   }
 
    proc_create_single_data("proc_1", 0, my_proc_entry, my_proc_1_show, &my_proc_1_data);
    proc_create_single_data("proc_2", 0, my_proc_entry, my_proc_2_show, &my_proc_2_data);
    return 0;
}
 
static void __exit my_proc_exit(void)
{
   remove_proc_entry(MY_PROC, NULL);
}
 
module_init(my_proc_init);
module_exit(my_proc_exit);
MODULE_AUTHOR("hzk");
MODULE_DESCRIPTION("A simple moudule for my proc");
MODULE_VERSION("V1.0");