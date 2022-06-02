#include "sams.h"
#include "tools.h"
#include "teacher.h"
#include "student.h"
#include "admin.h"

int main()
{
	// 系统初始化
	sams_init();
    // 系统开始运行
	sams_start();
    // 系统结束
	sams_exit();
}
