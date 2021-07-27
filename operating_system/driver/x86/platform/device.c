#include <linux/module.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/ioport.h>

static struct resource beep_resource[] =
	{
		[0] = {
			.start = 0x7F0080A0,
			.end = 0x7F0080A0 + 0x4,
			.flags = IORESOURCE_MEM,
		},

		[1] = {
			.start = 0x7F006000,
			.end = 0x7F006000 + 0x4,
			.flags = IORESOURCE_MEM,
		}};

static void hello_release(struct device *dev)
{
	printk("hello_release\n");
	return;
}

static struct platform_device hello_device =
	{
		.name = "bigbang",
		.id = -1,
		.dev.release = hello_release,
		.num_resources = ARRAY_SIZE(beep_resource),
		.resource = beep_resource,
};

static int hello_init(void)
{
	printk("hello_init");
	return platform_device_register(&hello_device);
}

static void hello_exit(void)
{
	printk("hello_exit");
	platform_device_unregister(&hello_device);
	return;
}

MODULE_LICENSE("GPL");
module_init(hello_init);
module_exit(hello_exit);
