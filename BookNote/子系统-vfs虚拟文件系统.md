# vfs虚拟文件系统

# minix文件系统

## minix文件系统挂载

```c
// 每一种注册了的文件系统都由一个类型为file_system_type的结构体来代表，
// 该结构体中含有一个类型为file_system_type*的域next，
// linux正是通过这个next域把所有注册了的文件系统连接起来的，
// 同时，linux内核还定义了一个指向链表中第一个元素的全局指针file_systems
// 和一个用来用来防止并发访问该链表的读/写自旋锁file_systems_lock。
static struct file_system_type minix_fs_type = {
    .owner        = THIS_MODULE,                //对于内部 VFS 使用：在大多数情况下，您应该将其初始化为 THIS_MODULE。
    .name        = "minix",
    .mount        = minix_mount,                //应挂载此文件系统的新实例时调用的方法
    .kill_sb    = kill_block_super,            //关闭此文件系统的实例时调用的方法
    .fs_flags    = FS_REQUIRES_DEV,            //各种标志（即 FS_REQUIRES_DEV、FS_NO_DCACHE 等）
};
```

minix_mount回调函数：

```c
/**
 * @fs_type:        描述文件系统
 * @flags:            安装标志
 * @dev_name:        我们正在安装的设备名称; /dev/loop*   /dev/sda  这些。
 * @data:            任意挂载选项，通常以 ASCII 字符串形式出现（参见“挂载选项”部分）
 * 
 * @return:         返回调用者请求的树的根目录项。必须获取对其超级块的活动引用，并且必须锁定超级块。失败时它应该返回 ERR_PTR(error)。
 * Usually, a filesystem uses one of the generic mount() implementations and provides a fill_super() callback instead. The generic variants are:
 * mount_bdev: mount a filesystem residing on a block device
*/
static struct dentry *minix_mount(struct file_system_type *fs_type, int flags, const char *dev_name, void *data)
{
    /*
    * mount_bdev是针对块设备挂载时使用的函数，此外还有mount_nodev, mount_single等函数，分别用于不同的挂载情况
    * 过载一次,调用一次.
    */
    printk("Author:%s | This is minix mount operation\ndev_name:%s ,data:%s\n",AUTHOR,dev_name,data);
    /**
     * 该函数用数据填充一个超级块对象，如果内存中没有适当的超级块对象，数据就必须从硬盘读取
    */
    return mount_bdev(fs_type, flags, dev_name, data, minix_fill_super);
}
```

mount_bdev函数功能分析：

```c
struct dentry *mount_bdev(struct file_system_type *fs_type,
    int flags, const char *dev_name, void *data,
    int (*fill_super)(struct super_block *, void *, int))
{
    //主要函数
    bdev = blkdev_get_by_path();
    s = sget();//find or create a superblock
    if (s->s_root) {//如果已经挂载过
        up_write(&s->s_umount);
        blkdev_put(bdev, mode);//释放获取到的bdev结构体
        down_write(&s->s_umount);
    } else {
        //粗略统计，设置sb的以下数据。
        s->s_mode;
        s->s_id;
        sb->s_blocksize;
        fill_super();//此fill_super指向 minix_fill_super()
            //这些由用户定义， 需要填充哪些超级块中的数据.
            sb->s_flags |= SB_NOSEC;
            sb->s_magic = GFS2_MAGIC;
            sb->s_op = &gfs2_super_ops;
            sb->s_d_op = &gfs2_dops;
            sb->s_export_op = &gfs2_export_ops;
            sb->s_xattr = gfs2_xattr_handlers;
            sb->s_qcop = &gfs2_quotactl_ops;
            sb->s_quota_types = QTYPE_MASK_USR | QTYPE_MASK_GRP;
            sb_dqopt(sb)->flags |= DQUOT_QUOTA_SYS_FILE;
            sb->s_time_gran = 1;
            sb->s_maxbytes = MAX_LFS_FILESIZE;
        s->s_flags;
        bdev->bd_super = s;//将该硬盘的->超级块->指向s;  将超级块放入bdev的结构体中.
    }
}
```

带此处就已经挂载完成。

由此可见，挂载的过程就是通过文件系统模块的fill_super()函数将对应的数据放入到超级块中。









## minix文件系统的格式化


