#include "tools.h"
#include "status.h"

// 清理屏幕
void clear_screen(void)
{
	system("clear");
}

// 任意键继续
void anykey_continue(void)
{
	printf("按任意键继续...\n");
	getchar();
}

// 获取性别
char get_sex(void)
{
	while(true)
	{
		char sex = getchar();
		if('w' == sex || 'm' == sex)
		{
			printf("%c\n", sex);
			return sex;
		}
	}
}

// 获取成绩
bool get_score(float* score_p)
{
	float score;
	scanf("%f", &score);
	if(score < 0 && score > 100)
	{
		printf("数据输入有误！请重新输入...\n");
		return false;
	}
	*score_p = score;
	return true;
}

// 清理输入缓冲区
void clear_stdin(void)
{
	stdin->_IO_read_ptr = stdin->_IO_read_end;
}

// 获取指令
char get_cmd(char start,char end)
{
	clear_stdin();

	printf("请输入指令:");
	while(true)
	{
		char val = getchar();
		if(val >= start && val <= end)
		{
			printf("%c\n",val);
			return val;
		}
	}
}

// 获取密码
char* get_passwd(char* passwd,bool is_show,size_t size)
{
	if(NULL == passwd) return NULL;
	int count = 0;
	do{
		char val = getchar();
		if(127 == val)
		{
			if(count > 0)
			{
				if(is_show)printf("\b \b");
				count--;
			}
			continue;
		}
		else if(10 == val)
		{
			break;
		}
		passwd[count++] = val;
		if(is_show) printf("*");
	}while(count < size -1);
	passwd[count] = '\0';
	printf("\n");
	return passwd;
}

// 从键盘获取字符串
char* get_str(char* str_p, size_t hope_len)
{
	if(NULL == str_p || 0 == hope_len)
	{
		return NULL;
	}
	size_t count = 0;
	while(count < hope_len-1)
	{
		char c = getchar();
		if(27 == c) //ESC
		{
			return NULL;
		}
		if(10 == c) //ENTER
		{
			break;
		}
		if(127 == c) //BSCKSPACE
		{
			if(count > 0)
			{
				printf("\b \b");
				count --;
			}
			continue;
		}
		printf("%c", c);
		str_p[count++] = c; 
	}

	str_p[count] = '\0';
	printf("\n");
	clear_stdin();
	return str_p;
}

// 密码加密函数
char* str_to_encrypt(char* str, const char* passwd)
{
	for(int i=0; i<strlen(str); i++)
	{
		str[i] = str[i] ^ passwd[i];
	}
	return str;
}

//判断密码是否正确
bool is_password_right(char* temp, char* password)
{
	return 0 == strcmp(temp, password)?true:false;
}

// 从文件加载数据
int load_data(const char* path, void* ptr, size_t node_size)
{
	if(NULL == path || NULL == ptr)
	{
		return PRNIL;
	}
	
	FILE* frp = fopen(path, "r");
	if(NULL == frp)
	{
		return ERROR;
	}

	size_t cnt = 0, num = 0;
	while((cnt = fread(ptr+num*node_size, node_size, 1, frp)))
	{
		num += cnt;
	}
	fclose(frp);
	return num;
}

// 保存数据到文件
int save_data(const char* path, void* ptr, size_t node_size, size_t num)
{
	if(NULL == path || NULL == ptr)
	{
		return PRNIL;
	}

	FILE* fwp = fopen(path, "w");
	if(NULL == fwp)
	{
		return ERROR;
	}

	size_t cnt = fwrite(ptr, node_size, num, fwp);
	fclose(fwp);
	return cnt;
}
