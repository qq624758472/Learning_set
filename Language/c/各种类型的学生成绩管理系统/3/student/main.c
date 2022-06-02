/*
 * @Description:图书管理系统， 通过网络进行命令发送控制服务器显示
 * @Version: 2.0
 * @Autor: ruog__
 * @Date: 2022-04-06 20:49:37
 * @LastEditors: ruog__
 * @LastEditTime: 2022-04-14 16:06:46
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include "server.h"
#include <sys/mman.h>
#include <signal.h>
#include <regex.h>

cmd ctl;
struct Student
{                  //每个图书对应一个结构体
    char ID[20];   //书的标记号
    char Name[10]; //书名
    float Mark1;   //图书总页数       图书总页数
    float Mark2;   //图书章节数       图书章节数
    float Mark3;   //图书价格       图书价格
    float Mark4;   // ISBN号     ISBN号
    float All;     //总分
    float Average; //平均成绩
} students[1000];
int num = 0; //计数器
void Copy(struct Student *arr, int i, int j)
{
    strcpy(arr[i].ID, arr[j].ID);
    strcpy(arr[i].Name, arr[j].Name);
    arr[i].Mark1 = arr[j].Mark1;
    arr[i].Mark2 = arr[j].Mark2;
    arr[i].Mark3 = arr[j].Mark3;
    arr[i].Mark4 = arr[j].Mark4;
    arr[i].All = arr[j].All;
    arr[i].Average = arr[j].Average;
}

int my_match(char *pattern, char *buf)
{
    int status, i;
    int flag = REG_EXTENDED;
    regmatch_t pmatch[1];
    const size_t nmatch = 1;
    regex_t reg;
    //编译正则模式
    regcomp(&reg, pattern, flag);
    //执行正则表达式和缓存的比较
    status = regexec(&reg, buf, nmatch, pmatch, 0);
    //打印匹配的字符串
    for (i = pmatch[0].rm_so; i < pmatch[0].rm_eo; ++i)
    {
        putchar(buf[i]);
    }
    printf("\n");
    regfree(&reg);
    return status;
}

int Student_SearchByName(char name[]) //通过书名来检索书
{
    int i;
    for (i = 0; i < num; i++)
    {
        int status = -1;
        status = my_match(name, students[i].Name);
        // if (strcmp(students[i].Name, name) == 0) //通过strcmp函数来对比书名，找到返回在数组的位置
        if (status == 0) //正则匹配成功
        {
            printf("正则匹配书名成功\n");
            return i;
        }
    }
    return -1; //未找到返回 -1
}
int Student_SearchByIndex(char id[]) //通过书号来检索图书信息
{
    int i;
    for (i = 0; i < num; i++)
    {
        if (strcmp(students[i].ID, id) == 0) //通过strcmp函数来对比图书id，找到返回位置
        {
            return i;
        }
    }
    return -1; //未找到返回 -1
}
void Student_DisplaySingle(int index) //输出表头
{
    printf("%12s %12s %12s %12s %12s %12s\n", "书号", "书名", "图书总页数", "图书章节数", "图书价格", "ISBN号");
    printf("-------------------------------------------------------------\n");
    printf("%10s%10s %8.2f %8.2f %8.2f %8.2f\n", students[index].ID, students[index].Name,
           students[index].Mark1, students[index].Mark2, students[index].Mark3, students[index].Mark4);
}
void inputt(cmd *shareMemory) //利用循环录入图书信息
{
    while (1)
    {
        printf("请输入书号:");
        fflush(stdout);
        myscanf(shareMemory, &students[num].ID, 0);
        // scanf("%s", &students[num].ID);
        // getchar();
        printf("请输入书名:");
        fflush(stdout);
        myscanf(shareMemory, &students[num].Name, 0);
        // scanf("%s", &students[num].Name);
        // getchar();
        printf("请输入图书总页数:");
        fflush(stdout);
        myscanf(shareMemory, &students[num].Mark1, 1);
        // scanf("%f", &students[num].Mark1);
        // getchar();
        printf("请输入图书章节数:");
        fflush(stdout);
        myscanf(shareMemory, &students[num].Mark2, 1);
        // scanf("%f", &students[num].Mark2);
        // getchar();
        printf("请输入图书价格:");
        fflush(stdout);
        myscanf(shareMemory, &students[num].Mark3, 1);
        // scanf("%f", &students[num].Mark3);
        // getchar();
        printf("请输入ISBN号:");
        fflush(stdout);
        myscanf(shareMemory, &students[num].Mark4, 1);
        // scanf("%f", &students[num].Mark4); //依次输入各项数据
        // getchar();
        // students[num].All = students[num].Mark1 + students[num].Mark2 + students[num].Mark3 + students[num].Mark4; //输完数据后自动计算总成绩与平均成绩
        // students[num].Average = (students[num].Mark1 + students[num].Mark2 + students[num].Mark3 + students[num].Mark4) / 4;
        if (Student_SearchByIndex(students[num].ID) == -1)
        {
            num++; //移向下一个位置
        }
        else
        {
            printf("书号重复，输入数据无效 !!!\n");
            fflush(stdout);
        }

        printf("是否继续?(y/n)");
        fflush(stdout);
        char eee = 0;
        myscanf(shareMemory, &eee, 0);
        if (eee == 'n')
        // if (getchar() == 'n')
        {
            break;
        }
    }
}
void modify(cmd *shareMemory) //修改成绩
{
    while (1)
    {
        char id[20];
        int index;
        printf("请输入要修改的图书的书号:");
        fflush(stdout);
        // scanf("%s", &id);
        // getchar();
        memset(id, 0, 20);
        myscanf(shareMemory, id, 0);
        index = Student_SearchByIndex(id); //调用搜查id函数，根据其返回值确定位置
        if (index == -1)
        {
            printf("图书不存在!\n");
            fflush(stdout);
        }
        else
        {
            printf("你要修改的图书信息为:\n");
            fflush(stdout);
            Student_DisplaySingle(index);
            printf("-- 请输入新值--\n");
            printf("请输入书号:");
            fflush(stdout);
            myscanf(shareMemory, &students[index].ID, 0);
            // scanf("%s", &students[index].ID);
            // getchar();
            printf("请输入书名:");
            fflush(stdout);
            memset(&students[index].Name, 0, 10);
            myscanf(shareMemory, &students[index].Name, 0);
            // scanf("%s", &students[index].Name);
            // getchar();
            printf("请输入图书总页数:");
            fflush(stdout);
            students[index].Mark1 = 0.0;
            myscanf(shareMemory, &students[index].Mark1, 1);
            // scanf("%f", &students[index].Mark1);
            // getchar();
            printf("请输入图书章节数:");
            fflush(stdout);
            students[index].Mark2 = 0.0;
            myscanf(shareMemory, &students[index].Mark2, 1);
            // scanf("%f", &students[index].Mark2);
            // getchar();
            printf("请输入图书价格:");
            fflush(stdout);
            students[index].Mark3 = 0.0;
            myscanf(shareMemory, &students[index].Mark3, 1);
            // scanf("%f", &students[index].Mark3);
            // getchar();
            printf("请输入ISBN号:");
            fflush(stdout);
            students[index].Mark4 = 0.0;
            myscanf(shareMemory, &students[index].Mark4, 1);
            // scanf("%f", &students[index].Mark4); //重新录入一套新的数据替代
            // getchar();
            // students[index].All = students[index].Mark1 + students[index].Mark2 + students[index].Mark3 + students[index].Mark4;
            // students[index].Average = (students[index].Mark1 + students[index].Mark2 + students[index].Mark3 + students[index].Mark4) / 4;
        }
        printf("是否继续?(y/n)");
        fflush(stdout);
        char eee = 0;
        myscanf(shareMemory, &eee, 0);
        if (eee == 'n')
        // if (getchar() == 'n')
        {
            break;
        }
    }
}
void deletee(cmd *shareMemory) //删除图书信息
{
    int i;
    while (1)
    {
        char id[20];
        int index;
        printf("请输入要删除的图书的书号:");
        fflush(stdout);
        // scanf("%s", &id);
        // getchar();
        memset(id, 0, 20);
        myscanf(shareMemory, id, 0);
        index = Student_SearchByIndex(id); //调用搜查id函数，根据其返回值确定位置
        if (index == -1)
        {
            printf("图书不存在!\n");
            fflush(stdout);
        }
        else
        {
            printf("你要删除的图书信息为:\n");
            Student_DisplaySingle(index);
            printf("是否真的要删除?(y/n)");
            fflush(stdout);
            char eee = 0;
            myscanf(shareMemory, &eee, 0);
            if (eee == 'y')
            // if (getchar() == 'y')
            {
                for (i = index; i < num - 1; i++)
                {
                    Copy(students, i, i + 1);
                    // students[i]=students[i+1];    //把后边的对象都向前移动
                }
                num--;
            }
            // getchar();
        }
        printf("是否继续?(y/n)");
        fflush(stdout);
        char eee = 0;
        myscanf(shareMemory, &eee, 0);
        if (eee == 'n')
        // if (getchar() == 'n')
        {
            break;
        }
    }
}
void display(cmd *shareMemory) //打印已录入的图书信息
{
    int a;
    printf("%12s %12s %12s %12s %12s %12s\n", "书号", "书名", "图书总页数", "图书章节数", "图书价格", "ISBN号");
    printf("-------------------------------------------------------------\n");
    for (a = 0; a < num; a++)
    {
        printf("%10s%10s %8.2f %8.2f %8.2f %8.2f\n", students[a].ID, students[a].Name,
               students[a].Mark1, students[a].Mark2, students[a].Mark3, students[a].Mark4);
    }
    fflush(stdout);
}
void insert(cmd *shareMemory) //指定位置插入图书信息
{
    int a = 0, b = 0, c = 0;
    printf("请输入你要插入的位置");
    fflush(stdout);
    myscanf(shareMemory, &a, 0);
    // scanf("%d", &a);
    if (a > num)
    {
        printf("输入的位置有误，请重新输入,当前共%d条数据\n", num);
        fflush(stdout);
        myscanf(shareMemory, &a, 0);
        // scanf("%d", &a);
    }

    b = num - 1;
    for (; b >= a - 1; b--)
    {
        // strcpy(students[b+1].ID,students[b].ID);
        // strcpy(students[b+1].Name,students[b].Name);
        // students[b+1].Mark1=students[b].Mark1;
        // students[b+1].Mark2=students[b].Mark2;
        // students[b+1].Mark3=students[b].Mark3;
        // students[b+1].Mark4=students[b].Mark4;
        // students[b+1].All=students[b].All;
        // students[b+1].Average=students[b].Average;
        Copy(students, b + 1, b); //根据其输入的位置，将其及以后的数据向后移动一个位置
    }
    num++;
    printf("请输入书号:");
    fflush(stdout);
    myscanf(shareMemory, &students[a - 1].ID, 0);
    // scanf("%s", &students[a - 1].ID);
    // getchar();
    printf("请输入书名:");
    fflush(stdout);
    myscanf(shareMemory, &students[a - 1].Name, 0);
    // scanf("%s", &students[a - 1].Name);
    // getchar();
    printf("请输入图书总页数:");
    fflush(stdout);
    myscanf(shareMemory, &students[a - 1].Mark1, 1);
    // scanf("%f", &students[a - 1].Mark1);
    // getchar();
    printf("请输入图书章节数:");
    fflush(stdout);
    myscanf(shareMemory, &students[a - 1].Mark2, 1);
    // scanf("%f", &students[a - 1].Mark2);
    // getchar();
    printf("请输入图书价格:");
    fflush(stdout);
    myscanf(shareMemory, &students[a - 1].Mark3, 1);
    // scanf("%f", &students[a - 1].Mark3);
    // getchar();
    printf("请输入ISBN号:");
    fflush(stdout);
    myscanf(shareMemory, &students[a - 1].Mark4, 1);
    // scanf("%f", &students[a - 1].Mark4); //输入新数据
    // getchar();
    // students[a - 1].All = students[a - 1].Mark1 + students[a - 1].Mark2 + students[a - 1].Mark3 + students[a - 1].Mark4;
    // students[a - 1].Average = (students[a - 1].Mark1 + students[a - 1].Mark2 + students[a - 1].Mark3 + students[a - 1].Mark4) / 4;
}
void search(cmd *shareMemory) //查询图书信息
{
    while (1)
    {
        char name[20];
        int index;
        printf("请输入要查询的图书的书名:");
        fflush(stdout);
        memset(name, 0, 20);
        myscanf(shareMemory, &name, 0);
        // scanf("%s", &name);
        // getchar();
        index = Student_SearchByName(name); //调用搜查name函数，根据其返回值确定位置
        if (index == -1)
        {
            printf("图书不存在!\n");
            fflush(stdout);
        }
        else
        {
            printf("你要查询的图书信息为:\n");
            Student_DisplaySingle(index);
        }
        printf("是否继续?(y/n)");
        fflush(stdout);
        char eee = 0;
        myscanf(shareMemory, &eee, 0);
        if (eee == 'n')
        // if (getchar() == 'n')
        {
            break;
        }
    }
}

void tar_all(cmd *shareMemory) //系统目录打包
{
    //while (1)
    {
        char name = 0; //系统打包命令
        int index;
        printf("需要进行哪些目录操作？\n 1.打包\n2.解压\n3.目录复制\n4.任意命令");
        fflush(stdout);
        myscanf(shareMemory, &name, 0);
        if (name == '1')
        {
            system("tar czvf system.tar.gz ./");
            printf("打包完成,本地目录下system.tar.gz文件\n");
            fflush(stdout);
        }
        else if(name == '2')
        {
            if(access("./system.tar.gz",F_OK) != 0)
            {
                printf("未发现打包好的程序\n");
                fflush(stdout);
                return;
            }
            system("mkdir tar;cd tar;mv ../system.tar.gz ./;tar xf system.tar.gz");
            printf("解压包完成,在本地tar目录下\n");
            fflush(stdout);
        }
        else if(name == '3')
        {
            printf("请输入目标文件夹的绝对路径!\n");
            fflush(stdout);
            char name[1024] = {0},mingling[1024] = {0};
            memset(name,0,1024);
            memset(mingling,0,1024);
            myscanf(shareMemory, name, 0);
            sprintf(mingling,"cp ../student/ %s -rf",name);
            system(mingling);
        }
        else if(name == '4')
        {
            printf("请输入任意命令\n");
            fflush(stdout);
            char name[1024] = {0};
            memset(name,0,1024);
            myscanf(shareMemory, name, 0);
            system(name);
        }
        else
        {
            printf("选项无效\n");
        }
    }
}

void sort(cmd *shareMemory) //根据平均分排序
{
    int i, j;
    // struct students tmp;
    for (i = 0; i < num; i++)
    {
        students[i].Average = (students[i].Mark1 + students[i].Mark2 + students[i].Mark3 + students[i].Mark4) / 4;
    }
    for (i = 0; i < num; i++)
    {
        for (j = 1; j < num - i; j++)
        {
            if (students[j - 1].Average < students[j].Average)
            {

                Copy(students, num, j - 1);
                Copy(students, j - 1, j);
                Copy(students, j, num);
                // tmp=students[j-1];
                // students[j-1]=students[j];
                // students[j]=tmp;      //冒泡排序
            }
        }
    }
    int a;
    printf("%12s %12s %12s %12s %12s %12s\n", "书号", "书名", "图书总页数", "图书章节数", "图书价格", "ISBN号");
    printf("-------------------------------------------------------------\n");
    for (a = 0; a < num; a++)
    {
        printf("%10s%10s %8.2f %8.2f %8.2f %8.2f\n", students[a].ID, students[a].Name,
               students[a].Mark1, students[a].Mark2, students[a].Mark3, students[a].Mark4);
    }
    fflush(stdout);
}
/*
void SearchLow(cmd *shareMemory) //搜索不及格的并输出
{
    int a;
    printf("           图书总页数不及格的有%10s%10s%8s\n", "书号", "书名", "图书总页数");
    for (a = 0; a < num; a++)
    {
        if (students[a].Mark1 < 60)
            printf("%10s%10s%8.2f\n", students[a].Name, students[a].ID, students[a].Mark1); //从头搜索到尾，若小于60就输出
    }

    printf("           图书章节数不及格的有%10s%10s%8s\n", "书号", "书名", "图书章节数");
    for (a = 0; a < num; a++)
    {
        if (students[a].Mark2 < 60)
            printf("%10s%10s%8.2f\n", students[a].Name, students[a].ID, students[a].Mark2);
    }

    printf("           图书价格不及格的有%10s%10s%8s\n", "书号", "书名", "图书价格");
    for (a = 0; a < num; a++)
    {
        if (students[a].Mark3 < 60)
            printf("%10s%10s%8.2f\n", students[a].Name, students[a].ID, students[a].Mark3);
    }

    printf("           ISBN号不及格的有%10s%10s%8s\n", "书号", "书名", "ISBN号");
    for (a = 0; a < num; a++)
    {
        if (students[a].Mark4 < 60)
            printf("%10s%10s%8.2f\n", students[a].Name, students[a].ID, students[a].Mark4);
    }
    //system("pause");
    fflush(stdout);
}

void SearchHigh(cmd *shareMemory) //搜索成绩最高者输出
{
    int a;
    int max;
    printf("           图书总页数最高分为%10s%10s%8s\n", "书号", "书名", "图书总页数");
    max = students[0].Mark1;
    for (a = 1; a < num; a++)
    {
        if (students[a].Mark1 > max)
            max = students[a].Mark1;
    }
    for (a = 0; a < num; a++)
    {
        if (max == students[a].Mark1)
            printf("%10s%10s%8.2f\n", students[a].Name, students[a].ID, students[a].Mark1);
    }

    printf("           图书章节数最高分为%10s%10s%8s\n", "书号", "书名", "图书章节数");
    max = students[0].Mark2;
    for (a = 1; a < num; a++)
    {
        if (students[a].Mark2 > max)
            max = students[a].Mark2;
    }
    for (a = 0; a < num; a++)
    {
        if (max == students[a].Mark2)
            printf("%10s%10s%8.2f\n", students[a].Name, students[a].ID, students[a].Mark2);
    }

    printf("           图书价格最高分为%10s%10s%8s\n", "书号", "书名", "图书价格");
    max = students[0].Mark3;
    for (a = 1; a < num; a++)
    {
        if (students[a].Mark3 > max)
            max = students[a].Mark3;
    }
    for (a = 0; a < num; a++)
    {
        if (max == students[a].Mark3)
            printf("           %10s%10s%8.2f\n", students[a].Name, students[a].ID, students[a].Mark3);
    }

    printf("           ISBN号最高分为%10s%10s%8s\n", "书号", "书名", "ISBN号");
    max = students[0].Mark4;
    for (a = 1; a < num; a++)
    {
        if (students[a].Mark4 > max)
            max = students[a].Mark4;
    }
    for (a = 0; a < num; a++)
    {
        if (max == students[a].Mark4)
            printf("           %10s%10s%8.2f\n", students[a].Name, students[a].ID, students[a].Mark4);
    }
    //system("pause");
    fflush(stdout);
}
*/
void Save()
{
    FILE *fp = fopen("temp.txt", "w+");
    fprintf(fp, "%d\n", num);
    for (int i = 0; i < num; i++)
    {
        fprintf(fp, "%s %s %f %f %f %f\n", students[i].ID, students[i].Name, students[i].Mark1, students[i].Mark2, students[i].Mark3, students[i].Mark4);
    }
    fclose(fp);
    fflush(stdout);
}
void Load()
{
    FILE *fp = fopen("temp.txt", "r");
    fscanf(fp, "%d", &num);
    for (int i = 0; i < num; i++)
    {
        fscanf(fp, "%s %s %f %f %f %f\n", students[i].ID, students[i].Name, &students[i].Mark1, &students[i].Mark2, &students[i].Mark3, &students[i].Mark4);
    }
    fclose(fp);
}

/*主程序*/
int main()
{
    // printf("start main\n");
    memset(&ctl, 0, sizeof(cmd));
    //开启服务器线程
    pid_t pid;
    if ((pid = fork()) < 0)
    {
        printf("erro");
        exit(0);
    }
    else if (pid == 0)
    {
        // printf("child process pid:%d\n", getpid());
        start();
    }
    else
    {
        //创建共享内存
        cmd *shareMemory = NULL;
        int shm_id = create_shm(&shareMemory);
        if (shareMemory == NULL)
        {
            printf("create_shm Error\n");
            return 0;
        }
        memset(shareMemory, 0, sizeof(cmd));

        int i;
        while (1)
        {
            Load();
            printf("\t\t\t\t\t-------- 图书信息管理系统-------\n\n\n\n"); //菜单
            printf("\t\t\t\t\t1. 增加图书记录\n\n");
            printf("\t\t\t\t\t2. 修改图书记录\n\n");
            printf("\t\t\t\t\t3. 删除图书记录\n\n");
            printf("\t\t\t\t\t4. 插入图书记录\n\n");
            printf("\t\t\t\t\t5. 显示所有记录\n\n");
            printf("\t\t\t\t\t6. 查询图书记录\n\n");
            printf("\t\t\t\t\t7. 系统目录打包\n\n");
            // printf("\t\t\t\t\t7. 按平均成绩排序\n\n");
            // printf("\t\t\t\t\t8. 输出各科目不及格学生\n\n");
            // printf("\t\t\t\t\t9. 输出各科目最高分\n\n");
            printf("\t\t\t\t\t0. 退出\n\n\n");
            printf("请选择(0-7):");
            fflush(stdout);
            i = myscanf(shareMemory, NULL, 0);
            printf("i = %d\n", i);
            fflush(stdout);
            // scanf("%d", &i);
            switch (i)
            {
            case 1:
                inputt(shareMemory);
                break;
            case 2:
                modify(shareMemory);
                break;
            case 3:
                deletee(shareMemory);
                break;
            case 4:
                insert(shareMemory);
                break;
            case 5:
                display(shareMemory);
                break;
            case 6:
                search(shareMemory);
                break;
            case 7:
                tar_all(shareMemory);
                break;
            /*
            case 7:
                sort(shareMemory);
                break;
            case 8:
                SearchLow(shareMemory);
                break;
            case 9:
                SearchHigh(shareMemory);
                break;
            */
            case 0:
                del_shm(shareMemory, shm_id);
                kill(pid, SIGKILL);
                wait(NULL);
                Save();
                return 0;
            default:
                printf("没有该选项\n");
                break;
            }
            Save();
        }
    }
    return 0;
}