#include <linux/init.h>
#include <linux/module.h>
#include <linux/kobject.h>
#include <linux/string.h>
#include <linux/stat.h>
#include <linux/slab.h>
#include <linux/sysfs.h>
MODULE_LICENSE("GPL v2");
 
static struct kobject * testKobj = NULL;
static struct kobject * testkobj_hello = NULL;
 
 
static ssize_t sysfs_test_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    return sprintf(buf,"hello world\n");
}
 
static ssize_t sysfs_test_store(struct kobject *kobj, struct kobj_attribute * attr, const char * buf, size_t count)
{
 
    printk(KERN_INFO "sys test module recv: %s\n", buf);
    return count;
}
static struct kobj_attribute testName =
    __ATTR(name, 0664,sysfs_test_show,sysfs_test_store);
 
static struct kobj_attribute testPara =
    __ATTR(para, 0664,sysfs_test_show,sysfs_test_store);
static struct attribute *testAttr[] = {
    &testName.attr,
    &testPara.attr,
    NULL,
};
static struct attribute_group testGroup = {
    .attrs = testAttr,
};
static int __init sysfs_test_init(void)
{
    testKobj = kobject_create_and_add("test2", NULL);
    if (!testKobj)
    {
        printk(KERN_INFO "create test kbject failed\n");
        return 1;
    }
 
    testkobj_hello = kobject_create_and_add("hello", testKobj);
    if (!testkobj_hello)
    {
        printk(KERN_INFO "create test hello kbject failed\n");
        return 1;
    }
 
    sysfs_create_group(testkobj_hello, &testGroup);
    return 0;
}
 
static void __exit sysfs_test_exit(void)
{
    sysfs_remove_group(testkobj_hello,&testGroup);
    kobject_put(testkobj_hello);
    kobject_put(testKobj);
}
 
module_init(sysfs_test_init);
module_exit(sysfs_test_exit);
MODULE_AUTHOR("hzk");
MODULE_DESCRIPTION("A simple moudule for sysft test");
MODULE_VERSION("V1.0");