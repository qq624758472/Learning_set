#include "tools.h"
#include "admin.h"
#include "teacher.h"

char admin_password[20] = "123456"; // 校长密码，初始密码123456.
int flag_admin_first_enter; // 校长第一次登入标志位
bool flag_admin_success_enter; // 校长登入成功标志位

//校长登入
void admin_enter()
{
	flag_admin_success_enter = false;
	char temp_password[20] = {};
	while(true)
	{
		clear_screen();
		printf("********学生管理系统********\n");
		printf("密码:");
		get_passwd(temp_password, true, 20);
		if(is_password_right(temp_password, admin_password))
		{
			flag_admin_success_enter = true;
			break;
		}
		else
		{	
			printf("密码错误！");
			anykey_continue();
		}
	}
	if(0 == flag_admin_first_enter)
	{
		clear_screen();
		printf("********学生管理系统********\n");
		printf("--------------------------\n");
		printf("第一次登入，请修改密码:");
		get_passwd(admin_password, true, 20);
		flag_admin_first_enter = 1;
	}
}

// 校长复位自身密码
void admin_reset_password()
{
	clear_screen();
	printf("********学生管理系统********\n");
	printf("--------------------------\n");
	strcpy(admin_password, "123456");
    flag_admin_first_enter = 0;
	printf("重置成功!");
	anykey_continue();
}

// 复位教师密码
void admin_reset_tea_password()
{
	clear_screen();
	printf("********学生管理系统********\n");
	printf("--------------------------\n");
	printf("输入教师的工号:");
   	int id;
   	scanf("%d", &id);
	getchar();
	if(is_tea_id_exist(id))
	{
		strcpy(tea[temp_tea].password, "123456");
		tea[temp_tea].flag_tea_first_enter = 0;
		tea[temp_tea].count_tea_password_wrong = 0;
		printf("重置成功!");
		anykey_continue();
	}
   	else
   	{
  		printf("工号错误!");
    	anykey_continue();
	}
}

// 添加教师
void admin_add_tea()
{
	if(count_tea_at_school >= MAX_TEA)
    {
        printf("教师已满,无法添加!");
		anykey_continue();
        return;
    }
	bzero(tea+count_tea_at_school,sizeof(teacher));
	clear_screen();
	printf("********学生管理系统********\n");
	printf("--------------------------\n");
	printf("姓名:  性别(m.男 w.女):\n");
	scanf("%s %c", tea[count_tea_at_school].name, &tea[count_tea_at_school].sex);
	tea[count_tea_at_school].id = 20160000 + count_tea_at_school;
	strcpy(tea[count_tea_at_school].password, "123456");
	count_tea_at_school ++;
	printf("添加成功!\n");
	anykey_continue();
}

// 删除教师
void admin_remove_tea()
{
	if(0 >= count_tea_at_school)
    {
        printf("系统内无教师!");
		anykey_continue();
        return;
    }
	clear_screen();
    int id1, id2;
    printf("********学生管理系统********\n");
    printf("--------------------------\n");
	printf("输入工号:");
	scanf("%d", &id1);
	printf("确认工号:");
	scanf("%d", &id2);
	getchar();
    if(id1 == id2 && is_tea_id_exist(id1))
	{
		FILE* fap = fopen("tea_dropout.txt", "a");
   		fwrite(tea+temp_tea, sizeof(teacher), 1, fap);
   		fclose(fap);
        
   		memcpy(tea+temp_tea, tea+temp_tea+1, (count_tea_at_school-temp_tea-1)*sizeof(teacher));
		count_tea_at_school--;
   		printf("删除帐号成功!");
		anykey_continue();  
    }
    else
    {
	    printf("工号输入错误!");
		anykey_continue();
    }
}

// 显示在职教师
void admin_show_all_tea_at_school()
{
	clear_screen();
	printf("********学生管理系统********\n");
	printf("--------------------------\n");
	printf("姓名: 性别: 工号:\n");
	for(int i=0; i<count_tea_at_school; i++)
	{
		printf("%s\t %s\t %d\n", tea[i].name, 'm'==tea[i].sex?"男":"女", tea[i].id);
	}
	anykey_continue();
}

// 显示离职教师
void admin_show_all_tea_dropout()
{
	FILE* frp = fopen("tea_dropout.txt","r");
    if(NULL == frp)
    {
        printf("*** 暂无离职教师! ***");
		anykey_continue();
        return;
    }
    
    clear_screen();
	printf("********学生管理系统********\n");
	printf("--------------------------\n");
    printf("******所有离职教师******\n");
    teacher tch_d;
    while(fread(&tch_d,sizeof(teacher),1,frp))
	{
		printf("%s\t %s\t %d\n", tch_d.name, 'm'==tch_d.sex?"男":"女", tch_d.id);
	}
    fclose(frp);
    
    anykey_continue();
}

// 校长系统流程控制
void admin_start()
{
	admin_enter();
	while(flag_admin_success_enter)
	{	
		clear_screen();
		printf("********学生管理系统********\n");
		printf("--------------------------\n");
		printf("1.重置校长密码\n");
		printf("2.重置教师密码\n");
		printf("3.添加教师\n");
		printf("4.删除教师\n");
		printf("5.显示所有在职教师\n");
		printf("6.显示所有离职教师\n");
		printf("7.退出帐号\n");
		switch(get_cmd('1', '7'))
		{
			case '1': admin_reset_password(); break;
			case '2': admin_reset_tea_password(); break;
			case '3': admin_add_tea(); break;
			case '4': admin_remove_tea(); break;
			case '5': admin_show_all_tea_at_school(); break;
			case '6': admin_show_all_tea_dropout(); break;
			case '7': return;
		}
	}
}
