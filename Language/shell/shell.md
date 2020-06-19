# shell基本功能

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

3. 判断abc.txt是否具有可读的权限。

   ```shell
   if [ -r /home/test/abc.txt ]；then
   	echo "文件abc.txt有可读权限"
   fi
   ```

4. 判断test目录是否有可读的权限

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

10.  如何获取变量长度 ?

    ${#variable}

11. 如何打印变量的最后 5 个字符 ?

     echo ${variable: -5}

12.  [ $a == $b ] 和[ $a -eq $b ] 有什么区别

     [ $a == $b ]     用于字符串比较
     [ $a -eq $b ] -  用于数字比较

13. bash 中 $! 表示什么意思 ?

     后台最近执行命令的 PID.

14.  $? 表示什么意思 ?

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



