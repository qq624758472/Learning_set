#ifndef TOOLS_H
#define TOOLS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
//#include <getch.h>

// 清理屏幕
void clear_screen(void);
// 任意键继续
void anykey_continue(void);
// 获取性别
char get_sex(void);
// 获取成绩
bool get_score(float* score_p);
// 清理输入缓冲区
void clear_stdin(void);
// 获取指令
char get_cmd(char start,char end);
// 获取密码
char* get_passwd(char* passwd,bool is_show,size_t size);
// 从键盘获取字符串
char* get_str(char* str_p, size_t hope_len);
// 密码加密函数
char* str_to_encrypt(char* str, const char* passwd);
//判断密码是否正确
bool is_password_right(char* temp, char* password);
// 从文件加载数据
int load_data(const char* path, void* ptr, size_t node_size);
// 保存数据到文件
int save_data(const char* path, void* ptr, size_t node_size, size_t num);

#endif//TOOLS_H

