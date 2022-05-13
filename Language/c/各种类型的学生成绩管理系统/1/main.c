#include <stdio.h>
#include <string.h>
#include <math.h>
#include "malloc.h"
#include <stdlib.h>

#define MAX_LEN 10	 /* 字符串最大长度 */
#define STU_NUM 30	 /* 最多的学生人数 */
#define COURSE_NUM 6 /* 最多的考试科目数 */
#define LEN sizeof(struct Student)

typedef struct Student
{
	long num;				 /* 每个学生的学号 */
	char name[MAX_LEN];		 /* 每个学生的姓名 */
	float score[COURSE_NUM]; /* 每个学生COURSE_NUM门功课的成绩 */
	float sum;				 /* 每个学生的总成绩 */
	float aver;				 /* 每个学生的平均成绩 */
	struct Student *next;
} STU;

int Menu(void);										 //创建菜单
void Print(STU *head, int n, int m);				 //打印函数
void AverSumofEveryStudent(STU *head, int n, int m); //计算每门课程的总分和平均分
void AverSumofEveryCourse(STU *head, int n, int m);	 //计算每个学生的总分和平均分
STU *SortbyScore(STU *head, int n);					 //按每个学生的总分由高到低排出名次表
STU *Creat(int n, int m);							 //创建链表并录入信息
STU *Creat1(int n, int m);
STU *SortbyScore1(STU *head, int n);			 //按每个学生的总分由低到高排出名次表
STU *SortbyNum(STU *head);						 //按学号由小到大排出成绩表
STU *SortbyName(STU *head, int n);				 //按姓名的字典顺序排出成绩表
void SearchbyNum(STU *head, int n, int m);		 //按学号查询学生排名及其考试成绩
void SearchbyName(STU *head, int n, int m);		 //按姓名查询学生排名及其考试成绩
void StatisticAnalysis(STU *head, int n, int m); //按类别及比例输出
void WritetoFile(STU *head, int n, int m);		 //将每个学生的纪录信息写入文件
STU *ReadfromFile(STU *head, int *n, int *m);	 //从文件中读出每个学生的纪录信息并显示
STU *change_student_info(STU *head, int *n, int *m);
STU *del_student_info(STU *head, int *n, int *m);
int main()
{
	int n, m;
	int i;
	STU *head; //定义头节点
	head = (STU *)malloc(LEN);
	while (1)
	{
		i = Menu();
		if (i == 1)
		{
			system("clear"); //清屏
			printf("\t\t\t******************************************************************************\n");
			printf("\t\t\t请输入创建的总学生数(n<30):\n"); //输入学生数
			printf("\t\t\t");
			scanf("%d", &n);
			printf("\t\t\t请输入具有多少门课程(m<=6):\n");
			printf("\t\t\t");
			scanf("%d", &m);
		}
		switch (i)
		{
		case 1:
			printf("\t\t\t请输入学生信息：  学号、姓名、成绩:\n");
			head = Creat(n, m);
			system("clear"); //清屏
			break;
		case 2:
			system("clear"); //清屏
			AverSumofEveryStudent(head, n, m);
			break;
		case 3:
			system("clear"); //清屏
			AverSumofEveryCourse(head, n, m);
			break;
		case 4:
			system("clear"); //清屏
			printf("\n\n\n");
			printf("\t\t\t按分数降序排序:\n");
			head = SortbyScore(head, n);
			Print(head, n, m);
			break;
		case 5:
			system("clear"); //清屏
			printf("\n\n\n");
			printf("\t\t\t按分数升序排序:\n");
			head = SortbyScore1(head, n);
			Print(head, n, m);
			break;
		case 6:
			system("clear"); //清屏
			printf("\n\n\n");
			printf("\t\t\t按编号升序排序:\n");
			head = SortbyNum(head);
			Print(head, n, m);
			break;
		case 7:
			system("clear"); //清屏
			printf("\n\n\n");
			printf("\t\t\t按字典顺序名称排序:\n");
			head = SortbyName(head, n);
			Print(head, n, m);
			break;
		case 8:
			system("clear"); //清屏
			printf("\n\n\n");
			printf("\t\t\t******************************************************************************\n");
			printf("\t\t\t输入您要搜索的号码:\n");
			SearchbyNum(head, n, m);
			break;
		case 9:
			system("clear"); //清屏
			printf("\n\n\n");
			printf("\t\t\t******************************************************************************\n");
			printf("\t\t\t输入要搜索的名称:\n");
			SearchbyName(head, n, m);
			break;
		case 10:
			system("clear"); //清屏
			StatisticAnalysis(head, n, m);
			break;
		case 11:
			system("clear"); //清屏
			printf("\n\n\n");
			Print(head, n, m);
			break;
		case 12:
			system("clear"); //清屏
			WritetoFile(head, n, m);
			break;
		case 13:
			system("clear"); //清屏
			head = ReadfromFile(head, &n, &m);
			break;
		case 14:			 //修改学生信息
			system("clear"); //清屏
			printf("\t\t\t输入您要改变信息的学生号:\n");
			head = change_student_info(head, &n, &m);
			break;
		case 15:			 //修改学生信息
			system("clear"); //清屏
			printf("\t\t\t输入您要删除信息的学生号:\n");
			head = del_student_info(head, &n, &m);
			break;
		case 0:
			system("clear"); //清屏
			printf("\n\n\n");
			printf("\t\t\t******************************************************************************\n");
			printf("\t\t\t程序结束!\n");
			printf("\t\t\t******************************************************************************\n");
			return 0;
		default:
			system("clear"); //清屏
			printf("\n\n\n");
			printf("\t\t\t******************************************************************************\n");
			printf("\t\t\t输入错误!\n");
			printf("\t\t\t******************************************************************************\n");
		}
	}

	return 0;
}
// 创建菜单
int Menu(void)
{
	int i;
	// system("title 学生成绩管理系统");
	printf("\n\n");
	printf("\t\t\t                         学生成绩管理系统\n");
	printf("\t\t\t******************************************************************************\n");
	printf("\t\t\t*                学生成绩管理系统                                               *\n");
	printf("\t\t\t*                1.  输入记录                                                  *\n");
	printf("\t\t\t*                2.  计算每门课程的总分和平均分                                   *\n");
	printf("\t\t\t*                3.  计算每个学生的总分和平均分                                   *\n");
	printf("\t\t\t*                4.  按分数降序排序                                             *\n");
	printf("\t\t\t*                5.  按分数升序排序                                             *\n");
	printf("\t\t\t*                6.  按编号升序排序                                             *\n");
	printf("\t\t\t*                7.  按字典顺序名称排序                                          *\n");
	printf("\t\t\t*                8.  按号码搜索                                                *\n");
	printf("\t\t\t*                9.  按名称搜索                                                *\n");
	printf("\t\t\t*                10. 统计分析                                                  *\n");
	printf("\t\t\t*                11. 列表记录                                                  *\n");
	printf("\t\t\t*                12. 写入文件                                                  *\n");
	printf("\t\t\t*                13. 从文件中读取                                               *\n");
	printf("\t\t\t*                14. 修改学生信息                                               *\n");
	printf("\t\t\t*                15. 删除学生信息                                               *\n");
	printf("\t\t\t*                0.  退出                                                    *\n");
	printf("\t\t\t******************************************************************************\n");
	printf("\n");
	printf("\n");
	printf("\n");
	printf("\n");
	printf("\t\t\t*****************输入操作:");
	scanf("%d", &i);
	return i;
}

// 创建链表
STU *Creat(int n, int m)
{
	STU *head;
	STU *p1, *p2;
	int i, j;
	// system("clear");
	for (i = 1; i < n + 1; i++)
	{
		p1 = (STU *)malloc(LEN);
		printf("请输入学生学号\t\t\t");
		scanf("%ld", &p1->num);
		printf("请输入学生姓名\t\t\t");
		scanf("%s", p1->name);
		for (j = 0; j < m; j++)
		{
			printf("请输入学生：%s  科目[%d]的成绩\t\t\t", p1->name, j);
			scanf("%f", &p1->score[j]);
		}
		p1->next = NULL;
		if (i == 1)
		{
			head = p2 = p1;
		}
		else
		{
			p2->next = p1;
			p2 = p1;
		}
	}
	return (head);
}

STU *Creat1(int n, int m)
{
	STU *head;
	STU *p1, *p2;
	int i, j;
	// system("clear");
	for (i = 1; i < n + 1; i++)
	{
		p1 = (STU *)malloc(LEN);
		p1->next = NULL;
		if (i == 1)
		{
			head = p2 = p1;
		}
		else
		{
			p2->next = p1;
			p2 = p1;
		}
	}
	return (head);
}
void AverSumofEveryStudent(STU *head, int n, int m)
{
	STU *p;
	int i, j = 1;
	char ch;
	float sum;
	p = head;
	if (head != NULL)
	{
		printf("\t\t\t******************************************************************************\n");
		for (i = 0; i < m; i++)
		{
			p = head;
			sum = 0.0;
			do
			{
				sum += p->score[i];
				p = p->next;
			} while (p != NULL);

			printf("\t\t\tcourse %d:    sum=%.0f , aver=%.0f\n", j, sum, sum / n);
			j++;
		}
		printf("\t\t\t******************************************************************************\n");
	}
}

void AverSumofEveryCourse(STU *head, int n, int m)
{
	STU *p;
	int i, j;
	p = head;
	if (head != NULL)
	{
		printf("\n\n\n");
		printf("\t\t\t******************************************************************************\n");
		for (i = 0; i < n; i++)
		{
			p->sum = 0.0;
			for (j = 0; j < m; j++)
			{
				p->sum += p->score[j];
			}
			p->aver = p->sum / m;
			printf("\t\t\tstudent %d:    sum=%.0f , aver=%.0f\n", i + 1, p->sum, p->aver);
			p = p->next;
		}
		printf("\t\t\t******************************************************************************\n");
	}
}

STU *SortbyScore(STU *head, int n)
{
	STU *endpt; //控制循环比较
	STU *p;		//临时指针变量
	STU *p1, *p2;

	p1 = (STU *)malloc(LEN);
	p1->next = head; //注意理解：我们增加一个节点，放在第一个节点的前面，主要是为了便于比较。因为第一个节点没有前驱，我们不能交换地址
	head = p1;		 //让head指向p1节点，排序完成后，我们再把p1节点释放掉

	for (endpt = NULL; endpt != head; endpt = p)
	{
		for (p = p1 = head; p1->next->next != endpt; p1 = p1->next)
		{
			if (p1->next->sum < p1->next->next->sum) //如果前面的节点键值比后面节点的键值大，则交换
			{
				p2 = p1->next->next;
				p1->next->next = p2->next;
				p2->next = p1->next;
				p1->next = p2;
				p = p1->next->next;
			}
		}
	}

	p1 = head;		   //把p1的信息去掉
	head = head->next; //让head指向排序后的第一个节点
	free(p1);		   //释放p1
	p1 = NULL;		   // p1置为NULL，保证不产生“野指针”，即地址不确定的指针变量

	return head;
}

STU *SortbyScore1(STU *head, int n)
{
	STU *endpt; //控制循环比较
	STU *p;		//临时指针变量
	STU *p1, *p2;

	p1 = (STU *)malloc(LEN);
	p1->next = head; //注意理解：我们增加一个节点，放在第一个节点的前面，主要是为了便于比较。因为第一个节点没有前驱，我们不能交换地址
	head = p1;		 //让head指向p1节点，排序完成后，我们再把p1节点释放掉

	for (endpt = NULL; endpt != head; endpt = p)
	{
		for (p = p1 = head; p1->next->next != endpt; p1 = p1->next)
		{
			if (p1->next->sum > p1->next->next->sum) //如果前面的节点键值比后面节点的键值大，则交换
			{
				p2 = p1->next->next;
				p1->next->next = p2->next;
				p2->next = p1->next;
				p1->next = p2;
				p = p1->next->next;
			}
		}
	}

	p1 = head;		   //把p1的信息去掉
	head = head->next; //让head指向排序后的第一个节点
	free(p1);		   //释放p1
	p1 = NULL;		   // p1置为NULL，保证不产生“野指针”，即地址不确定的指针变量

	return head;
}

STU *SortbyNum(STU *head)
{
	STU *first; //为原链表剩下用于直接插入排序的节点头指针
	STU *t;		//临时指针变量：插入节点
	STU *p, *q; //临时指针变量

	first = head->next; //原链表剩下用于直接插入排序的节点链表：可根据图12来理解
	head->next = NULL;	//只含有一个节点的链表的有序链表：可根据图11来理解

	while (first != NULL) //遍历剩下无序的链表
	{
		//注意：这里for语句就是体现直接插入排序思想的地方
		for (t = first, q = head; ((q != NULL) && (q->num < t->num)); p = q, q = q->next)
			; //无序节点在有序链表中找插入的位置

		first = first->next; //无序链表中的节点离开，以便它插入到有序链表中

		if (q == head) //插在第一个节点之前
		{
			head = t;
		}
		else // p是q的前驱
		{
			p->next = t;
		}
		t->next = q; //完成插入动作
					 // first = first->next;
	}
	return head;
}

STU *SortbyName(STU *head, int n)
{
	STU *endpt; //控制循环比较
	STU *p;		//临时指针变量
	STU *p1, *p2;

	p1 = (STU *)malloc(LEN);
	p1->next = head; //注意理解：我们增加一个节点，放在第一个节点的前面，主要是为了便于比较。因为第一个节点没有前驱，我们不能交换地址
	head = p1;		 //让head指向p1节点，排序完成后，我们再把p1节点释放掉

	for (endpt = NULL; endpt != head; endpt = p)
	{
		for (p = p1 = head; p1->next->next != endpt; p1 = p1->next)
		{
			if (strcmp(p1->next->name, p1->next->next->name) > 0) //如果前面的节点键值比后面节点的键值大，则交换
			{
				p2 = p1->next->next;
				p1->next->next = p2->next;
				p2->next = p1->next;
				p1->next = p2;		//结合第4点理解
				p = p1->next->next; //结合第6点理解
			}
		}
	}

	p1 = head;		   //把p1的信息去掉
	head = head->next; //让head指向排序后的第一个节点
	free(p1);		   //释放p1
	p1 = NULL;		   // p1置为NULL，保证不产生“野指针”，即地址不确定的指针变量

	return head;
}

void Print(STU *head, int n, int m)
{
	STU *p;
	int i;
	p = head;

	if (head != NULL) //只要不是空链表，就输出链表中所有节点
	{
		printf("\t\t\t******************************************************************************\n");
		do
		{

			printf("\t\t\t%ld\t%s\t", p->num, p->name);
			for (i = 0; i < m; i++)
			{
				printf("%.0f\t", p->score[i]);
			}
			printf("%.0f\t%.0f\n", p->sum, p->sum / m);
			p = p->next; //移到下一个节点
		} while (p != NULL);
		printf("\t\t\t******************************************************************************\n");
	}
}

void SearchbyNum(STU *head, int n, int m)
{
	long num;
	int i;
	int flag = 1;
	printf("\t\t\t");
	scanf("%ld", &num);
	STU *p;
	p = head;
	if (head != NULL)
	{
		do
		{
			if (p->num == num)
			{
				printf("\t\t\t");
				printf("%ld\t%s\t", p->num, p->name);
				for (i = 0; i < m; i++)
				{
					printf("%.0f\t", p->score[i]);
				}
				printf("%.0f\t%.0f\n", p->sum, p->sum / m);
				flag = 0;
			}
			p = p->next;

		} while (p != NULL);
		if (flag)
		{
			printf("\t\t\t");
			printf("没有找到!\n");
		}
	}
	printf("\t\t\t******************************************************************************\n");
}

void SearchbyName(STU *head, int n, int m)
{
	char name[MAX_LEN];
	int i;
	int flag = 1;
	printf("\t\t\t");
	scanf("%s", name);
	STU *p;
	p = head;
	if (head != NULL)
	{
		do
		{
			if (strcmp(name, p->name) == 0)
			{
				printf("\t\t\t");
				printf("%ld\t%s\t", p->num, p->name);
				for (i = 0; i < m; i++)
				{
					printf("%.0f\t", p->score[i]);
				}
				printf("%.0f\t%.0f\n", p->sum, p->sum / m);
				flag = 0;
			}
			p = p->next;

		} while (p != NULL);
		if (flag)
		{
			printf("\t\t\t");
			printf("没有找到!\n");
		}
	}
	printf("\t\t\t******************************************************************************\n");
}

void StatisticAnalysis(STU *head, int n, int m)
{
	int a[6];
	STU *p;
	p = head;
	int i, j;
	for (i = 0; i < m; i++)
	{
		p = head;				// 不要忘了初始化，不然会炸
		for (j = 0; j < 6; j++) // 初始化
		{
			a[j] = 0;
		}
		do
		{
			if (p->score[i] < 60)
			{
				a[0]++;
			}
			else if (p->score[i] < 70)
			{
				a[1]++;
			}
			else if (p->score[i] < 80)
			{
				a[2]++;
			}
			else if (p->score[i] < 90)
			{
				a[3]++;
			}
			else if (p->score[i] < 100)
			{
				a[4]++;
			}
			else
			{
				a[5]++;
			}

			p = p->next;

		} while (p != NULL);
		printf("\n\n\n");
		printf("\t\t\t******************************************************************************\n");
		printf("\t\t\t* For course %d:\n", i + 1);
		printf("\t\t\t* <60\t%d\t%.2f%%\n", a[0], 100 * a[0] / 6.0);
		printf("\t\t\t* %d-%d\t%d\t%.2f%%\n", 60, 69, a[1], 100 * a[1] / 6.0);
		printf("\t\t\t* %d-%d\t%d\t%.2f%%\n", 70, 79, a[2], 100 * a[2] / 6.0);
		printf("\t\t\t* %d-%d\t%d\t%.2f%%\n", 80, 89, a[3], 100 * a[3] / 6.0);
		printf("\t\t\t* %d-%d\t%d\t%.2f%%\n", 90, 99, a[4], 100 * a[4] / 6.0);
		printf("\t\t\t* %d\t%d\t%.2f%%\n", 100, a[5], 100 * a[5] / 6.0);
		printf("\t\t\t******************************************************************************\n");
	}
}

void WritetoFile(STU *head, int n, int m)
{
	STU *p;
	p = head;
	FILE *fp;
	int i, j;
	printf("\n\n\n");
	printf("\t\t\t******************************************************************************\n");
	if ((fp = fopen("student.txt", "w")) == NULL)
	{
		printf("\t\t\t打开文件失败\n");
		return;
	}
	fprintf(fp, "%d\t%d\n", n, m);
	for (i = 0; i < n; i++)
	{
		fprintf(fp, "%12ld%12s", p->num, p->name);
		for (j = 0; j < m; j++)
		{
			fprintf(fp, "%12.0f", p->score[j]);
		}
		fprintf(fp, "%12.0f%12.0f\n", p->sum, p->sum / m);

		p = p->next;
	}
	printf("\t\t\t导出成功!\n");
	fclose(fp);
	printf("\t\t\t******************************************************************************\n");
}

/*
void  ReadfromFile(STU1 stu[], int *n, int *m)
{
FILE *fp;
int i, j;
printf("\n\n\n");
printf("\t\t\t******************************************************************************\n");
if ((fp = fopen("student.txt", "r")) == NULL)
{
printf("\t\t\tFail to open student.txt\n");
return;
}
fscanf(fp, "%d\t%d\n", n, m);
for (i = 0; i<*n; i++)
{
fscanf(fp, "%12ld", &stu[i].num);
fscanf(fp, "%12s", stu[i].name);
for (j = 0; j<*m; j++)
{
fscanf(fp, "%12f", &stu[i].score[j]);
}
fscanf(fp, "%12f%12f", &stu[i].sum, &stu[i].aver);
}
printf("\t\t\tImport Successfully!\n");
fclose(fp);
for (i = 0; i < *n; i++)
{
printf("\t\t\t%ld\t%s\t", stu[i].num, stu[i].name);
for (j = 0; j<*m; j++)
{
printf("%.0f\t", stu[i].score[j]);
}
printf("%.0f\t%.0f\n", stu[i].sum, stu[i].aver);
}
printf("\t\t\t******************************************************************************\n");
}
*/

//这里用了直接生成了一个新的链表，确保可以直接读取存好的数据
STU *ReadfromFile(STU *head, int *n, int *m)
{
	STU *p;
	FILE *fp;
	int i, j;
	if ((fp = fopen("student.txt", "r")) == NULL)
	{
		printf("\t\t\tFail to open student.txt\n");
		return NULL;
	}
	fscanf(fp, "%d\t%d\n", n, m);
	head = Creat1(*n, *m); //创建了一个空链表，并且赋给head
	p = head;
	for (i = 0; i < *n; i++)
	{
		fscanf(fp, "%12ld", &p->num);
		fscanf(fp, "%12s", &p->name);
		for (j = 0; j < *m; j++)
		{
			fscanf(fp, "%12f", &p->score[j]);
		}
		fscanf(fp, "%12f%12f", &p->sum, &p->aver);

		p = p->next;
	}
	i = *n;
	j = *m;
	printf("\t\t\tImport Successfully!\n");
	fclose(fp);

	Print(head, i, j);

	return head;
}


STU *del_student_info(STU *head, int *n, int *m)
{
	long num;
	int i;
	int flag = 1;
	printf("\t\t\t");
	scanf("%ld", &num);
	STU *p, *p_recv = NULL;
 	p_recv = p = head;
	if (head != NULL)
	{
		do
		{
			if (p->num == num)
			{
				printf("\t\t\t");
				printf("%ld\t%s\t", p->num, p->name);
				for (i = 0; i < *m; i++)
				{
					printf("%.0f\t", p->score[i]);
				}
				printf("%.0f\t%.0f\n", p->sum, p->sum / *m);
				flag = 0;
				break;
			}
			p_recv = p;
			p = p->next;
		} while (p != NULL);
		if (flag)
		{
			printf("\t\t\t");
			printf("没有找到需要修改的学生!\n");
		}
		else //查找到该学生
		{
			if(p_recv == NULL || p_recv== head)//删除的是第一个节点
			{
				head = p->next;
			}
			else
			{
				p_recv->next = p->next;
			}
			free(p);	
			(*n) -=1;		
		}
	}
	return head;
	printf("\t\t\t******************************************************************************\n");
}
//修改学生信息
STU *change_student_info(STU *head, int *n, int *m)
{
	long num;
	int i;
	int flag = 1;
	printf("\t\t\t");
	scanf("%ld", &num);
	STU *p;
	p = head;
	if (head != NULL)
	{
		do
		{
			if (p->num == num)
			{
				printf("\t\t\t");
				printf("%ld\t%s\t", p->num, p->name);
				for (i = 0; i < *m; i++)
				{
					printf("%.0f\t", p->score[i]);
				}
				printf("%.0f\t%.0f\n", p->sum, p->sum / *m);
				flag = 0;
				break;
			}
			p = p->next;

		} while (p != NULL);
		if (flag)
		{
			printf("\t\t\t");
			printf("没有找到需要修改的学生!\n");
		}
		else //查找到该学生
		{
			printf("请输入学生的新学号\t\t\t");
			scanf("%ld", &p->num);
			printf("请输入学生的新姓名\t\t\t");
			scanf("%s", p->name);
			for (int j = 0; j < *m; j++)
			{
				printf("请输入学生：%s  的科目[%d]的新成绩\t\t\t", p->name, j);
				scanf("%f", &p->score[j]);
			}
		}
	}
	return head;
	printf("\t\t\t******************************************************************************\n");
}