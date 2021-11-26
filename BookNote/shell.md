# shell基本功能

### set -e作用

你写的每个脚本都应该在文件开头加上set -e,这句语句告诉bash如果任何语句的执行结果不是true则应该退出。

### set -o pipefail

设置了这个选项以后，包含管道命令的语句的返回值，会变成最后一个返回非零的管道命令的返回值。
ls ./a.txt |echo "hi" >/dev/null
echo $?

运行test.sh，因为当前目录并不存在a.txt文件，输出：
ls: ./a.txt: No such file or directory
1  # 设置了set -o pipefail，返回从右往左第一个非零返回值，即ls的返回值1

注释掉set -o pipefail 这一行，再次运行，输出：
ls: ./a.txt: No such file or directory
0  # 没有set -o pipefail，默认返回最后一个管道命令的返回值

## shell文件比较符

```shell
-e 判断对象是否存在
-d 判断对象是否存在，并且为目录
-f 判断对象是否存在，并且为常规文件
-L 判断对象是否存在，并且为符号链接
-h 判断对象是否存在，并且为软链接
-s 判断对象是否存在，并且长度不为0
-r 判断对象是否存在，并且可读
-w 判断对象是否存在，并且可写
-x 判断对象是否存在，并且可执行
-O 判断对象是否存在，并且属于当前用户
-G 判断对象是否存在，并且属于当前用户组
-nt 判断file1是否比file2新  [ "/data/file1" -nt "/data/file2" ]
-ot 判断file1是否比file2旧  [ "/data/file1" -ot "/data/file2" ]
-n 可以用来判断该参数是否已被赋值

#=====================================================================

[ -a FILE ]  如果 FILE 存在则为真。  
[ -b FILE ]  如果 FILE 存在且是一个块特殊文件则为真。  
[ -c FILE ]  如果 FILE 存在且是一个字特殊文件则为真。  
[ -d FILE ]  如果 FILE 存在且是一个目录则为真。  
[ -e FILE ]  如果 FILE 存在则为真。  
[ -f FILE ]  如果 FILE 存在且是一个普通文件则为真。  
[ -g FILE ] 如果 FILE 存在且已经设置了SGID则为真。 [ -h FILE ]  如果 FILE 存在且是一个符号连接则为真。  
[ -k FILE ]  如果 FILE 存在且已经设置了粘制位则为真。  
[ -p FILE ]  如果 FILE 存在且是一个名字管道(F如果O)则为真。  
[ -r FILE ]  如果 FILE 存在且是可读的则为真。  
[ -s FILE ]  如果 FILE 存在且大小不为0则为真。  
[ -t FD ]  如果文件描述符 FD 打开且指向一个终端则为真。  
[ -u FILE ]  如果 FILE 存在且设置了SUID (set user ID)则为真。  
[ -w FILE ]  如果 FILE 如果 FILE 存在且是可写的则为真。  
[ -x FILE ]  如果 FILE 存在且是可执行的则为真。  
[ -O FILE ]  如果 FILE 存在且属有效用户ID则为真。  
[ -G FILE ]  如果 FILE 存在且属有效用户组则为真。  
[ -L FILE ]  如果 FILE 存在且是一个符号连接则为真。  
[ -N FILE ]  如果 FILE 存在 and has been mod如果ied since it was last read则为真。  
[ -S FILE ]  如果 FILE 存在且是一个套接字则为真。  
[ FILE1 -nt FILE2 ]  如果 FILE1 has been changed more recently than FILE2, or 如果 FILE1 exists and FILE2 does not则为真。  
[ FILE1 -ot FILE2 ]  如果 FILE1 比 FILE2 要老, 或者 FILE2 存在且 FILE1 不存在则为真。  
[ FILE1 -ef FILE2 ]  如果 FILE1 和 FILE2 指向相同的设备和节点号则为真。  
[ -o OPTIONNAME ]  如果 shell选项 “OPTIONNAME” 开启则为真。  
[ -z STRING ]  “STRING” 的长度为零则为真。  
[ -n STRING ] or [ STRING ]  “STRING” 的长度为非零 non-zero则为真。  
[ STRING1 == STRING2 ]  如果2个字符串相同。 “=” may be used instead of “==” for strict POSIX compliance则为真。  
[ STRING1 != STRING2 ]  如果字符串不相等则为真。 
[ STRING1 < STRING2 ]  如果 “STRING1” sorts before “STRING2” lexicographically in the current locale则为真。  
[ STRING1 > STRING2 ]  如果 “STRING1” sorts after “STRING2” lexicographically in the current locale则为真。  
[ ARG1 OP ARG2 ] “OP” is one of -eq, -ne, -lt, -le, -gt or -ge. These arithmetic binary operators return true if “ARG1” is equal to, not equal to, less than, less than or equal to, greater than, or greater than or equal to “ARG2”, respectively. “ARG1” and “ARG2” are integers.



#=====================================================================


1、字符串判断
str1 = str2　　　　　　当两个串有相同内容、长度时为真 
str1 != str2　　　　　 当串str1和str2不等时为真 
-n str1　　　　　　　 当串的长度大于0时为真(串非空) 
-z str1　　　　　　　 当串的长度为0时为真(空串) 
str1　　　　　　　　   当串str1为非空时为真
#=====================================================================
2、数字的判断
int1 -eq int2　　　　两数相等为真 
int1 -ne int2　　　　两数不等为真 
int1 -gt int2　　　　int1大于int2为真 
int1 -ge int2　　　　int1大于等于int2为真 
int1 -lt int2　　　　int1小于int2为真 
int1 -le int2　　　　int1小于等于int2为真

#=====================================================================
3、文件的判断
-r file　　　　　用户可读为真 
-w file　　　　　用户可写为真 
-x file　　　　　用户可执行为真 
-f file　　　　　文件为正规文件为真 
-d file　　　　　文件为目录为真 
-c file　　　　　文件为字符特殊文件为真 
-b file　　　　　文件为块特殊文件为真 
-s file　　　　　文件大小非0时为真 
-t file　　　　　当文件描述符(默认为1)指定的设备为终端时为真

#=====================================================================
4、复杂逻辑判断
-a 　 　　　　　 与 
-o　　　　　　　 或 
!　　　　　　　　非


#=====================================================================
$0 这个程式的执行名字
$n 这个程式的第n个参数值，n=1..9
$* 这个程式的所有参数,此选项参数可超过9个。
$# 这个程式的参数个数
$$ 这个程式的PID(脚本运行的当前进程ID号)
$! 执行上一个背景指令的PID(后台运行的最后一个进程的进程ID号)
$? 执行上一个指令的返回值 (显示最后命令的退出状态。0表示没有错误，其他任何值表明有错误)
$- 显示shell使用的当前选项，与set命令功能相同
$@ 跟$*类似，但是可以当作数组用
```

## shell常用判断语句

1. 判断两个字符串是否相等
   
   ```shell
   if [ "哈哈" = "哈哈" ]
   then
       echo "两个字符串相等！"
   fi
   ```

2. 判断2是否大于1
   
   ```shell
   if [ 2 -gt 1 ]
   then
       echo "2大于1"
   fi
   ```

3. 判断test目录是否存在
   
   ```shell
    if [ -d /home/test ]；then
        echo "目录存在"
    fi
   ```
   
   4. 判断abc.txt的文件是否存在
   
   ```shell
    if [ -e /home/test/abc.txt ]；then
        echo "文件存在"
    fi
   ```

4. 判断abc.txt是否具有可读的权限。
   
   ```shell
   if [ -r /home/test/abc.txt ]；then
       echo "文件abc.txt有可读权限"
   fi
   ```

5. 判断test目录是否有可读的权限
   
   ```shell
   if [ -r /home/test ]；then
       echo "test目录有可读权限"
   fi
   ```

## shell常见面试题

一些经典的 Shell 脚本面试问题

1. 如何在脚本中使用参数 ?
    第一个参数 : $1，第二个参数 :$2
   
    例子 : 脚本会复制文件(arg1) 到目标地址(arg2)
     ./copy.sh file1.txt /tmp/
     cat copy.sh
     \#!/bin/bash
     cp $1 $2

2. 如何计算传递进来的参数 ?
   
    $#

3. 如何检查之前的命令是否运行成功?
   
    $?

4. 如何获取文件的最后一行 ?
   
    tail -1

5. 如何获取文件的第一行 ?
    head -1

6. 如何获取一个文件每一行的第三个元素 ?
    awk'{print $3}'

7. 假如文件中每行第一个元素是FIND，如何获取第二个元素
    awk'{ if ($1 == "FIND") print$2}'

8. 如何调试 bash 脚本
    将 -xv 参数加到#!/bin/bash 后

9. #! /bin/bash在Linux有什么作用？
   
    于可执行的文本文件，第一行的 #！指明了用于执行该文本的解释器
    #! /bin/bash说明这是一个bash脚本
    如果不使用#!，系统默认使用当前的shell进行解释执行。

10. 如何获取变量长度 ?
    
    ${#variable}

11. 如何打印变量的最后 5 个字符 ?
    
     echo ${variable: -5}

12. [ $a == $b ] 和[ $a -eq $b ] 有什么区别
    
    [ $a == $b ]     用于字符串比较
    [ $a -eq $b ] -  用于数字比较

13. bash 中 $! 表示什么意思 ?
    
     后台最近执行命令的 PID.

14. $? 表示什么意思 ?
    
    前台最近命令的结束状态。

15. 如何输出当前 shell 的 PID ?
    
     echo $$

16. $* 和 $@ 有什么区别？
    
     $* - 以一个字符串形式输出所有传递到脚本的参数
     $@ - 以 $IFS 为分隔符列出所有传递到脚本中的参数

17. 如何打印数组的第一个元素 ?
    
     echo ${array[0]}

18. 如何打印数组的所有元素 ?
    
     echo ${array[@]}

19. 如何输出所有数组索引 ?
    
     echo ${!array[@]}

20. 如何在 bash 中定义数组 ?
    
     array=("Hi" "my""name" "is")

21. 
