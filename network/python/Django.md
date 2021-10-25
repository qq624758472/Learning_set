# Django学习笔记

## 1.Python虚拟环境
1. sudo apt-get install python-virtualenv或者 sudo pip install virtualenv
2. pip install virtualenvwrapper 
3. 安装完成之后，需要在用户根目录下（即/home/[username]）的.bashrc末尾加入：
       source /usr/local/bin/virtualenvwrapper.sh  然后执行source ~/.bashrc 
4. 创建并进入环境：mkvirtualenv env1
   退出环境：deactivate
   进入已存在的环境或者切换环境：workon env1或者env2
   删除环境： rmvirtualenv env1
## 2.python安装requirements.txt命令：

```shell
pip install -r requirements.txt
```

## 3. 一般命令

### 1.新建⼀个工程

```shell
 django-projectdjango-admin.py startproject project-name
```

### 2.新建一个APP

```python
python manage.py startapp app-name
```

### 3.同步数据库

```python
python manage.py syncdb
```

### 4.运行服务器

```python
python manage.py runserver 0.0.0.0:8000
```

### 5.清空数据库

```python
python manage.py flush
```

### 6.创建超级管理员

```
python manage.py createsuperuser
```

### 7.导出数据 导⼊数据

```
python manage.py dumpdata appname > appname.json
python manage.py loaddata appname.json
```

