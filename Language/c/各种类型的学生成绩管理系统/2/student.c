#include "tools.h"
#include "student.h"

// 学生数组指针
student* stu;

int temp_stu; // 记录当前学生
int count_stu_at_school; // 学生人数

bool flag_stu_success_enter; // 学生成功登入标志位

// 总分成绩比较
int stu_cmp_s(const void* stu_p1,const void* stu_p2)
{
    return ((student*)stu_p1)->Sum < ((student*)stu_p2)->Sum;
}

// 总分成绩排名
void stu_rank_s(void)
{
    qsort(stu, count_stu_at_school, sizeof(student), stu_cmp_s);
    for(int i=0; i<count_stu_at_school; i++)
	{
        stu[i].rank = i+1;
	}
}

// 判断学生学号是否存在
bool is_stu_id_exist(int id)
{
	int i;
	for(i=0; i<count_stu_at_school; i++)
	{
		if(id == stu[i].id)
		{
			temp_stu = i;
			return true;
		}
	}
	return false;
}

// 学生登入
void stu_enter()
{
	int temp_id;
	char temp_password[20];
	bool flag_right_id = true;
	flag_stu_success_enter = false;
	while(true)
	{
		clear_screen();
		printf("********学生管理系统********\n");
		printf("--------------------------\n");
		if(flag_right_id)
		{
			printf("学号:");
			scanf("%d", &temp_id);
			getchar();
		}
		else
		{
			printf("学号:%d\n", temp_id);
		}
		if(is_stu_id_exist(temp_id))
		{
			flag_right_id = false;
			printf("密码:");
			get_passwd(temp_password, true, 20);
			if(is_password_right(temp_password, stu[temp_stu].password) && stu[temp_stu].count_stu_password_wrong < 3)
			{
				stu[temp_stu].count_stu_password_wrong= 0;
				flag_stu_success_enter = true;
				break;
			}
			else
			{
				stu[temp_stu].count_stu_password_wrong ++;
				if(stu[temp_stu].count_stu_password_wrong >= 3)
				{
					printf("三次密码错误,帐号被锁定,由教师解锁!");
					anykey_continue();
					return;
				}
				else
				{
					printf("密码错误！");
					anykey_continue();
				}
			}
		}
		else
		{
			printf("学号错误!");
			anykey_continue();
		}
	}
	if(0 == stu[temp_stu].flag_stu_first_enter)
	{
		clear_screen();
		printf("********学生管理系统********\n");
		printf("--------------------------\n");
		printf("第一次登入，请修改密码:");
		get_passwd(stu[temp_stu].password, true, 20);
		stu[temp_stu].flag_stu_first_enter = 1;
	}
}

//个人信息
void stu_personal_information()
{
	clear_screen();
	printf("********学生管理系统********\n");
	printf("--------------------------\n");
	printf("姓名:%s\n性别:%s\n学号:%d\n", stu[temp_stu].name, 'm'==stu[temp_stu].sex?"男":"女", stu[temp_stu].id);
	anykey_continue();
}

// 修改密码
void stu_change_password()
{
    char password1[20], password2[20];
    system("clear");
    printf("********学生管理系统********\n");
    printf("--------------------------\n");
    printf("输入修改的密码:");
	get_str(password1, 20);
	printf("确认修改的密码:");
	get_str(password2, 20);
	if(0 == strcmp(password1, password2))
	{
		strcpy(stu[temp_stu].password, password1);
		printf("修改成功!");
		anykey_continue();
	}
	else
	{
		printf("两次输入不一致!");
		anykey_continue();
	}
}

// 查询成绩
void stu_search_grade(student* stu_p)
{
	clear_screen();
	printf("********学生管理系统********\n");
	printf("--------------------------\n");
    printf("语文成绩:%g\n",stu_p->Chinese);
    printf("数学成绩:%g\n",stu_p->Math); 
    printf("英语成绩:%g\n",stu_p->English);
    printf("总分成绩:%g\n",stu_p->Sum);
	stu_rank_s();
	printf("排名:%d\n",stu_p->rank);
	anykey_continue();
}

// 学生系统流程控制
void stu_start()
{
	stu_enter();
	while(flag_stu_success_enter)
	{
		clear_screen();
		printf("********学生管理系统********\n");
		printf("--------------------------\n");
		printf("1.个人信息\n");
		printf("2.查询成绩\n");
		printf("3.修改密码\n");
		printf("4.退出帐号\n");
		switch(get_cmd('1', '4'))
		{
			case '1': stu_personal_information(); break;
			case '2': stu_search_grade(stu+temp_stu); break;
			case '3': stu_change_password(); break;
			case '4': return;
		}
	}
}
