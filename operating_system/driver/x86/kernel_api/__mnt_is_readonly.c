#include <linux/init.h>
# include <linux/module.h>
# include <linux/fs.h>
# include <linux/fs_struct.h>
# include <linux/path.h>
# include <linux/sched.h>
# include <linux/fdtable.h>
# include <linux/mount.h>
# include <linux/seq_file.h>

//文件系统相关的访问
#include <linux/syscalls.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/kmod.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

#include <linux/fs.h>
#include <linux/magic.h>
#include <linux/mount.h>
#include <linux/init.h>
#include <linux/user_namespace.h>

#include "sysfs.h"

int __mnt_is_readonly_init(void)
{
#if 0
    int data1,data2;
    struct  vfsmount *mnt;
    mnt = current->fs->pwd.mnt;
    data1=__mnt_is_readonly(mnt);
    if(data1)
        printk("mntfs is read only\n");
    else
        printk("mntfs is not read only\n");
    data2 = mnt_want_write(mnt);
    if(data2)
        printk("mntfs is en_write\n");
    else
        printk("mntfs is not en_write\n");
#endif

#if 0
    struct file_system_type *type;
    type = get_fs_type("sysfs");
    if (!type)
    {
        printk("Can't find rootfs type");
    }
    else
    {
        printk("fs name:%s\n",type->name);
    }

#endif

#if 1
    struct kernfs_root *sysfs_root;
    sysfs_root = kernfs_create_root(NULL, KERNFS_ROOT_EXTRA_OPEN_PERM_CHECK,
					NULL);
	if (IS_ERR(sysfs_root))
		return PTR_ERR(sysfs_root);
#endif
    
    return 0;
}

void __mnt_is_readonly_exit(void)
{
    printk("<0>Goodbye \n");
}


module_init(__mnt_is_readonly_init);
module_exit(__mnt_is_readonly_exit);
MODULE_LICENSE ("GPL");