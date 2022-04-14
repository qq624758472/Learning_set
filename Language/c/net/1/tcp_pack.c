#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/ip_icmp.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>
#include <sys/time.h>

#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <net/if_arp.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <netdb.h>
#include <signal.h>

#define RECV_BUFFER_SIZE (2048) //接收缓冲区大小

int dataLength = 0;
int ttl = 64;
int delay = 1000; // 1000ms
double min = 0.0;
double avg = 0.0;
double max = 0.0;
double mdev = 0.0;
unsigned char RecvBuffer[RECV_BUFFER_SIZE];
char dstIp[64] = {0};

static int pgrecver;
#define IP4_HDRLEN (sizeof(struct ip))
#define EXTRACT_ICMPHDRP(ip_pktp) ((struct icmp *)((ip_pktp->ip_hl << 2) + ((char *)(ip_pktp))))

struct udp_front // tcp(udp)伪首部结构体
{
    uint32_t srcip;
    uint32_t desip;
    u_int8_t zero;
    u_int8_t protocol;
    u_int16_t len;
};

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

u_int16_t in_chksum(u_int16_t *addr, int len);
u_int16_t tcp_check(char *sendbuf, int len, const struct udp_front front);
int make_message(char *sendbuf, int send_buf_len, uint32_t src_ip, u_int16_t src_port, uint32_t des_ip, u_int16_t des_port);
int create_ping_recver(void);
int filter_ip_icmp(struct ip *ip_pktp, size_t n);
double GetRtt(struct timeval *RecvTime, struct timeval *SendTime);
int RecvePacket(int sockfd, struct sockaddr *dest_addr,int len);
int unpack(struct timeval *RecvTime, int SendLen);
void printf_all_char(unsigned char *buff, int len);

struct timeval RecvTime;
struct timeval SendTime;

int recvPackNum = 0;

//./a.out src_ip src_port dst_ip dst_port
// user must root
int main(int argc, char *argv[])
{
    if (argc <= 6)
    {
        printf("./a.out src_ip src_port dst_ip dst_port dataLength ttl\n");
        return;
    }
    int raw_sockfd;
    int size = 1024 * 50;
    dataLength = atoi(argv[5]); // dataLength
    // delay = atoi(argv[4]);      // delayTime
    ttl = atoi(argv[6]); // ttl
    memcpy(dstIp, argv[3], strlen(argv[3]));

    char send_message[dataLength];
    struct sockaddr_in server_address;
    //创建原始套接字
    raw_sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    //创建套接字地址
    bzero(&server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(argv[3]);
    //设置套接字为随数据包含IP首部(设置这个选项后需要我们手动写入IP头)
    setsockopt(raw_sockfd, IPPROTO_IP, IP_HDRINCL, &size, sizeof(size));

    bzero(&send_message, sizeof(send_message));
    //拼接完整的TCP数据包(IP头+TCP头+数据)
    int mesg_len = make_message(send_message, dataLength, inet_addr(argv[1]), atoi(argv[2]), inet_addr(argv[3]), atoi(argv[4]));

#if 0 //创建一个icmp协议接收线程
    // recv icmp
    pgrecver = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (pgrecver < 0)
    {
        printf("%s: %m\n", "socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)");
        exit(EXIT_FAILURE);
    }
    create_ping_recver(); //创建icmp包接收线程
#endif
    //将IP数据包发送出去
    int nu = 0;
    while (nu < 10)
    {
        memset(&RecvTime, 0, sizeof(struct timeval));
        memset(&SendTime, 0, sizeof(struct timeval));
        int length = sendto(raw_sockfd, send_message, mesg_len, 0, (struct sockaddr *)&server_address, sizeof(server_address));
        gettimeofday(&SendTime, NULL);

        RecvePacket(raw_sockfd, (struct sockaddr *)&server_address,mesg_len);
        nu++;
    }

    printf("send 10 pack ,recv %d pack,loss %ld%\n", recvPackNum, (10 - recvPackNum) * 10);
    // close(pgrecver);
    close(raw_sockfd);
    return 0;
}

int RecvePacket(int sockfd, struct sockaddr *dest_addr, int SendLen)
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
    gettimeofday(&RecvTime, NULL);
    //printf_all_char(RecvBuffer, RECV_BUFFER_SIZE);

    if (unpack(&RecvTime,SendLen) == -1)
    {
        printf("unpack error\n");
        return -1;
    }
}

void *ping_recver(void *pgrecverp)
{
    /**/
    int pgrecver = *(int *)pgrecverp;
    struct sockaddr_in srcaddr;
    socklen_t slen;
    ssize_t errs;
    char buf[IP_MAXPACKET];
    struct ip *ip_pktp = (struct ip *)buf;
    struct icmp *icmpp;

    for (;;)
    {
        slen = sizeof(srcaddr);
        memset(&srcaddr, 0, slen);
        errs = recvfrom(pgrecver, ip_pktp, IP_MAXPACKET, 0, (struct sockaddr *)&srcaddr, &slen);
        if (errs < 0)
        {
            printf("%s: %m\n", "recvfrom()");
            pthread_exit((void *)EXIT_FAILURE);
        }

        //
        double rtt = 0.0;
        buf[errs] = 0;
        gettimeofday(&RecvTime, NULL);
        rtt = GetRtt(&RecvTime, &SendTime);
        printf("Got stuff from ping recv'er....\nrtt=%0.4f ms\n", rtt / 2);

        if ((filter_ip_icmp(ip_pktp, (size_t)errs)) < 0)
        {
            continue;
        }
        icmpp = EXTRACT_ICMPHDRP(ip_pktp);
        // printf("%d bytes from %s: id=0x%x seq=%hu, ttl=%d, data: %s\n", (int)errs,
        //        getvmname(srcaddr.sin_addr), ntohs(icmpp->icmp_id), ntohs(icmpp->icmp_seq),
        //        ip_pktp->ip_ttl, (((char *)icmpp) + ICMP_MINLEN));
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

#define PING_ICMP_ID 0x2691
int filter_ip_icmp(struct ip *ip_pktp, size_t n)
{
    struct icmp *icmpp;

    if (ip_pktp->ip_p != IPPROTO_ICMP)
    {
        printf("ip proto: 0x%x is not IPPROTO_ICMP. Ignoring....\n", ip_pktp->ip_p);
        return -1;
    }
    if (n < (IP4_HDRLEN + ICMP_MINLEN))
    {
        printf("icmp msg too small, len: %u bytes. Ignoring....\n", (u_int)n);
        return -1;
    }
    printf("msg IP.id: 0x%x, PING_ICMP_ID: 0x%x\n", ntohs(ip_pktp->ip_id), PING_ICMP_ID);

    /* point past the ip header to the icmp header */
    icmpp = EXTRACT_ICMPHDRP(ip_pktp);
    if (icmpp->icmp_code != 0 || icmpp->icmp_type != ICMP_ECHOREPLY)
    {
        printf("icmp msg not echo reply, type 0x%x, code: 0x%x. Ignoring....\n", icmpp->icmp_type, icmpp->icmp_code);
        return -1;
    }
    if (ntohs((uint16_t)icmpp->icmp_id) != PING_ICMP_ID)
    {
        printf("icmp echo reply has wrong ID: 0x%x. Ignoring....\n", ntohs((uint16_t)icmpp->icmp_id));
        return -1;
    }
    return 0;
}

int create_ping_recver(void)
{
    int erri;
    struct pthread_t *tidentp;

    tidentp = malloc(sizeof(pthread_t));
    if (tidentp == NULL)
    {
        printf("%s: %m\n", "malloc()");
        return -1;
    }
    /* Create the thread for recving pings replies */
    erri = pthread_create(tidentp, NULL, &ping_recver, &pgrecver);
    if (0 > erri)
    {
        printf("%s: %m\n", "pthread_create(ping_recver)");
        free(tidentp);
        return -1;
    }

    return 0;
}

//拼接IP数据报
int make_message(char *sendbuf, int send_buf_len, uint32_t src_ip, u_int16_t src_port, uint32_t des_ip, u_int16_t des_port)
{
    // char message[20] = {"this tcp info"}; //数据在这里并没有用，为空值
    //char message[2048] = {"this tcp info"}; //数据在这里并没有用，为空值
    char *message = (char*)malloc(send_buf_len-40);
    memset(message, 0X5A, send_buf_len-40);
    // bzero(message, sizeof(message));
    // strcpy(message, "hello，world!");
    struct iphdr *ip;
    ip = (struct iphdr *)sendbuf;
    ip->ihl = sizeof(struct iphdr) >> 2; //首部长度
    ip->version = 4;                     // ip协议版本
    ip->tos = 0;                         //服务类型字段
    ip->tot_len = 0;                     //总长度
    ip->id = htons(10000);               // id值
    ip->frag_off = 0;
    ip->ttl = ttl;
    ip->protocol = IPPROTO_TCP;
    ip->check = 0; //内核会算相应的效验和
    ip->saddr = src_ip;
    ip->daddr = des_ip;

    struct udp_front front; //仅为了计算校验和使用
    front.srcip = src_ip;
    front.desip = des_ip;
    front.len = htons(20 + strlen(message));
    front.protocol = 6;
    front.zero = 0;

    struct tcphdr *tcp;
    tcp = (struct tcphdr *)(sendbuf + sizeof(struct iphdr));
    bzero(tcp, sizeof(struct tcphdr *));
    tcp->source = htons(src_port); //源端口
    tcp->dest = htons(des_port);   //目的端口
    tcp->seq = htonl(100000000);   //随机生成的数
    tcp->ack_seq = 0;              //当ack置0的时候，ack_seq无所谓

    tcp->doff = 5;              //数据偏移(TCP头部字节长度/4)
    tcp->res1 = 0;              //保留字段(4位)
    tcp->fin = 0;               //..用来释放一个连接
    tcp->syn = 1;               //..表示这是一个连接请求
    tcp->rst = 0;               //..用来表示tcp连接是否出现严重差错
    tcp->psh = 0;               //..推送
    tcp->ack = 0xaa;            //..0:表示是一个连接请求
    tcp->urg = 0;               //..紧急数据标志
    tcp->res2 = 0;              //保留字段(2位)
    tcp->window = htons(65535); //初始窗口值设置

    tcp->check = 0;
    tcp->urg_ptr = 0;

    tcp->check = 0;                       //效验和，效验整个tcp数据报
    strcpy((sendbuf + 20 + 20), message); //此处message为空

    tcp->check = tcp_check((sendbuf + 20), 20 + strlen(message), front);

    ip->tot_len = (20 + 20 + strlen(message)); //总长度
    printf("ip->tot_len:%d, des_port:%d\n", ip->tot_len, des_port);
    ip->check = in_chksum((unsigned short *)sendbuf, 20);

    return (ip->tot_len);
}

//计算tcp(udp)效验和
unsigned short tcp_check(char *sendbuf, int len, const struct udp_front front)
{
    char str[dataLength];
    bzero(&str, dataLength);
    bcopy(&front, str, sizeof(front));
    bcopy(sendbuf, str + sizeof(front), len);
    struct udp_front *ptr;
    ptr = (struct udp_front *)str;
    char *s;
    s = (str + 20);
    return in_chksum((unsigned short *)str, sizeof(front) + len);
}

//效验和算法
uint16_t in_chksum(uint16_t *addr, int len)
{
    int nleft = len;
    uint32_t sum = 0;
    uint16_t *w = addr;
    uint16_t answer = 0;
    //把ICMP报头二进制数据以2字节为单位累加起来
    while (nleft > 1)
    {
        sum += *w++;
        nleft -= 2;
    }
    if (nleft == 1)
    {
        *(unsigned char *)(&answer) = *(unsigned char *)w;
        sum += answer;
    }
    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    answer = ~sum;
    return answer;
}

int unpack(struct timeval *RecvTime, int SendLen)
{
    // struct ether *eth = (struct ether *)RecvBuffer;
    struct ip *Ip = (struct ip *)(RecvBuffer);
    struct tcphdr *tcp;
    int ipHeadLen;
    double rtt;
    char *p = tcp;

    ipHeadLen = Ip->ip_hl << 2; // ip_hl字段单位为4字节
    tcp = (struct tcphdr *)(RecvBuffer + 20);
    // Icmp = (RecvBuffer + sizeof(struct ether) + sizeof(struct ip));
    // printf_all_char(RecvBuffer,1024);
    rtt = GetRtt(RecvTime, &SendTime);
    printf("%u bytes from %s: ttl=%u time=%.1f ms\n",
           SendLen,
           inet_ntoa(Ip->ip_src),
           Ip->ip_ttl,
           rtt);
    recvPackNum++;
}

void printf_all_char(unsigned char *buff, int len)
{
    for (int i = 0; i < len; i++)
    {
        if (i % 16 == 0)
            printf("\n");
        printf(" %2x ", buff[i]);
    }

    printf("\n\n");
}