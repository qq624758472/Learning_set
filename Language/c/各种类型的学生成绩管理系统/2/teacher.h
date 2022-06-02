#ifndef TEACHER_H
#define TEACHER_H

#define MAX_TEA 50

typedef struct teacher
{
	char name[10]; // 姓名
	char sex; // 性别
	int id; // 工号
	char password[20]; // 密码
	int count_tea_password_wrong; // 密码错误次数
	int flag_tea_first_enter; // 第一次登入标志位
}teacher;

// 教师数组指针
extern teacher* tea;

// 记录当前登入教师 
extern int temp_tea;
// 教师人数
extern int count_tea_at_school;

// 判断教师是否存在
bool is_tea_id_exist(int id);
// 教师系统流程控制
void tea_start();

#endif
