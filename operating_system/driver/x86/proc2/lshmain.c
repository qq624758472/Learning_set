/*
 * @Description:
 * @Version: 2.0
 * @Autor: ruog__
 * @Date: 2022-06-20 16:22:13
 * @LastEditors: ruog__
 * @LastEditTime: 2022-06-20 16:22:15
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/seq_file.h>
#include <linux/debugfs.h>
#include <linux/fs.h>
#include <linux/list.h>
#include <linux/slab.h>

static struct dentry *seq_demo_dir;
static LIST_HEAD(seq_demo_list);
static DEFINE_MUTEX(seq_demo_lock);

struct seq_demo_node
{
    char name[10];
    struct list_head list;
};

static void *seq_demo_start(struct seq_file *s, loff_t *pos)
{
    mutex_lock(&seq_demo_lock);

    return seq_list_start(&seq_demo_list, *pos);
}

static void *seq_demo_next(struct seq_file *s, void *v, loff_t *pos)
{
    return seq_list_next(v, &seq_demo_list, pos);
}

static void seq_demo_stop(struct seq_file *s, void *v)
{
    mutex_unlock(&seq_demo_lock);
}

static int seq_demo_show(struct seq_file *s, void *v)
{
    struct seq_demo_node *node = list_entry(v, struct seq_demo_node, list);

    seq_printf(s, "name: %s, addr: 0x%p\n", node->name, node);

    return 0;
}

static const struct seq_operations seq_demo_ops = {
    .start = seq_demo_start,
    .next = seq_demo_next,
    .stop = seq_demo_stop,
    .show = seq_demo_show,
};

static int seq_demo_open(struct inode *inode, struct file *file)
{
    return seq_open(file, &seq_demo_ops);
}

static const struct file_operations seq_demo_fops = {
    .owner = THIS_MODULE,
    .open = seq_demo_open,
    .read = seq_read,
    .llseek = seq_lseek,
    .release = seq_release,
};

static int __init seq_demo_init(void)
{
    int i;
    struct seq_demo_node *node;

    for (i = 0; i < 7; i++)
    {
        node = kzalloc(sizeof(struct seq_demo_node), GFP_KERNEL);
        sprintf(node->name, "node%d", i);

        INIT_LIST_HEAD(&node->list);
        list_add_tail(&node->list, &seq_demo_list);
    }

    seq_demo_dir = debugfs_create_file("seq_demo", 0444, NULL,
                                       NULL, &seq_demo_fops);
    return 0;
}

static void __exit seq_demo_exit(void)
{
    struct seq_demo_node *node_pos, *node_n;

    if (seq_demo_dir)
    {
        debugfs_remove(seq_demo_dir);
        list_for_each_entry_safe(node_pos, node_n, &seq_demo_list, list) if (node_pos)
        {
            printk("%s: release %s\n", __func__, node_pos->name);
            kfree(node_pos);
        }
    }
}

module_init(seq_demo_init);
module_exit(seq_demo_exit);
MODULE_LICENSE("GPL");