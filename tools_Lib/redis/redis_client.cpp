#include "pch.h"
#include "redis_client.h"
#if defined(_MSC_VER)
#define strcasecmp _stricmp
#endif

redis_client::redis_client()
{
}


redis_client::~redis_client()
{
}


int redis_client::init_net()
{
	testConnectRedis();
	//setCacheValue("a","b");
	getCacheValue("a");
	/*getCacheValueLength("b");
	getCacheValueTLL("a");*/
	
	
	return 0;
}


redisContext* connectRedis()
{
	//redis默认监听端口为6379 可以再配置文件中修改
	redisContext* c = redisConnect("127.0.0.1", 6379);
	if (c->err)
	{
		redisFree(c);
		printf("Connect to redisServer faile\n");
		//强制退出
		MessageBox(NULL, TEXT("redis连接失败，服务端即将退出，请先启动redis!"), TEXT("提示"), MB_ICONWARNING);
		exit(0);
		return NULL;
	}
	//printf("Connect to redisServer Success\n");
	return c;
}

//测试连接redis
void testConnectRedis()
{
	connectRedis();
}

//设置key的生命周期
boolean setExpire(string key, int expire)
{
	return setExpire(key, expire, nullptr);
}

//设置key的生命周期
boolean setExpire(string key, int expire, redisContext* context)
{
	redisContext* c = context;
	if (c == nullptr)
	{
		c = connectRedis();
	}
	if (c == NULL)
	{
		return false;
	}
	char buf[10] = "";
	itoa(expire, buf, 10);
	string command = "expire " + key + " " + buf;
	redisReply* r = (redisReply*)redisCommand(c, command.c_str());
	if (r->type = REDIS_REPLY_INTEGER && r->integer == 1)
	{
		return true;
	}
	return false;
}

//设置key的值
boolean setCacheValue(string key, string value)
{
	return setCacheValue(key, value, NULL);
}

//设置key的值以及生命周期
boolean setCacheValue(string key, string value, int expire)
{
	redisContext* c = connectRedis();
	if (c == NULL)
	{
		return false;
	}

	//const char* command1 = "set stest1 value1";
	string command = "set " + key + " " + value;
	redisReply* r = (redisReply*)redisCommand(c, command.c_str());

	if (NULL == r)
	{
		printf("Execut command1 failure\n");
		redisFree(c);
		return false;
	}
	if (!(r->type == REDIS_REPLY_STATUS && strcasecmp(r->str, "OK") == 0))
	{
		printf("Failed to execute command[%s]\n", command.c_str());
		freeReplyObject(r);
		redisFree(c);
		return false;
	}
	//设置生命周期
	if (expire != NULL)
	{
		if (!setExpire(key, expire, c))
		{
			freeReplyObject(r);
			redisFree(c);
			return false;
		}
	}
	freeReplyObject(r);
	redisFree(c);
	printf("Succeed to execute command[%s]\n", command.c_str());
	return true;
}

//获取key的值
string getCacheValue(string key)
{
	redisContext* c = connectRedis();
	if (c == NULL)
	{
		return "";
	}

	string command = "get " + key;
	//const char* command3 = "get stest1";
	redisReply* r = (redisReply*)redisCommand(c, command.c_str());
	//判断是否没查询到
	if (r->type == REDIS_REPLY_NIL)
	{
		printf("Failed to execute command[%s]\n", command.c_str());
		freeReplyObject(r);
		redisFree(c);
		return "";
	}
	//判断返回的是否是字符串类型
	if (r->type != REDIS_REPLY_STRING)
	{
		printf("Failed to execute command[%s]\n", command.c_str());
		freeReplyObject(r);
		redisFree(c);
		return "";
	}
	printf("The value is: %s\n", r->str);

	string value = r->str;
	freeReplyObject(r);
	redisFree(c);
	printf("Succeed to execute command[%s]\n", command.c_str());
	return value;
}

//获取key的值的长度
int getCacheValueLength(string key)
{
	redisContext* c = connectRedis();
	if (c == NULL)
	{
		return -1;
	}
	string command = "strlen " + key;
	//const char* command2 = "strlen stest1";
	redisReply* r = (redisReply*)redisCommand(c, command.c_str());
	//判断返回的是否是整型
	if (r->type != REDIS_REPLY_INTEGER)
	{
		printf("Failed to execute command[%s]\n", command.c_str());
		freeReplyObject(r);
		redisFree(c);
		return -1;
	}
	int length = r->integer;
	freeReplyObject(r);
	redisFree(c);
	printf("The length of value is: %d.\n", length);
	printf("Succeed to execute command[%s]\n", command.c_str());
	return length;
}

//以秒为单位返回key的剩余时间
int getCacheValueTLL(string key)
{
	redisContext* c = connectRedis();
	if (c == NULL)
	{
		return -1;
	}
	string command = "ttl " + key;
	redisReply* r = (redisReply*)redisCommand(c, command.c_str());
	//判断返回的是否是整型
	if (r->type != REDIS_REPLY_INTEGER)
	{
		printf("Failed to execute command[%s]\n", command.c_str());
		freeReplyObject(r);
		redisFree(c);
		return -1;
	}
	int ttl = r->integer;
	freeReplyObject(r);
	redisFree(c);
	printf("The ttl of 'stest1' is %d.\n", ttl);
	printf("Succeed to execute command[%s]\n", command.c_str());
	return ttl;
}

//int main()
//{
//	WSADATA wsaData;
//	WSAStartup(MAKEWORD(2,1), &wsaData);
//	setCacheValue("a","b",100);
//	getCacheValue("a");
//	getCacheValueLength("b");
//	getCacheValueTLL("a");
//	getchar();
//	return 0;
//}
