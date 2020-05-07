#ifndef REDIS_CLIENT
#define REDIS_CLIENT

#include <hiredis.h>
//#include "../../src/Win32_Interop/win32fixes.h"
#include <assert.h>
//#include <winsock.h>
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <tuple>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <iostream>
//#include <winsock2.h> 


typedef unsigned char boolean;
class redis_client
{
public:
	redis_client();
	~redis_client();

	int init_net();
};


int getCacheValueTLL(string key);
redisContext* connectRedis();
void testConnectRedis();
boolean setExpire(string key, int expire);
boolean setExpire(string key, int expire, redisContext* context);
boolean setCacheValue(string key, string value);
boolean setCacheValue(string key, string value, int expire);
string getCacheValue(string key);
int getCacheValueLength(string key);


#endif