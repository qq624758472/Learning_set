/*
 * @Description: 
 * @Version: 2.0
 * @Autor: lsh
 * @Date: 2021-11-03 20:07:12
 * @LastEditors: lsh
 * @LastEditTime: 2021-11-14 16:54:53
 */

#ifndef _FORMAT_MINIX
#define _FORMAT_MINIX

#include <fstream>
#include <cstring>
#include <iostream>


#include <sys/types.h>
#include <sys/stat.h>


#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <string.h> 


#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdlib.h>
#include <termios.h>
#include <sys/stat.h>
#include <getopt.h>
#include <err.h>

#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdlib.h>
#include <termios.h>
#include <sys/stat.h>
#include <getopt.h>
#include <err.h>
using namespace std;

struct minix_inode {
	uint16_t i_mode;
	uint16_t i_uid;
	uint32_t i_size;
	uint32_t i_time;
	uint8_t  i_gid;
	uint8_t  i_nlinks;
	uint16_t i_zone[9];//数据块的指针
};


#define printf_err(a) printf("%s:%d   INFO:%s\n",__FUNCTION__, __LINE__,a)
#define MINIX_BLOCK_SIZE_BITS 10
#define MINIX_BLOCK_SIZE     (1 << MINIX_BLOCK_SIZE_BITS)//1024
#define MINIX_MAX_INODES 65535
#define BITS_PER_BLOCK (MINIX_BLOCK_SIZE << 3)
#define MINIX_INODES_PER_BLOCK ((MINIX_BLOCK_SIZE)/(sizeof (struct minix_inode)))
#define UPPER(size,n) ((size+((n)-1))/(n))


#define MINIX_ROOT_INO 1

struct minix_super_block {
	uint16_t s_ninodes; 		//i节点数
	uint16_t s_nzones;			//区块数
	uint16_t s_imap_blocks;		//i节点位图所占块数
	uint16_t s_zmap_blocks;		//区块位图所占块数
	uint16_t s_firstdatazone;	//数据区中第一个逻辑块块号
	uint16_t s_log_zone_size;	//磁盘块数/区块数
	uint32_t s_max_size;		//最大文件长度
	uint16_t s_magic;			//魔数
	uint16_t s_state;			
	uint32_t s_zones;
};

struct fs_control
{
	char *device_name;			  /* device on a Minix file system is created */
	int device_fd;				  /* open file descriptor of the device */
	char *lockmode;				  /* as specified by --lock */
	unsigned long long fs_blocks; /* device block count for the file system */
	int fs_used_blocks;			  /* used blocks on a device */
	int fs_bad_blocks;			  /* number of bad blocks found from device */
	uint16_t fs_namelen;		  /* maximum length of filenames 文件名的最大长度*/
	size_t fs_dirsize;			  /* maximum size of directory */
	unsigned long fs_inodes;	  /* number of inodes */
	int fs_magic;				  /* file system magic number */
	unsigned int
		check_blocks : 1; /* check for bad blocks */
};





void format_minix(char *filePath);

#endif