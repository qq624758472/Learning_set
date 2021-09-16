#include <linux/init.h>
#include <linux/module.h>
#include <linux/kobject.h>
#include <linux/string.h>
#include <linux/stat.h>
#include <linux/slab.h>
#include <linux/sysfs.h>
MODULE_LICENSE("GPL v2");

static struct kobject *testKsetKobj = NULL;

static ssize_t test_attr_show(struct kobject *kobj, struct attribute *attr, char *buf)
{
    struct kobj_attribute *obj_attr = NULL;

    obj_attr = container_of(attr, struct kobj_attribute, attr);

    if (!obj_attr->show)
    {
        return 0;
    }

    return obj_attr->show(kobj, obj_attr, buf);
}

static ssize_t test_attr_store(struct kobject *kobj, struct attribute *attr, const char *buf, size_t len)
{
    struct kobj_attribute *obj_attr = NULL;

    obj_attr = container_of(attr, struct kobj_attribute, attr);

    if (!obj_attr->store)
    {
        return 0;
    }

    return obj_attr->store(kobj, obj_attr, buf, len);
}

static const struct sysfs_ops test_sysfs_ops = {
    .show = test_attr_show,
    .store = test_attr_store,
};
static void test_kset_release(struct kobject *kobj)
{
    printk(KERN_INFO "test kset relsase\n");
    kfree(kobj);
}
static ssize_t sysfs_test_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    return sprintf(buf, "hello world\n");
}

static ssize_t sysfs_test_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{

    printk(KERN_INFO "sys test module recv: %s\n", buf);
    return count;
}
static struct kobj_attribute testName =
    __ATTR(name, 0664, sysfs_test_show, sysfs_test_store);

static struct kobj_attribute testPara =
    __ATTR(para, 0664, sysfs_test_show, sysfs_test_store);

static struct attribute *test_sys_attrs[] = {
    &testName.attr,
    &testPara.attr,
    NULL,
};

ATTRIBUTE_GROUPS(test_sys);

static struct kobj_type test_ktype = {
    .sysfs_ops = &test_sysfs_ops,
    .release = test_kset_release,
    .default_groups = test_sys_groups,
};
static int __init kset_test_init(void)
{
    testKsetKobj = kzalloc(sizeof(struct kobject), GFP_KERNEL);
    if (!testKsetKobj)
    {
        printk(KERN_INFO "create and add kset kobject failed\n");
        return 1;
    }

    if (kobject_init_and_add(testKsetKobj, &test_ktype, NULL, "test2"))
    {
        kfree(testKsetKobj);
        return 1;
    }

    return 0;
}
static void __exit kset_test_exit(void)
{
    kobject_put(testKsetKobj);
}

module_init(kset_test_init);
module_exit(kset_test_exit);
MODULE_AUTHOR("hzk");
MODULE_DESCRIPTION("A simple moudule for sysft test");
MODULE_VERSION("V1.0");