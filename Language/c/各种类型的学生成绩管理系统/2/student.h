#ifndef STUDENT_H
#define STUDENT_H

#define MAX_STU 100

typedef struct student
{
	char name[10]; // 姓名
	char sex; // 性别
	int id; // 学号
	char password[20]; // 密码
	float Chinese; // 语文成绩
	float Math; // 数学成绩
	float English; // 英语成绩
	float Sum;   // 总分
    int rank;      // 排名
	int count_stu_password_wrong; // 密码错误次数
	int flag_stu_first_enter; // 第一次登入
}student;

// 学生数组指针
extern student* stu;

// 记录当前学生
extern int temp_stu;
// 学生人数
extern int count_stu_at_school;

// 教师系统流程控制
void stu_start();
// 判断学生是否存在
bool is_stu_id_exist(int id);

#endif //STUDENT_H
