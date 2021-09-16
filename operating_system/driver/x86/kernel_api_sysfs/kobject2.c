#include <linux/init.h>
#include <linux/module.h>
#include <linux/kobject.h>
#include <linux/slab.h>
MODULE_LICENSE("GPL v2");

static struct kobject *testKobj = NULL;

static void test_sysfs_release(struct kobject *kobj)
{
    printk(KERN_INFO "test sysfs release\n");
    kfree(kobj);
}

static struct kobj_type test_type = {
    //.sysfs_ops = &test_sysfs_ops,
    .release = test_sysfs_release,
    // .default_groups = test_sys_groups,
};
static int __init sysfs_test_init(void)
{
    int ret = 0;
    testKobj = kzalloc(sizeof(struct kobject), GFP_KERNEL);
    if (!testKobj)
    {
        printk(KERN_INFO "kzalloc kobject failed\n");
        return 1;
    }

    ret = kobject_init_and_add(testKobj, &test_type, NULL, "test1");//初始化kobject，并将其加入到sysfs文件系统中
    if (ret)
    {
        printk(KERN_INFO "int and add kbject failed\n");
        return 1;
    }

    return 0;
}

static void __exit sysfs_test_exit(void)
{
    kobject_put(testKobj);
}

module_init(sysfs_test_init);
module_exit(sysfs_test_exit);
MODULE_AUTHOR("hzk");
MODULE_DESCRIPTION("A simple moudule for sysft test");
MODULE_VERSION("V1.0");