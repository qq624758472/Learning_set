#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <net/if.h>    //struct ifreq
#include <sys/ioctl.h> //ioctl、SIOCGIFADDR
#include <sys/socket.h>
#include <netinet/ether.h>    //ETH_P_ALL
#include <netpacket/packet.h> //struct sockaddr_ll
#include <sys/time.h>
#include <netdb.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <netinet/ip_icmp.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <signal.h>
#include <math.h>

#pragma pack(1)
// ethernet head 14 bytes
struct ether
{
    unsigned char dmac[6];
    unsigned char smac[6];
    unsigned char eth_typ_len[2];
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
#pragma pack()

#define RECV_BUFFER_SIZE (2048) //接收缓冲区大小

double min = 0.0;
double avg = 0.0;
double max = 0.0;
double mdev = 0.0;
unsigned char RecvBuffer[RECV_BUFFER_SIZE];
int recvPackNum = 0;
char dstIp[64] = {0};

unsigned short checksum(unsigned short *buf, int nword); //校验和函数
double GetRtt(struct timeval *RecvTime, struct timeval *SendTime);
int unpack(struct timeval *RecvTime);
int RecvePacket(int sockfd, struct sockaddr *dest_addr);
void printf_all_char(unsigned char *buff, int len);

int main(int argc, char *argv[])
{
    if (argc <= 6)
    {
        printf("./udp_send srcIp srcPort dstIp dstPort sendDataLen(>64 && <1024)  TTL\n");
        return;
    }

    recvPackNum = 0;

    // 1.创建通信用的原始套接字
    int sock_raw_fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

    // 2.根据各种协议首部格式构建发送数据报  42Byte
    unsigned char send_msg[2048] = {
        //--------------组MAC--------14------
        0x02, 0xf1, 0xa5, 0x79, 0x87, 0x30, // dst_mac: 74-27-EA-B5-FF-D8    a8:a1:59:2c:82:5e
        0xa8, 0xa1, 0x59, 0x2c, 0x82, 0x5e, // src_mac: c8:9c:dc:b7:0f:19    02:f1:a5:79:87:30
        0x08, 0x00,                         //类型：0x0800 IP协议
        //--------------组IP---------20------
        0x45, 0x00, 0x00, 0x00, //版本号：4, 首部长度：20字节, TOS:0, --总长度--：
        0x00, 0x00, 0x00, 0x00, // 16位标识、3位标志、13位片偏移都设置0
        atoi(argv[6]), 17, 0x00, 0x00,   // TTL：128、协议：UDP（17）、16位首部校验和
        10, 7, 14, 62,          // src_ip: 10,   7, 14, 113
        10, 7, 14, 113,         // dst_ip: 10,   7, 14,  62
        //--------------组UDP--------8+78=86------
        0x1f, 0x90, 0x1f, 0x90, // src_port:0x1f90(8080), dst_port:0x1f90(8080)
        0x00, 0x00, 0x00, 0x00, //#--16位UDP长度--30个字节、#16位校验和
    };

    struct ether *eth = send_msg;
    struct ipheader *ip = send_msg + sizeof(struct ether);
    struct udpheader *udp = send_msg + sizeof(struct ether) + sizeof(struct ipheader);

    ip->iph_sourceip = inet_addr(argv[1]);
    ip->iph_destip = inet_addr(argv[3]);
    udp->udph_srcport = htons(atoi(argv[2]));
    udp->udph_destport = htons(atoi(argv[4]));
    memcpy(dstIp, argv[3], strlen(argv[3]));

    // 6.发送数据
    struct sockaddr_ll sll; //原始套接字地址结构
    struct ifreq req;       //网络接口地址

    strncpy(req.ifr_name, "eno1", IFNAMSIZ);          //指定网卡名称
    if (-1 == ioctl(sock_raw_fd, SIOCGIFINDEX, &req)) //获取网络接口
    {
        perror("ioctl");
        close(sock_raw_fd);
        exit(-1);
    }

    // 3.UDP伪头部
    unsigned char pseudo_head[1024] =
        {
            //------------UDP伪头部--------12--
            10, 7, 14, 113,       // src_ip: 10,   7, 14, 113
            10, 7, 14, 62,        // dst_ip: 10,   7, 14,  62
            0x00, 17, 0x00, 0x00, // 0,17,#--16位UDP长度--20个字节
        };

    /*将网络接口赋值给原始套接字地址结构*/
    bzero(&sll, sizeof(sll));
    sll.sll_ifindex = req.ifr_ifindex;

    int um = 0;
    while (um < 10)
    {
        struct timeval *pTime;
        pTime = send_msg + 42;
        gettimeofday(pTime, NULL); //数据段存放发送时间
        
        int len = sizeof(struct timeval) + (atoi(argv[5])-sizeof(struct timeval)-14-20-8);
        if (len % 2 == 1) //判断len是否为奇数
        {
            len++; //如果是奇数，len就应该加1(因为UDP的数据部分如果不为偶数需要用0填补)
        }

        *((unsigned short *)&send_msg[16]) = htons(20 + 8 + len);     // IP总长度 = 20 + 8 + len
        *((unsigned short *)&send_msg[14 + 20 + 4]) = htons(8 + len); // udp总长度 = 8 + len

        *((unsigned short *)&pseudo_head[10]) = htons(8 + len); //为头部中的udp长度（和真实udp长度是同一个值）
        // 4.构建udp校验和需要的数据报 = udp伪头部 + udp数据报
        memcpy(pseudo_head + 12, send_msg + 34, 8 + len); //--计算udp校验和时需要加上伪头部--
        // 5.对IP首部进行校验
        *((unsigned short *)&send_msg[24]) = htons(checksum((unsigned short *)(send_msg + 14), 20 / 2));
        // 6.--对UDP数据进行校验--
        *((unsigned short *)&send_msg[40]) = htons(checksum((unsigned short *)pseudo_head, (12 + 8 + len) / 2));

        /*send */
        len = sendto(sock_raw_fd, send_msg, 14 + 20 + 8 + len, 0, (struct sockaddr *)&sll, sizeof(sll));
        if (len <= 0)
        {
            perror("sendto");
        }

        RecvePacket(sock_raw_fd, (struct sockaddr *)&sll);
        usleep(10000);
        um++;
    }

    printf("send 10 pack ,recv %d pack,loss %ld%\n", recvPackNum, (10 - recvPackNum) * 10);
    close(sock_raw_fd);
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

int RecvePacket(int sockfd, struct sockaddr *dest_addr)
{
    int RecvBytes = 0;
    int addrlen = sizeof(struct sockaddr_in);
    struct timeval RecvTime;

    memset(RecvBuffer, 0, sizeof(RecvBuffer));

    if ((RecvBytes = recvfrom(sockfd, RecvBuffer, RECV_BUFFER_SIZE,
                              0, dest_addr, &addrlen)) < 0)
    {
        perror("recvfrom");
        return 0;
    }
    // printf_all_char(RecvBuffer,RECV_BUFFER_SIZE);
    gettimeofday(&RecvTime, NULL);

    if (unpack(&RecvTime) == -1)
    {
        printf("unpack error\n");
        return -1;
    }
}

int unpack(struct timeval *RecvTime)
{
    struct ether *eth = (struct ether *)RecvBuffer;
    struct ip *Ip = (struct ip *)(RecvBuffer + sizeof(struct ether));
    struct icmp *Icmp;
    int ipHeadLen;
    double rtt;

    ipHeadLen = Ip->ip_hl << 2; // ip_hl字段单位为4字节
    // Icmp = (struct icmp *)(RecvBuffer + ipHeadLen);
    Icmp = (RecvBuffer + sizeof(struct ether) + sizeof(struct ip));

    //判断接收到的报文是否是自己所发报文的响应
    if (Icmp->icmp_type == ICMP_DEST_UNREACH)
    {
        int len = sizeof(struct ether) + sizeof(struct ip) + sizeof(struct icmp);
        struct timeval *SendTime = (struct timeval *)(Icmp->icmp_data + 28); // 28字节可以偏移到icmp的数据区
        rtt = GetRtt(RecvTime, SendTime);
        // printf_all_char(SendTime,1024);

        if (memcmp(inet_ntoa(Ip->ip_src), dstIp, strlen(dstIp)) != 0)
        {
            return 0;
        }

        printf("%u bytes from %s: icmp_seq=%u ttl=%u time=%.1f ms\n",
               ntohs(Ip->ip_len) - ipHeadLen,
               inet_ntoa(Ip->ip_src),
               Icmp->icmp_seq,
               Ip->ip_ttl,
               rtt);
        recvPackNum++;

        if (rtt < min || 0 == min)
            min = rtt;
        if (rtt > max)
            max = rtt;
        avg += rtt;
        mdev += rtt * rtt;

        return 0;
    }
}

double GetRtt(struct timeval *RecvTime, struct timeval *SendTime)
{
    struct timeval sub = *RecvTime;

    if ((sub.tv_usec -= SendTime->tv_usec) < 0)
    {
        --(sub.tv_sec);
        sub.tv_usec += 1000000;
    }
    sub.tv_sec -= SendTime->tv_sec;

    return sub.tv_sec * 1000.0 + sub.tv_usec / 1000.0; //转换单位为毫秒
}

void printf_all_char(unsigned char *buff, int len)
{
    for (int i = 0; i < len; i++)
    {
        if (i % 16 == 0)
            printf("\n");
        printf(" %2x ", buff[i]);
    }
}