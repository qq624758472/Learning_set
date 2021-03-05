# git命令使用总结
## git使用方法

1. git clone 代码地址
2. 拷贝或者修改代码
3. 进入拷贝的代码文件夹
4. git add *
5. git commit -m '修改日志'
6. git push/ git push my_server master


## git删除文件夹

1. git rm -r --cached target              # 删除target文件夹
2. git commit -m '删除了target'        # 提交,添加操作说明
3. git push

git commit -a #将删除的文件提交。
git push      #远程代码仓库就删除文件了。
##  遇到的一些问题
1. 解决 The file will have its original line endings in your working directory

首先出现这个问题主要原因是：我们从别人github地址上通过git clone下载下来，而又想git push到我们自己的github上，那么就会出现上面提示的错误信息

此时需要执行如下代码：

    git rm -r --cached .
    git config core.autocrlf false
    
    git add .
---------------------

## 如何给github 打tag
1.git查看tag的命令是：git tag
2.创建Tag 是直接加名字就ok了，格式： git tag 名字 –m "注释"
3.推送到远端 git push origin tag名

## git配置默认账号和密码
git config --global user.name "youname"设置密码
git config --global user.email "aa@qq.com"设置邮箱


## 多人协同提交时,或者自己在不同的电脑上开发时
多人协同提交时,或者自己在不同的电脑上开发时,会遇到代码不同步的情况.
需要先暂存修改,暂存修改需要:1.git add 修改的文件 2.git -m commit "log" 后,在同步代码,再恢复暂存, 最后直接提交. 

git pull会把本地未提交修改覆盖。

处理的方式非常简bai单，主要是使用git stash命令进行处理，分成以下几个步骤进行处理。

1、先将本地修改存储起来
$ git stash
这样本地的所有修改就都被暂时存储起来 。是用git stash list可以看到保存的信息：

git stash暂存修改

其中stash@{0}就是刚才保存的标记。

2、pull内容
暂存了本地修改之后，就可以pull了。

$ git pull

3、还原暂存的内容

$ git stash pop stash@{0}

系统提示如下类似的信息：

Auto-merging c/environ.c

CONFLICT (content): Merge conflict in c/environ.c

意思就是系统自动合并修改的内容，但是其中有冲突，需要解决其中的冲突。

4、解决文件中冲突的的部分

打开冲突的文件，会看到类似如下的内容：
git冲突内容

其中Updated upstream 和=====之间的内容就是pull下来的内容，====和stashed changes之间的内容就是本地修改的内容。碰到这种情况，git也不知道哪行内容是需要的，所以要自行确定需要的内容。

解决完成之后，就可以正常的提交了。
