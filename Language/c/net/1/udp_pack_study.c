
#if 0
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <net/if.h>    //struct ifreq
#include <sys/ioctl.h> //ioctl、SIOCGIFADDR
#include <sys/socket.h>
#include <netinet/ether.h>    //ETH_P_ALL
#include <netpacket/packet.h> //struct sockaddr_ll

unsigned short checksum(unsigned short *buf, int nword); //校验和函数
int main(int argc, char *argv[])
{
    // 1.创建通信用的原始套接字
    int sock_raw_fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

    // 2.根据各种协议首部格式构建发送数据报
    unsigned char send_msg[1024] = {
        //--------------组MAC--------14------
        0xa8, 0xa1, 0x59, 0x2c, 0x82, 0x5e, // dst_mac: 74-27-EA-B5-FF-D8    a8:a1:59:2c:82:5e
        0x02, 0xf1, 0xa5, 0x79, 0x87, 0x30, // src_mac: c8:9c:dc:b7:0f:19    02:f1:a5:79:87:30
        0x08, 0x00,                         //类型：0x0800 IP协议
        //--------------组IP---------20------
        0x45, 0x00, 0x00, 0x00, //版本号：4, 首部长度：20字节, TOS:0, --总长度--：
        0x00, 0x00, 0x00, 0x00, // 16位标识、3位标志、13位片偏移都设置0
        0x80, 17, 0x00, 0x00,   // TTL：128、协议：UDP（17）、16位首部校验和
        10, 7, 14, 113,         // src_ip: 10,   7, 14, 113
        10, 7, 14, 62,          // dst_ip: 10,   7, 14,  62
        //--------------组UDP--------8+78=86------
        0x1f, 0x90, 0x1f, 0x90, // src_port:0x1f90(8080), dst_port:0x1f90(8080)
        0x00, 0x00, 0x00, 0x00, //#--16位UDP长度--30个字节、#16位校验和
    };

    int len = sprintf(send_msg + 42, "%s", "this is for the udp test");
    if (len % 2 == 1) //判断len是否为奇数
    {
        len++; //如果是奇数，len就应该加1(因为UDP的数据部分如果不为偶数需要用0填补)
    }

    *((unsigned short *)&send_msg[16]) = htons(20 + 8 + len);     // IP总长度 = 20 + 8 + len
    *((unsigned short *)&send_msg[14 + 20 + 4]) = htons(8 + len); // udp总长度 = 8 + len
    // 3.UDP伪头部
    unsigned char pseudo_head[1024] = 
    {
        //------------UDP伪头部--------12--
        10, 7, 14, 113,       // src_ip: 10,   7, 14, 113
        10, 7, 14, 62,        // dst_ip: 10,   7, 14,  62
        0x00, 17, 0x00, 0x00, // 0,17,#--16位UDP长度--20个字节
    };

    *((unsigned short *)&pseudo_head[10]) = htons(8 + len); //为头部中的udp长度（和真实udp长度是同一个值）
    // 4.构建udp校验和需要的数据报 = udp伪头部 + udp数据报
    memcpy(pseudo_head + 12, send_msg + 34, 8 + len); //--计算udp校验和时需要加上伪头部--
    // 5.对IP首部进行校验
    *((unsigned short *)&send_msg[24]) = htons(checksum((unsigned short *)(send_msg + 14), 20 / 2));
    // 6.--对UDP数据进行校验--
    *((unsigned short *)&send_msg[40]) = htons(checksum((unsigned short *)pseudo_head, (12 + 8 + len) / 2));

    // 6.发送数据
    struct sockaddr_ll sll; //原始套接字地址结构
    struct ifreq req;       //网络接口地址

    strncpy(req.ifr_name, "enp0s3f0", IFNAMSIZ);      //指定网卡名称
    if (-1 == ioctl(sock_raw_fd, SIOCGIFINDEX, &req)) //获取网络接口
    {
        perror("ioctl");
        close(sock_raw_fd);
        exit(-1);
    }

    /*将网络接口赋值给原始套接字地址结构*/
    bzero(&sll, sizeof(sll));
    sll.sll_ifindex = req.ifr_ifindex;
    len = sendto(sock_raw_fd, send_msg, 14 + 20 + 8 + len, 0, (struct sockaddr *)&sll, sizeof(sll));
    if (len <= 0)
    {
        perror("sendto");
    }
    return 0;
}

unsigned short checksum(unsigned short *buf, int nword)
{
    unsigned long sum;
    for (sum = 0; nword > 0; nword--)
    {
        sum += htons(*buf);
        buf++;
    }
    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    return ~sum;
}

#else

#define BUFFER_MAX 2048
#define PCKT_LEN 8492

#pragma pack(1)
// ethernet head 14 bytes
struct ether
{
    char dmac[6];
    char smac[6];
    char eth_typ_len[2];
};

// ip head 20 bytes
struct ipheader
{

    unsigned char iplv; // iph_ihl:5, iph_ver:4;

    unsigned char iph_tos;

    unsigned short int iph_len;

    unsigned short int iph_ident;

    // unsigned uint8_t      iph_flag;

    unsigned short int iph_offset; // 16bit include flag and offset

    unsigned char iph_ttl;

    unsigned char iph_protocol;

    unsigned short int iph_chksum;

    unsigned int iph_sourceip;

    unsigned int iph_destip;
};

// udp head 8 bytes
struct udpheader
{

    unsigned short int udph_srcport;

    unsigned short int udph_destport;

    unsigned short int udph_len;

    unsigned short int udph_chksum;
};

// tcp head 20bytes
struct tcphdr
{
    unsigned short int source;
    unsigned short int dest;
    unsigned int seq;
    unsigned int ack_seq;
    unsigned short int tcp_len;
#if 0
    unsigned short int doff:4;
    unsigned short int res1:4;
    unsigned short int res2:2;
    unsigned short int urg:1;
    unsigned short int ack:1;
    unsigned short int psh:1;
    unsigned short int rst:1;
    unsigned short int syn:1;
    unsigned short int fin:1;
#endif
    unsigned short int window;
    unsigned short int check;
    unsigned short int urg_prt;
};

#pragma pack()

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <linux/in.h>

// caculate ip checksum
unsigned short int ip_csum(unsigned short int *addr, int len)
{
    int nleft = len;
    int sum = 0;
    unsigned short int *w = addr;
    unsigned short int answer = 0;
    while (nleft > 1)
    {
        sum += *w++;
        nleft -= sizeof(unsigned short int);
    }

    if (nleft == 1)
    {
        *(char *)(&answer) = *(char *)w;
        sum += answer;
    }

    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    answer = ~sum;
    return (answer);
}

#if 1
// caculate udp checksum (include payload)
unsigned short int udp_csum(struct ipheader iphdr, struct udpheader udphdr, unsigned char *payload, int payloadlen)
{
    char buf[65535];
    char *ptr;
    int chksumlen = 0;
    int i;

    ptr = &buf[0]; // ptr points to beginning of buffer buf

    /****************pseudo header  12byte*********************************/

    // Copy source IP address into buf (32 bits)
    memcpy(ptr, &iphdr.iph_sourceip, sizeof(iphdr.iph_sourceip));
    ptr += sizeof(iphdr.iph_sourceip);
    chksumlen += sizeof(iphdr.iph_sourceip);

    // Copy destination IP address into buf (32 bits)
    memcpy(ptr, &iphdr.iph_destip, sizeof(iphdr.iph_destip));
    ptr += sizeof(iphdr.iph_destip);
    chksumlen += sizeof(iphdr.iph_destip);

    // Copy zero field to buf (8 bits)
    *ptr = 0;
    ptr++;
    chksumlen += 1;

    // Copy transport layer protocol to buf (8 bits)
    memcpy(ptr, &iphdr.iph_protocol, sizeof(iphdr.iph_protocol));
    ptr += sizeof(iphdr.iph_protocol);
    chksumlen += sizeof(iphdr.iph_protocol);

    // Copy UDP length to buf (16 bits)
    memcpy(ptr, &udphdr.udph_len, sizeof(udphdr.udph_len));
    ptr += sizeof(udphdr.udph_len);
    chksumlen += sizeof(udphdr.udph_len);

    /*******************end pseudo header*************************/

    // Copy UDP source port to buf (16 bits)
    memcpy(ptr, &udphdr.udph_srcport, sizeof(udphdr.udph_srcport));
    ptr += sizeof(udphdr.udph_srcport);
    chksumlen += sizeof(udphdr.udph_srcport);

    // Copy UDP destination port to buf (16 bits)
    memcpy(ptr, &udphdr.udph_destport, sizeof(udphdr.udph_destport));
    ptr += sizeof(udphdr.udph_destport);
    chksumlen += sizeof(udphdr.udph_destport);

    // Copy UDP length again to buf (16 bits)
    memcpy(ptr, &udphdr.udph_len, sizeof(udphdr.udph_len));
    ptr += sizeof(udphdr.udph_len);
    chksumlen += sizeof(udphdr.udph_len);

    // Copy UDP checksum to buf (16 bits)
    // Zero, since we don't know it yet
    *ptr = 0;
    ptr++;
    *ptr = 0;
    ptr++;
    chksumlen += 2;

    // Copy payload to buf
    memcpy(ptr, payload, payloadlen);
    ptr += payloadlen;
    chksumlen += payloadlen;

    // Pad to the next 16-bit boundary
    for (i = 0; i < payloadlen % 2; i++, ptr++)
    {
        *ptr = 0;
        ptr++;
        chksumlen++;
    }

    return ip_csum((unsigned short int *)buf, chksumlen);
}
#endif

int main(int argc, char *argv[])
{
    struct sockaddr_ll device;
    int sd;
    char buffer[1024] = {0};

    struct ether *eth = (struct ether *)malloc(sizeof(struct ether));
    struct ipheader *ip = (struct ipheader *)malloc(sizeof(struct ipheader));
    struct udpheader *udp = (struct udpheader *)malloc(sizeof(struct udpheader));
    // struct tcphdr   *tcph = (struct tcphdr *)malloc(sizeof(struct tcphdr));

    if (argc != 5)
    {
        printf("- Invalid parameters!!!\n");
        printf("- Usage %s<source hostname/IP> <source port> <target hostname/IP> <target port>\n", argv[0]);
        exit(-1);
    }

    /*****************fill udp header*********************************/
    // udp payload
    char data[9] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};

    // udp header
    udp->udph_srcport = htons(atoi(argv[2]));

    udp->udph_destport = htons(atoi(argv[4]));

    udp->udph_len = htons(sizeof(struct udpheader) + sizeof(data));

    udp->udph_chksum = udp_csum(*ip, *udp, data, sizeof(data)); // options

    /*******************end fill udp header**************************/

#if 0
	/********************fill tcp header*************************/
	tcph->source = htons(6666);
	tcph->dest = htons(6666);
	tcph->seq = random();
	tcph->ack_seq = 0;
	tcph->tcp_len = 0x0250;
#if 0
	tcph->res1 = 0;
	tcph->res2 = 0;
	tcph->urg = 0;
	tcph->ack = 0;
	tcph->psh = 0;
	tcph->rst = 0;
	tcph->syn = 0;
	tcph->fin = 0;
#endif
	tcph->window = htons(20480);
	tcph->check = 0;//caculate chksum and fill it
	tcph->urg_prt = 0;
	/*****************end fill tcp header**************************/
#endif
    /********************fill ip header****************************/
    // fill ip header (20 bytes)

    ip->iplv = 1 << 6 | 5; // version v4 or v6 and head_len 5
                           // ip首部长度以32bit为单位计算

    ip->iph_tos = 0; // Low delay

    ip->iph_len = htons(sizeof(struct ipheader) + sizeof(struct udpheader) + sizeof(data));

    ip->iph_ident = 0; //标示字段 唯一标示一个数据包

    ip->iph_offset = 0x0040; // 16bit include offset and flag

    ip->iph_ttl = 1; // time to live

    ip->iph_protocol = 17; // UDP

    // Source IP address, can use spoofed address here!!!

    ip->iph_sourceip = inet_addr(argv[1]);

    // The destination IP address

    ip->iph_destip = inet_addr(argv[3]);

    // Calculate the checksum for integrity
    ip->iph_chksum = ip_csum((unsigned short int *)ip, (int)sizeof(struct ipheader));

    /******************end fill ip header**********************/

    /********************fill eth header*********************/
    //  Fill the eth header (14bytes)
    eth->smac[0] = 0x88;
    eth->smac[1] = 0x78;
    eth->smac[2] = 0x56;
    eth->smac[3] = 0x8c;
    eth->smac[4] = 0x58;
    eth->smac[5] = 0x0c;

    eth->dmac[0] = 0xFF;
    eth->dmac[1] = 0xFF;
    eth->dmac[2] = 0xFF;
    eth->dmac[3] = 0xFF;
    eth->dmac[4] = 0xFF;
    eth->dmac[5] = 0xFF;

    eth->eth_typ_len[0] = 0x08;
    eth->eth_typ_len[1] = 0x00;
    /***********************end fill eth header*****************/

    memcpy(buffer, eth, sizeof(struct ether));
    memcpy(buffer + sizeof(struct ether), ip, sizeof(struct ipheader));
    memcpy(buffer + sizeof(struct ether) + sizeof(struct ipheader), udp, sizeof(struct udpheader));
    memcpy(buffer + sizeof(struct ether) + sizeof(struct ipheader) + sizeof(struct udpheader), data, sizeof(data));

    /*************************fill sockaddr_ll***********************/
    /*
    利用原始套接字发送数据，那么发送的时候需要自己组织整个以太网数据帧.
    所有相关的地址使用struct sockaddr_ll 而不是struct sockaddr_in(因为协议簇是PF_PACKET不是AF_INET了),
    比如发送给某个机器,对方的地址需要使用struct sockaddr_ll.
    */
    memset(&device, 0, sizeof(device));
    if ((device.sll_ifindex = if_nametoindex("enp0s3f0")) == 0)
    {
        printf("if_nametoindex() failed to obtain interface index\n");
        exit(EXIT_FAILURE);
    }
    printf("index of interface eth0 is %i\n", device.sll_ifindex);
    device.sll_family = AF_PACKET;
    memcpy(device.sll_addr, eth->dmac, 6);
    device.sll_halen = htons(6);
    /******************end fill sockaddr_ll***************************/

    // Create a raw socket with UDP protocol
    if ((sd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0)
    {
        printf("create SOCK_RAW socket error\n");
        exit(-1);
    }
    else
    {
        printf("create SOCK_RAW socket OK.\n");
    }

    while (1)
    {
        // send udp packet
        if (sendto(sd, (void *)buffer, sizeof(struct ether) + sizeof(struct ipheader) + sizeof(struct udpheader) + sizeof(data), 0, (struct sockaddr *)&device, sizeof(device)) < 0)

        {
            perror("sendto() error");
            exit(-1);
        }
        else
        {
            printf("sendto() is OK.\n");
            sleep(1);
        }
    }
    close(sd);
    return 0;
}

#endif