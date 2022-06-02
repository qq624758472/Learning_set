#include "tools.h"
#include "teacher.h"
#include "student.h"
#include "stddef.h"
// 教师数组指针
teacher* tea;

int temp_tea; // 记录工号所对应的教师 
int count_tea_at_school; // 教师人数

bool flag_tea_success_enter; // 教师成功登入标志位

// 判断教师是否存在
bool is_tea_id_exist(int id)
{
	int i;
	for(i=0; i<count_tea_at_school; i++)
	{
		if(id == tea[i].id)
		{
			temp_tea = i;
			return true;
		}
	}
	return false; 
}

// 教师登入
void tea_enter()
{
	int temp_id;
	char temp_password[20];
	bool flag_right_id = true;
	flag_tea_success_enter = false;
	while(true)
	{
		clear_screen();
		printf("********学生管理系统********\n");
		printf("--------------------------\n");
		if(flag_right_id)
		{
      		printf("工号:");
			scanf("%d", &temp_id);
			getchar();
		}
		else
		{
 			printf("工号:%d\n", temp_id);
		}
		if(is_tea_id_exist(temp_id))
		{
			flag_right_id = false;
			printf("密码:");
			get_passwd(temp_password, true, 20);
			if(is_password_right(temp_password, tea[temp_tea].password) && tea[temp_tea].count_tea_password_wrong < 3)
			{
				tea[temp_tea].count_tea_password_wrong = 0;
				flag_tea_success_enter = true;
				break;
			}
			else
			{
				tea[temp_tea].count_tea_password_wrong ++;
				if(tea[temp_tea].count_tea_password_wrong >= 3)
				{
					printf("三次密码错误,帐号被锁定,由校长解锁!");
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
			printf("工号错误!");
			anykey_continue();
		}
	}
	if(0 == tea[temp_tea].flag_tea_first_enter)
	{
		clear_screen();
		printf("********学生管理系统********\n");
		printf("--------------------------\n");
		printf("第一次登入，请修改密码:");
		get_passwd(tea[temp_tea].password, true, 20);
		tea[temp_tea].flag_tea_first_enter = 1;
	}
}

// 单独添加学生
void add_stu_single()
{
	if(count_stu_at_school >= MAX_STU)
    {
        printf("学生已满,无法添加!");
		anykey_continue();
        return;
    }
	clear_screen();
	bzero(stu+count_stu_at_school, sizeof(student));
	printf("********学生管理系统********\n");
	printf("--------------------------\n");
	printf("姓名:  性别(m.男 w.女):\n");
	scanf("%s %c", stu[count_stu_at_school].name, &stu[count_stu_at_school].sex);
	stu[count_stu_at_school].id = 201600000 + count_stu_at_school;
	strcpy(stu[count_stu_at_school].password, "123456");
	count_stu_at_school ++;
}

// 批量添加学生
void add_stu_volume()
{	
	clear_screen();
	printf("********学生管理系统********\n");
	printf("--------------------------\n");
	char path[30] = {};
    printf("请输入要导入的文件名:");
    get_str(path,30);
    FILE* frp = fopen(path,"r");
    if(NULL == frp)
    {
        printf("输入的路径有误!");
		anykey_continue();
		return;
    }
    
    student* stu_p = stu + count_stu_at_school;
    size_t num_r = 0;
    size_t num_s = 0;
    while(count_stu_at_school < MAX_STU)
    {
        bzero(stu_p, sizeof(student));
        if(-1 == fscanf(frp,"%s %c\n", stu_p->name, &stu_p->sex))
		{
            break;
		}
        num_r++;
        if('w' != stu_p->sex && 'm' != stu_p->sex)
		{
			continue;
		}
        stu_p->id = 201600000 + count_stu_at_school;
        strcpy(stu_p->password, "123456");
        
        count_stu_at_school ++;
        stu_p ++;
        num_s ++;
    }
    printf("*** 共读取到%u条数据,成功添加%u位学生! ***\n",num_r,num_s);
    anykey_continue();  
}

// 添加学生流程控制
void tea_add_stu()
{
	while(true)
	{
		clear_screen();
		printf("********学生管理系统********\n");
		printf("--------------------------\n");
		printf("1.单个添加\n");
		printf("2.批量添加\n");
		printf("3.返回\n");
		switch(get_cmd('1', '3'))
		{
			case '1': add_stu_single(); break;
			case '2': add_stu_volume(); break;
			case '3': return;
		}
	}
}

// 删除学生
void tea_remove_stu()
{
    int id1, id2;
	clear_screen();
    printf("********学生管理系统********\n");
    printf("--------------------------\n");
	printf("输入学号:");
	scanf("%d", &id1);
	printf("确认学号:");
	scanf("%d", &id2);
	getchar();
    if(id1 == id2 && is_stu_id_exist(id1))
	{
    	FILE* fap = fopen("stu_dropout.txt", "a");
    	fwrite(stu+temp_stu, sizeof(student), 1, fap);
    	fclose(fap);
        
    	memcpy(stu+temp_stu, stu+temp_stu+1, (count_stu_at_school-temp_stu-1)*sizeof(student));
		count_stu_at_school --;
    	printf("删除帐号成功!");
		anykey_continue();
   	}
   	else
   	{
	   	printf("学号输入错误!");
		anykey_continue();
   	}
}

// 通过姓名查找学生
void search_stu_by_name()
{
	clear_screen();
	printf("********学生管理系统********\n");
	printf("--------------------------\n");
	printf("输入姓名:");
	char name[10];
	int  count = 0;
	scanf("%s", name);
	for(int i=0; i<count_stu_at_school; i++)
	{
		if(0 == strcmp(name, stu[i].name))
		{
			printf("%s %s %d\n", stu[i].name, 'm'==stu[i].sex?"男":"女", stu[i].id);
			count ++;
		}
	}
	if(0 == count)
	{
		printf("查无此人!");
		anykey_continue();
	}
	else
	{
		anykey_continue();
	}
}

// 通过学号查找学生
void search_stu_by_id()
{	
	clear_screen();
	printf("********学生管理系统********\n");
	printf("--------------------------\n");
	printf("输入学号:");
	int id;
	bool flag = true;
	scanf("%d", &id);
	getchar();
	for(int i=0; i<count_stu_at_school; i++)
	{
		if(id == stu[i].id)
		{
			printf("%s %s %d\n", stu[i].name, 'm'==stu[i].sex?"男":"女", stu[i].id);
			flag = false;
			anykey_continue();
			break;
		}
	}
	if(flag)
	{
		printf("查无此人!");
		anykey_continue();
	}
}

// 查找学生
void tea_search_stu()
{
	while(true)
	{
		clear_screen();
		printf("********学生管理系统********\n");
		printf("--------------------------\n");
		printf("1.按姓名查找\n");
		printf("2.按学号查找\n");
		printf("3.返回\n");
		switch(get_cmd('1', '3'))
		{
			case '1': search_stu_by_name(); break;
			case '2': search_stu_by_id(); break;
			case '3': return;
		}
	}
}

// 修改学生基础信息
void change_stu_information()
{
	clear_screen();
	printf("********学生管理系统********\n");
	printf("--------------------------\n");
	printf("输入修改学生的学号:");
   	int id;
   	bool flag = true;
   	scanf("%d", &id);
	getchar();
   	for(int i=0; i<count_stu_at_school; i++)
   	{
		if(id == stu[i].id)
	   	{
	    	flag = false;
			printf("姓名:%s   性别:%s\n", stu[i].name, 'm'==stu[i].sex?"男":"女");
			printf("修改姓名:  性别(m.男 w.女):\n");
			scanf("%s %c", stu[i].name, &stu[i].sex);
	    	break;
		}
    }
   	if(flag)
   	{
  		printf("学号错误!");
    	anykey_continue();
   	}
}

// 修改学生成绩
void change_stu_grade()
{
	clear_screen();
	printf("********学生管理系统********\n");
	printf("--------------------------\n");
	printf("输入修改学生的学号:");
   	int id;
   	bool flag = true;
   	scanf("%d", &id);
	getchar();
   	for(int i=0; i<count_stu_at_school; i++)
   	{
		if(id == stu[i].id)
	   	{
	    	flag = false;
			printf("语文:%g  数学:%g  英语:%g\n", stu[i].Chinese, stu[i].Math, stu[i].English);
			printf("修改语文:  数学:  英语:\n");
			scanf("%f %f %f", &stu[i].Chinese, &stu[i].Math, &stu[i].English);
			stu[i].Sum = stu[i].Chinese + stu[i].Math + stu[i].English;
	    	break;
		}
    }
   	if(flag)
   	{
  		printf("学号错误!");
    	anykey_continue();
   	}
}

// 修改学生信息流程控制
void tea_change_stu_information()
{
	while(true)
	{
		clear_screen();
		printf("********学生管理系统********\n");
		printf("--------------------------\n");
		printf("1.修改学生基础信息\n");
		printf("2.修改学生成绩\n");
		printf("3.返回\n");
		switch(get_cmd('1', '3'))
		{
			case '1': change_stu_information(); break;
			case '2': change_stu_grade(); break;
			case '3': return;
		}
	}
}

// 单独录入学生成绩
void add_stu_grade_single()
{	
	clear_screen();
	printf("********学生管理系统********\n");
	printf("--------------------------\n");
	printf("输入学生的学号:");
   	int id;
   	bool flag = true;
   	scanf("%d", &id);
	getchar();
   	for(int i=0; i<count_stu_at_school; i++)
   	{
		if(id == stu[i].id)
	   	{
	    	flag = false;
			printf("语文: 数学: 英语:\n");
			scanf("%f %f %f", &stu[i].Chinese, &stu[i].Math, &stu[i].English);
			stu[i].Sum = stu[i].Chinese + stu[i].Math + stu[i].English;
	    	break;
		}
    }
   	if(flag)
   	{
  		printf("学号错误!");
    	anykey_continue();
	}
}

// 批量录入学生成绩
void add_stu_grade_volume()
{
	clear_screen();
	printf("********学生管理系统********\n");
	printf("--------------------------\n");
	char path[30] = {};
    printf("请输入要导入的文件名:");
    get_str(path,30);
    FILE* frp = fopen(path,"r");
    if(NULL == frp)
    {
        printf("输入的路径有误!");
		anykey_continue();
    }
    
    size_t num_r = 0;
    size_t num_s = 0;
    int id = 0;
    float sc = 0 , sm = 0 , se = 0;
    while(-1 != fscanf(frp, "%d %f %f %f\n", &id, &sc, &sm, &se))
    {      
        num_r++;
         
        if(!is_stu_id_exist(id))
		{
			continue;
		}
        if(sc < 0 || sc > 100 || sm < 0 || sm > 100 || se < 0 || se > 100)
		{
            continue;
		}
        stu[temp_stu].Chinese = sc;
        stu[temp_stu].Math = sm;
        stu[temp_stu].English = se;
		stu[temp_stu].Sum = sc + sm + se;
        num_s++;
    }
    printf("*** 共读取到%u条数据,成功添加%u位教师! ***\n",num_r,num_s);
    anykey_continue();
}

// 录入学生成绩流程控制
void tea_add_stu_grade()
{
	while(true)
	{
		clear_screen();
		printf("********学生管理系统********\n");
		printf("--------------------------\n");
		printf("1.单个录入\n");
		printf("2.批量录入\n");
		printf("3.返回\n");
		switch(get_cmd('1', '3'))
		{
			case '1': add_stu_grade_single(); break;
			case '2': add_stu_grade_volume(); break;
			case '3': return;
		}
	}
}

// 复位学生密码
void tea_reset_stu_password()
{
	clear_screen();
	printf("********学生管理系统********\n");
	printf("--------------------------\n");
	printf("输入学生的学号:");
   	int id;
   	scanf("%d", &id);
	getchar();
	if(is_stu_id_exist(id))
	{
		strcpy(stu[temp_stu].password, "123456");
		stu[temp_stu].flag_stu_first_enter = 0;
		stu[temp_stu].count_stu_password_wrong = 0;
		printf("重置成功!");
		anykey_continue();
    }
   	else
   	{
  		printf("学号错误!");
    	anykey_continue();
	}
}

// 显示在校学生
void tea_show_all_stu_information_at_school()
{
	clear_screen();
	printf("********学生管理系统********\n");
	printf("--------------------------\n");
	printf("姓名: 性别: 学号:\n");
	for(int i=0; i<count_stu_at_school; i++)
	{
		printf("%s\t %s\t %d\n", stu[i].name, 'm'==stu[i].sex?"男":"女", stu[i].id);
	}
	anykey_continue();
}

// 显示退学学生
void tea_show_all_stu_information_dropout()
{
	FILE* frp = fopen("stu_dropout.txt","r");
    if(NULL == frp)
    {
        printf("暂无退学学生!");
        anykey_continue();
    }
    
    clear_screen();
    puts("******所有退学学生******");
    student stu_d;
    while(fread(&stu_d, sizeof(student), 1, frp))
	{
		printf("%s\t %s\t %d\n", stu_d.name, 'm'==stu_d.sex?"男":"女", stu_d.id);
	}
    fclose(frp);
    
    anykey_continue();
}

// 教师系统流程控制
void tea_start()
{
	tea_enter();
	while(flag_tea_success_enter)
	{
		clear_screen();
		printf("********学生管理系统********\n");
		printf("--------------------------\n");
		printf("1.添加学生\n");
		printf("2.删除学生\n");
		printf("3.查找学生\n");
		printf("4.修改学生信息\n");
		printf("5.录入学生成绩\n");
		printf("6.重置学生密码\n");
		printf("7.显示所有在校学生信息\n");
		printf("8.显示所有退学学生信息\n");
		printf("9.退出帐号\n");
		switch(get_cmd('1', '9'))
		{
			case '1': tea_add_stu(); break;
			case '2': tea_remove_stu(); break;
			case '3': tea_search_stu(); break;
			case '4': tea_change_stu_information(); break;
			case '5': tea_add_stu_grade(); break;
			case '6': tea_reset_stu_password(); break;
			case '7': tea_show_all_stu_information_at_school(); break;
			case '8': tea_show_all_stu_information_dropout(); break;
			case '9': return;
		}
	}
}
