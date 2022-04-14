#include "ip_pack.h"

#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <sys/socket.h>

struct hostent *pHost = NULL; //保存主机信息
int sockfd;					  // icmp套接字
int nSend = 1;
char *IP = NULL;

#define WAIT_TIME 5

#define IPPROTO_TOUR 176
#define TOUR_IP_ID 0x2691
#define IP4_HDRLEN (sizeof(struct ip))

char SendBuffer[SEND_BUFFER_SIZE];
char RecvBuffer[RECV_BUFFER_SIZE];
int nRecv = 0;				  //实际接收到的报文数
struct timeval FirstSendTime; //用以计算总的时间
struct timeval LastRecvTime;
double min = 0.0;
double avg = 0.0;
double max = 0.0;
double mdev = 0.0;

/*calculates the checksum everything*/
uint16_t csum(void *data, size_t len)
{
	uint32_t sum = 0;
	uint16_t *ptr = (uint16_t *)data;

	/*subtract two each time since we are going by bytes*/
	for (; len > 1; ptr += 1)
	{
		sum += *ptr;
		/*printf("new sum: %X\n", sum);*/

		/*if sums most sig bit set then wrap around, cuz 1's comp addition*/
		if (sum & 0x10000)
		{
			sum = (sum & 0xFFFF) + ((sum >> 16) & 0xFFFF);
			/*printf("sum after wrap: %X\n", sum);*/
		}

		/*dont update the len in forloop so we get an extra loop*/
		len -= 2;
	}

	/*if there was not an even amount of bytes*/
	if (len > 0)
	{
		sum += *ptr;
	}

	/*fold 32 bits into 16*/
	if (((sum >> 16) & 0xFFFF) > 0)
	{
		sum = (sum & 0xFFFF) + ((sum >> 16) & 0xFFFF);
	}

	/*if there is stuff in the high bits, I probably did it wrong*/
	if (((sum >> 16) & 0xFFFF) > 0)
	{
		printf("%s\n", "yew sux, try again");
		exit(EXIT_FAILURE);
	}

	/*take ones comp and return it*/
	/*printf("rtn sum before not: %X\n", sum);*/
	return ((uint16_t)~sum);
}

void craft_ip(void *ip_pktbuf, uint8_t proto, u_short ip_id, struct in_addr src_ip, struct in_addr dst_ip, size_t paylen)
{
	struct ip *ip_pkt = ip_pktbuf;
	if (ip_pktbuf == NULL)
	{
		printf("%s\n", "ip_pktbuf is NULL!");
		exit(EXIT_FAILURE);
	}

	/*IPv4 header*/
	ip_pkt->ip_hl = IP4_HDRLEN / sizeof(uint32_t);
	ip_pkt->ip_v = IPVERSION;
	ip_pkt->ip_tos = 0;
	ip_pkt->ip_len = htons((uint16_t)(IP4_HDRLEN + paylen));
	ip_pkt->ip_id = htons(ip_id);
	ip_pkt->ip_off = htons(IP_DF); //不分段标志
	ip_pkt->ip_ttl = 25;
	ip_pkt->ip_p = proto;
	ip_pkt->ip_dst.s_addr = dst_ip.s_addr;
	ip_pkt->ip_src.s_addr = src_ip.s_addr;
	/*memcpy(&(ip_pkt->ip_dst.s_addr), &dst_ip.s_addr, sizeof(in_addr_t));
	memcpy(&(ip_pkt->ip_src.s_addr), &src_ip.s_addr, sizeof(in_addr_t));*/
	ip_pkt->ip_sum = 0;
	if (proto == IPPROTO_ICMP)
	{
		/**
		 * MIS-LEADING: We csum here because when WE use IPPROTO_ICMP
		 *  we are sending over a PF_PACKET socket. We must provide
		 *  the normal function of the IPv4 layer here by calculating
		 *  the checksum ourselves.
		 */
		ip_pkt->ip_sum = csum(ip_pkt, IP4_HDRLEN);
	}
}

/**
 * @brief Use ip protocol to from icmp package
 *
 * @param argc
 * @param argv
 */
void Call(int argc, char *argv[])
{

#if 0
	struct protoent *protocol;
	struct sockaddr_in dest_addr; // IPv4专用socket地址,保存目的地址

	int ttl = 2;
	int yes = 1;

	in_addr_t inaddr; // ip地址（网络字节序）

	if (argc < 2)
	{
		printf("Usage: %s [hostname/IP address]\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	if ((protocol = getprotobyname("ip")) == NULL)
	{
		perror("getprotobyname");
		exit(EXIT_FAILURE);
	}

	/* 创建IP套接字 */
	// AF_INET:IPv4, SOCK_RAW:IP协议数据报接口, IPPROTO_IP:IP协议
	if ((sockfd = socket(PF_INET, SOCK_RAW, IPPROTO_IP)) < 0)
	{
		perror("socket");
		exit(EXIT_FAILURE);
	}
	setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_TTL, (char *)&ttl, sizeof(ttl)); //设置组播TTL
	setsockopt(sockfd, IPPROTO_IP, IP_TTL, (char *)&ttl, sizeof(ttl));			 //在所有socket上设置TTL
	setsockopt(sockfd, IPPROTO_IP, IP_RECVTTL, &yes, sizeof(yes));				 //告诉我输入数据包的TTL。
	dest_addr.sin_family = AF_INET;

	/* 将点分十进制ip地址转换为网络字节序 */
	if ((inaddr = inet_addr(argv[1])) == INADDR_NONE)
	{
		/* 转换失败，表明是主机名,需通过主机名获取ip */
		if ((pHost = gethostbyname(argv[1])) == NULL)
		{
			herror("gethostbyname()");
			exit(EXIT_FAILURE);
		}
		memmove(&dest_addr.sin_addr, pHost->h_addr_list[0], pHost->h_length);
	}
	else
	{
		memmove(&dest_addr.sin_addr, &inaddr, sizeof(struct in_addr));
	}

	if (NULL != pHost)
		printf("PING %s", pHost->h_name);
	else
		printf("PING %s", argv[1]);
	printf("(%s) %d bytes of data.\n", inet_ntoa(dest_addr.sin_addr), ICMP_LEN);

	IP = argv[1];
	signal(SIGINT, Statistics);
	while (nSend < SEND_NUM)
	{
		int unpack_ret;

		SendPacket(sockfd, &dest_addr, nSend);

		unpack_ret = RecvePacket(sockfd, &dest_addr);
		if (-1 == unpack_ret) //（ping回环时）收到了自己发出的报文,重新等待接收
			RecvePacket(sockfd, &dest_addr);

		sleep(1);
		nSend++;
	}

	Statistics(0); //输出信息，关闭套接字
#endif
}

char *getvmname(struct in_addr vmaddr)
{
	struct hostent *he;
	char *name;
	/*int i = 0;*/
	if (NULL == (he = gethostbyaddr(&vmaddr, 4, AF_INET)))
	{
		herror("ERROR gethostbyaddr()");
		printf("Target ip for lookup was: %s\n", inet_ntoa(vmaddr));
		//exit(EXIT_FAILURE);
	}
	name = he->h_name;
	/*while(name != NULL && name[0] != 'v' && name[1] != 'm') {
		name = he->h_aliases[i];
		i++;
	}*/
	return name;
}

void SetIP(int argc, char *argv[])
{
	int rtsock;
	const int on = 1;
	ssize_t n;
	struct ip *ip_pktp;
	struct sockaddr_in addr;
	socklen_t slen;
	char buf[IP_MAXPACKET];
	ip_pktp = (struct ip *)(buf);

	rtsock = socket(AF_INET, SOCK_RAW, IPPROTO_TOUR);
	if (rtsock < 0)
	{
		printf("%s: %m\n", "socket(AF_INET, SOCK_RAW, IPPROTO_TOUR)");
		exit(EXIT_FAILURE);
	}
	if ((setsockopt(rtsock, IPPROTO_IP, IP_HDRINCL, &on, sizeof(on))) < 0)
	{
		printf("%s: %m\n", "setsockopt(IP_HDRINCL)");
		exit(EXIT_FAILURE);
	}

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = 0;
	inet_pton(AF_INET, argv[1], &addr.sin_addr);
	// char *dst_host =  getvmname(addr.sin_addr);
	// if(dst_host != NULL) printf("Dst/src: %s , %s\n",dst_host, inet_ntoa(addr.sin_addr));

	craft_ip(ip_pktp, IPPROTO_TOUR, TOUR_IP_ID, addr.sin_addr, addr.sin_addr, 0);

	printf("%s\n", "Sending...");
	n = sendto(rtsock, ip_pktp, IP4_HDRLEN, 0, (struct sockaddr *)&addr, sizeof(addr));
	if (n < 0)
	{
		printf("%s: %m\n", "socket(AF_INET, SOCK_RAW, IPPROTO_TOUR)");
		exit(EXIT_FAILURE);
	}

	printf("%s\n", "Waiting to recv...");
	slen = sizeof(addr);
	memset(buf, 0, IP_MAXPACKET);
	n = recvfrom(rtsock, ip_pktp, IP_MAXPACKET, 0, (struct sockaddr *)&addr, &slen);
	if (n < 0)
	{
		printf("%s: %m\n", "socket(AF_INET, SOCK_RAW, IPPROTO_TOUR)");
		exit(EXIT_FAILURE);
	}
	return;
}

int main(int argc, char *argv[])
{
	SetIP(argc, argv);

	return 0;
}