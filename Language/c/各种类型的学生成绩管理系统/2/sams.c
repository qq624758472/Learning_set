#include "sams.h"
#include "tools.h"
#include "admin.h"
#include "teacher.h"
#include "student.h"

// 系统初始化
void sams_init(void)
{
	// 申请堆内存
	stu = calloc(MAX_STU, sizeof(student));
	tea = calloc(MAX_TEA, sizeof(teacher));

	// 加载数据
    count_tea_at_school = load_data("tea_data.txt", tea, sizeof(teacher));

    count_stu_at_school = load_data("stu_data.txt", stu, sizeof(student));
	
	FILE* frp_admin = fopen("admin_data.txt", "r");
	if(NULL == frp_admin)
	{
		return;
	}
	fread(admin_password, sizeof(admin_password), 1, frp_admin);
	fread(&flag_admin_first_enter, 4, 1, frp_admin);
	fclose(frp_admin);
}

// 系统开始运行
void sams_start(void)
{
	// 进入系统的业务流程控制
	while(true)
	{
		clear_screen();
		printf("***欢迎使用指针学生成绩管理系统***\n");
    	printf("---------------\n");
		printf("1、校长系统\n");
		printf("2、教师系统\n");
		printf("3、学生系统\n");
		printf("4、退出系统\n");
		printf("---------------\n");
		switch(get_cmd('1','4'))
		{
			case '1': admin_start(); break;
			case '2': tea_start(); break;
			case '3': stu_start(); break;
			case '4': return;
		}
	}
}

// 系统结束
void sams_exit(void)
{
	// 保存数据
	save_data("tea_data.txt", tea, sizeof(teacher), count_tea_at_school);
    free(tea);

    save_data("stu_data.txt", stu, sizeof(student), count_stu_at_school);
    free(stu);
	
    FILE* fwp_admin = fopen("admin_data.txt", "w");
	fwrite(admin_password, sizeof(admin_password), 1, fwp_admin);
	fwrite(&flag_admin_first_enter, 4, 1, fwp_admin);
	fclose(fwp_admin);
}
