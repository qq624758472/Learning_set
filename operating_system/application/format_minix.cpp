/*
 * @Description: format minix
 * @Version: 2.0
 * @Autor: lsh
 * @Date: 2021-11-03 20:06:40
 * @LastEditors: lsh
 * @LastEditTime: 2021-11-14 17:01:08
 */
#include <sys/param.h>
#include "format_minix.h"

#define MINIX_SUPER_MAGIC    0x137F          /* minix V1 fs, 14 char names */
#define MINIX_SUPER_MAGIC2   0x138F          /* minix V1 fs, 30 char names */


char *super_block_buffer;
static char boot_block_buffer[512];
#define Super (*(struct minix_super_block *) super_block_buffer)
static char *inode_map;
static char *zone_map;
static char *inode_buffer = NULL;
#define Inode (((struct minix_inode *)inode_buffer) - 1)


#define mark_zone(x) (setbit(zone_map, (x)-Super.s_firstdatazone + 1))
#define unmark_zone(x) (clrbit(zone_map, (x)-Super.s_firstdatazone + 1))
#define mark_inode(x) (setbit(inode_map, (x)))
#define unmark_inode(x) (clrbit(inode_map, (x)))


static inline void *xmalloc(const size_t size)
{
	void *ret = malloc(size);

	if (!ret && size)
		printf_err("xmalloc");
	return ret;
}

static inline void *xcalloc(const size_t nelems, const size_t size)
{
	void *ret = calloc(nelems, size);

	if (!ret && size && nelems)
		printf_err("xcalloc");
	return ret;
}

inline int is_same_inode(const int fd, const struct stat *st)
{
	struct stat f;

	if (fstat(fd, &f) < 0)
		return 0;
	else if (f.st_dev != st->st_dev || f.st_ino != st->st_ino)
		return 0;
	return 1;
}


int blkdev_is_misaligned(int fd)
{
	int aligned;

	// if (ioctl(fd, BLKALIGNOFF, &aligned) < 0)
	// 	return 0;			/* probably kernel < 2.6.32 */
	// /*
	//  * Note that kernel returns -1 as alignment offset if no compatible
	//  * sizes and alignments exist for stacked devices
	//  */
	return aligned != 0 ? 1 : 0;
}

int open_blkdev_or_file(const struct stat *st, const char *name, const int oflag)
{
	int fd;

	if (S_ISBLK(st->st_mode)) {//是否是一个块设备
		fd = open(name, oflag | O_EXCL);
	} else
		fd = open(name, oflag);
	if (-1 < fd && !is_same_inode(fd, st)) {
		close(fd);
		errno = EBADFD;
		return -1;
	}
	if (-1 < fd && S_ISBLK(st->st_mode) && blkdev_is_misaligned(fd))
		printf_err(name);
	return fd;
}

static void determine_device_blocks(struct fs_control *ctl, const struct stat *statbuf)
{
	unsigned long long dev_blocks = 0;
	
	if(S_ISBLK(statbuf->st_mode))//路径文件为块设备
	{

	}
	else if(!S_ISBLK(statbuf->st_mode))//路径文件非块设备
	{
		dev_blocks = statbuf->st_size / MINIX_BLOCK_SIZE;
		printf("%s,%d :%d\n",__FUNCTION__,__LINE__,dev_blocks);
	}

	ctl->fs_blocks = dev_blocks;
	if(ctl->fs_blocks < 10)
		printf_err("ctl->fs_blocks < 10");
		
	if(ctl->fs_blocks > MINIX_MAX_INODES)
		ctl->fs_blocks = MINIX_MAX_INODES;


	if (ctl->fs_blocks > (4 + ((MINIX_MAX_INODES - 4) * BITS_PER_BLOCK))) //BITS_PER_BLOCK: 8192
		ctl->fs_blocks = 4 + ((MINIX_MAX_INODES - 4) * BITS_PER_BLOCK);
}


static void super_set_map_blocks(const struct fs_control *ctl, unsigned long inodes)
{
	Super.s_imap_blocks = UPPER(inodes + 1, BITS_PER_BLOCK);
	Super.s_zmap_blocks = UPPER(ctl->fs_blocks - (1 + Super.s_imap_blocks + UPPER(Super.s_ninodes, MINIX_INODES_PER_BLOCK)), BITS_PER_BLOCK + 1);
	Super.s_firstdatazone = 2 + Super.s_imap_blocks + Super.s_zmap_blocks + UPPER(Super.s_ninodes, MINIX_INODES_PER_BLOCK);
}

static inline off_t first_zone_data(void)
{
	return (2 + Super.s_imap_blocks + Super.s_zmap_blocks + UPPER(Super.s_ninodes, MINIX_INODES_PER_BLOCK));
}


static void setup_tables(const struct fs_control *ctl)
{
	unsigned long inodes, zmaps, imaps, zones, i;

	super_block_buffer = (char *)xcalloc(1, MINIX_BLOCK_SIZE);

	memset(boot_block_buffer, 0, 512);
	Super.s_magic = ctl->fs_magic; //分区魔数
	Super.s_log_zone_size = 0;//硬盘个数/区块数


	Super.s_max_size = (7 + 512 + 512 * 512) * 1024;//设置最大文件长度
	Super.s_nzones = ctl->fs_blocks;//设置fs_block  分区的区块数
	zones = Super.s_nzones;

	/* some magic nrs: 1 inode / 3 blocks for smaller filesystems,
	 * for one inode / 16 blocks for large ones. mkfs will eventually
	 * crab about too far when getting close to the maximum size. 
	
	* 一些神奇的nrs:1 inode/3块用于较小的文件系统，
	*对于一个索引节点/16个块用于大型索引节点。mkfs最终将
	*当接近最大尺寸时，螃蟹离得太远。
	*/
	if (2048 * 1024 < ctl->fs_blocks) /* 2GB */
		inodes = ctl->fs_blocks / 16;
	else if (512 * 1024 < ctl->fs_blocks) /* 0.5GB */
		inodes = ctl->fs_blocks / 8;
	else
		inodes = ctl->fs_blocks / 3; //65535/3= 21845

	/* Round up inode count to fill block size */
	inodes = ((inodes + MINIX_INODES_PER_BLOCK - 1) & ~(MINIX_INODES_PER_BLOCK - 1));

	if (inodes > MINIX_MAX_INODES)
		inodes = MINIX_MAX_INODES;
	Super.s_ninodes = inodes;

	super_set_map_blocks(ctl, inodes);
	if (MINIX_MAX_INODES < first_zone_data())
		printf_err("zone long");

	imaps = Super.s_imap_blocks;
	zmaps = Super.s_zmap_blocks;

	inode_map = (char *)xmalloc(imaps * MINIX_BLOCK_SIZE);
	zone_map = (char *)xmalloc(zmaps * MINIX_BLOCK_SIZE);
	memset(inode_map, 0xff, imaps * MINIX_BLOCK_SIZE);
	memset(zone_map, 0xff, zmaps * MINIX_BLOCK_SIZE);

	for (i = Super.s_firstdatazone; i < zones; i++)
		unmark_zone(i);//bitmap 置位宏。
	for (i = MINIX_ROOT_INO; i <= inodes; i++)
		unmark_inode(i);

	
	inode_buffer = (char *)xmalloc(UPPER(Super.s_ninodes, MINIX_INODES_PER_BLOCK) * MINIX_BLOCK_SIZE);
	memset(inode_buffer, 0, UPPER(Super.s_ninodes, MINIX_INODES_PER_BLOCK) * MINIX_BLOCK_SIZE);

	// printf(P_("%lu inode\n", "%lu inodes\n", inodes), inodes);
	// printf(P_("%lu block\n", "%lu blocks\n", zones), zones);
	// printf(_("Firstdatazone=%jd (%jd)\n"),
	// 	   (intmax_t)get_first_zone(), (intmax_t)first_zone_data());
	// printf(_("Zonesize=%zu\n"), (size_t)MINIX_BLOCK_SIZE << get_zone_size());
	// printf(_("Maxsize=%zu\n\n"), get_max_size());
}





void format_minix(char *filePath)
{
    struct stat statbuf;
    struct fs_control ctl = {
		.fs_namelen = 30, /* keep in sync with DEFAULT_FS_VERSION */
		0};

    ctl.device_name = filePath;
    ctl.fs_dirsize = ctl.fs_namelen + 2;
    ctl.fs_magic = MINIX_SUPER_MAGIC2;

    //获取该设备，或者文件的状态
	if (stat(ctl.device_name, &statbuf) < 0)
		printf_err(ctl.device_name);
    

    ctl.device_fd = open_blkdev_or_file(&statbuf, ctl.device_name, O_RDWR);
	if (ctl.device_fd < 0)
		printf_err("open_blkdev_or_file");

	determine_device_blocks(&ctl, &statbuf); //确定设备块的个数 ；文件系统的设备块数

	//设置bitmap 中的位
	setup_tables(&ctl);
}