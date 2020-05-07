### git使用方法

1. git clone 代码地址
2. 拷贝或者修改代码
3. 进入拷贝的代码文件夹
4. git add *
5. git commit -m '修改日志'
6. git push/ git push my_server master


### git删除文件夹

1. git rm -r --cached target              # 删除target文件夹
2. git commit -m '删除了target'        # 提交,添加操作说明
3. git push

####  遇到的一些问题
1. 解决 The file will have its original line endings in your working directory

首先出现这个问题主要原因是：我们从别人github地址上通过git clone下载下来，而又想git push到我们自己的github上，那么就会出现上面提示的错误信息

此时需要执行如下代码：

    git rm -r --cached .
    git config core.autocrlf false
    
    git add .
---------------------
