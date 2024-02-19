/*
 * @Description:
 * @Version: 2.0
 * @Autor: ruog__
 * @Date: 2024-02-19 11:25:48
 * @LastEditors: ruog__
 * @LastEditTime: 2024-02-19 11:30:00
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/mod_devicetable.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
struct resource *res;
int irqno;
struct gpio_desc *gpiono;
int pdrv_probe(struct platform_device *pdev)
{
    res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    if (res == NULL)
    {
        return ENODATA;
    }
    irqno = platform_get_irq(pdev, 0);
    if (irqno < 0)
    {
        return ENODATA;
    }
    printk("addr:%#x,irqno:%d\n", res->start, irqno);
    gpiono = gpiod_get_from_of_node(pdev->dev.of_node, "myled1", 0, GPIOD_OUT_HIGH, 0);
    if (IS_ERR(gpiono))
    {
        printk("ªÒ»°gpio±‡∫≈ ß∞‹\n");
        return PTR_ERR(gpiono);
    }
    gpiod_set_value(gpiono, 1);
    return 0;
}
int pdrv_remove(struct platform_device *pdev)
{
    gpiod_set_value(gpiono, 0);
    gpiod_put(gpiono);
    printk("%s:%d\n", __func__, __LINE__);
    return 0;
}
struct of_device_id oftable[] = {
    {
        .compatible = "hqyj,platform",
    },
    {}};
struct platform_driver pdrv = {
    .probe = pdrv_probe,
    .remove = pdrv_remove,
    .driver = {
        .name = "aaaaa",
        .of_match_table = oftable,
    },

};
module_platform_driver(pdrv);
MODULE_LICENSE("GPL");