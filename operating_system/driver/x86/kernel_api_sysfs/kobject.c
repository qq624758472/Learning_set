#include <linux/init.h>
#include <linux/module.h>
#include <linux/kobject.h>
MODULE_LICENSE("GPL v2");

static struct kobject *testKobj = NULL;
static struct kobject *testKobj2 = NULL;
static int __init sysfs_test_init(void)
{
    testKobj = kobject_create_and_add("lshtest", NULL);
    if (!testKobj)
    {
        printk(KERN_INFO "create test kbject failed\n");
        return 1;
    }

    testKobj2 = kobject_create_and_add("lsh", testKobj);
    if (!testKobj)
    {
        printk(KERN_INFO "create lsh kbject failed\n");
        return 1;
    }
    return 0;
}

static void __exit sysfs_test_exit(void)
{
    kobject_put(testKobj);
    kobject_put(testKobj2);
}

module_init(sysfs_test_init);
module_exit(sysfs_test_exit);
MODULE_AUTHOR("hzk");
MODULE_DESCRIPTION("A simple moudule for sysft test");
MODULE_VERSION("V1.0");
