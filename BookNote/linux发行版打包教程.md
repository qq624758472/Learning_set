# centos下rpm包相关

参考博客：

https://www.cnblogs.com/michael-xiang/p/10480809.html

https://www.cnblogs.com/SQL888/p/5776442.html

## 一、rpm包制作

### 1.安装打包工具

```sh
yum install -y rpm-build
yum install redhat-rpm-config
```

### 2.打包文件的配置

安装完成后会在root目录下生成BUILD BUILDROOT RPMS SOURCES SPECS SRPMS五个文件夹，都是空的。

如果没有就手动创建

然后将程序源码包放在SOURCES文件夹中，打成tar包。

**下面总结一下运行rpm -ba filename.spec时,RPM都做些什么**

1)读取并解析 filename.spe文件

2)运行%prep部分来将源代码解包到一个临时目录,并应用所有的补丁程序

3)运行%build部分来编译代码

4)运行% install部分将代码安装到构建机器的目录中。读取9%fles部分的文件列表,收集文件并创建二进制和源RPM文件。源RPM文件简单地捆绑了spec文件和构建软件包用到的所有源文件和补丁文件。如果您选择分发它,则其他人可以很容易地用它重建您的软件。二进制RPM文件仅包含已绵译的软件和如何安装的信息

5)运行% clean部分来除去临时构建目录。



**文件头**

这个区域定义的 `Name`、`Version` 这些字段对应的值可以在后面通过 `%{name}`,`%{version}` 这样的方式来引用，类似于 C 语言中的宏

- `Summary`：用一句话概括该软件s包尽量多的信息。

- `Name`：软件包的名字，最终 rpm 软件包是用该名字与版本号（`Version`）、释出号(`Release`）及体系号来命名软件包的，后面可使用 `%{name}` 的方式引用

- `Version`：软件版本号。仅当软件包比以前有较大改变时才增加版本号，后面可使用`%{version}`引用

- `Release`：软件包释出号/发行号。一般我们对该软件包做了一些小的补丁的时候就应该把释出号加 1，后面可使用 `%{release}` 引用

- `Packager`：打包的人（一般喜欢写个人邮箱）

- `Vendor`：软件开发者的名字，发行商或打包组织的信息，例如`RedFlagCo,Ltd`

- `License`：软件授权方式，通常是GPL（自由软件）或GPLv2,BSD

- `Copyright`：软件包所采用的版权规则。具体有：`GPL（自由软件）`，`BSD`，`MIT`，`Public Domain（公共域）`，`Distributable（贡献）`，`commercial（商业）`，`Share（共享）`等，一般的开发都写 `GPL`。

- `Group`：软件包所属类别

  Development/System （开发/系统）

  System Environment/Daemons （系统环境/守护）

- `Source`：源程序软件包的名字/源代码包的名字，如 `stardict-2.0.tar.gz`。可以带多个用 `Source1`、`Source2` 等源，后面也可以用 `%{source1}`、`%{source2}` 引用

  ```cmake
  Source0: %{name}-boost-%{version}.tar.gz    ← 源码包名称(可以使用URL)，可以用SourceN指定多个，如配置文件
  #Patch0: some-bugs0.patch                    ← 如果需要打补丁，则依次填写
  #Patch1: some-bugs1.patch                    ← 如果需要打补丁，则依次填写
  ```

- `BuildRequires`: 制作过程中用到的软件包，构建依赖

- `Requires`: 安装时所需软件包和版本号．

  - `Requires(pre)`: 指定不同阶段的依赖

- `BuildRoot`: 这个是安装或编译时使用的「虚拟目录」，打包时会用到该目录下文件，可查看安装后文件路径，例如：`BuildRoot: %_topdir/BUILDROOT`。**这个必须加，否则make install 找不到安装地址．**

- `Prefix: %{_prefix}` 这个主要是为了解决今后安装 rpm 包时，并不一定把软件安装到 rpm 中打包的目录的情况。这样，必须在这里定义该标识，并在编写 `%install` 脚本的时候引用，才能实现 rpm 安装时重新指定位置的功能

- `BuildArch`: 指编译的目标处理器架构，`noarch` 标识不指定，但通常都是以 `/usr/lib/rpm/marcros` 中的内容为默认值

- `%description`：软件包详细说明，可写在多个行上。这样任何人使用 `rpm -qi`查询您的软件包时都可以看到它。您可以解释这个软件包做什么，描述任何警告或附加的配置指令，等等。

- `URL`：软件的主页



**文件头必选域：**

1.Name此域定义软件名。

2.Versio此域定义版本号。仅当软件较以前有较大改变时才增加版本号。注:版本号中不能含减号(-)字符

3.Release此域定义释出号。若软件较以前改变较小,则仅增加释出号,不改变版本号。注:释岀号中亦不能含减号(-)字符PM利用上述的Name(软件名) Version(版本号), Release(释出号)及体系号来命名软件包.

4.Summary此域定义软件包简介。

5.Group此域定义软件所属类别,本例的 Developmet/Tools表示本软件属”开发/工具"类

6.License此域定义软件适用的许可证或版权规则。该域也可用 Copyright(版权)来定义,二者同意。许可证具体有:GPL(通用公共许可证,自由软件适用, BSD. MIT, Public Domain(公共域, Distributable(贡献), Commercial(商业), Share(共享)等。

7.BuildRoot:这个是安装或编译时使用的「虚拟目录」，打包时会用到该目录下文件，可查看安装后文件路径，例如：`BuildRoot: %_topdir/BUILDROOT`。**这个必须加，否则make install 找不到安装地址．** 一般为：`%{_tmppath}/%{name}-%{version}-%{release}-root`

然后进入SPEC文件夹中，编写.spec文件，我写如下所示：

```SPARQL
Name:test			#程序名字
Version:0.0.1		#程序版本号
Release:        1%{?dist}	#发行版
Summary:        123			#概要
Requires:gcc<=1.1.1			#在rpm包安装时，gcc版本不符合要求就会安装失败．
Source0:        %{name}-%{version}.tar.gz	#放在SOURCES文件夹中的程序包名称
License: GPL	#必须得加，依据的什么规则
BuildRoot:%{_tmppath}/%{name}-%{version}-%{release}-root

%description
test


%prep			    #这个阶段是「预处理」，通常用来执行一些解开源程序包的命令，为下一步的编译安装作准备。
%setup -q          #宏的作用是解压并切换到目录
#%patch0 -p1       #如果需要打补丁，则依次写
#%patch1 -p1       #如果需要打补丁，则依次写

%build						#解包完成后编译的方式，可以用.configure先生成makefile后make ,也可以直接make, 也可以cmake ,　　我这个是自己写的Makefile，然后直接make就可以了。
make %{?_smp_mflags}


%install				#安装时执行的make命令。
make install DESTDIR=%{buildroot}	#安装的路径，buildroot为上边设置的路径．对应的程序中的Makefile也得加入DESTDIR字段．


%files
%doc
/usr/bin/main_test		#需要打包的文件


%changelog
```

Makefile实例：

```makefile
CC=gcc
prom=main_test
deps=
obj=test.o

$(prom):$(obj)
                $(CC) -o $(prom) $(obj)

%.o:%.c $(deps)
        $(CC) -c $< -o $@

clean:
        rm -rf $(obj) $(prom)

#必须加入DESTDIR字段．在spec文件中才能安装到临时虚拟root目录下．
install:
        test -d '$(DESTDIR)/usr/bin/' || mkdir -p '$(DESTDIR)/usr/bin/'
        cp ./main_test $(DESTDIR)/usr/bin/
```



### 3.spec文件编写完成后，就可以运行编译打包命令：

```sh
rpmbuild -ba /root/rpmbuild/SPECS/test.spec --nodebuginfo
```

--nodebuginfo:禁止使用调试信息，程序和这个应该对齐使用。　　不加这个就得在使用gcc/g++编译的时候加-g参数

### 4.生成rpm包。

生成的rpm包在RPMS文件夹中。

### 5.安装rpm包。

## 二、rpm打包实例

### 1.打包自己写的test程序

* 在SOURCES文件夹下创建test-0.0.1文件夹

* 在test-0.0.1文件夹下创建test.c,和Makefile文件,内容如下．

* ```makefile
  CC=gcc
  prom=main_test
  deps=
  obj=test.o
  
  $(prom):$(obj)
                  $(CC) -o $(prom) $(obj)
  
  %.o:%.c $(deps)
          $(CC) -c $< -o $@
  
  clean:
          rm -rf $(obj) $(prom)
  
  #必须加入DESTDIR字段．在spec文件中才能安装到临时虚拟root目录下．
  install:
          test -d '$(DESTDIR)/usr/bin/' || mkdir -p '$(DESTDIR)/usr/bin/'
          cp ./main_test $(DESTDIR)/usr/bin/
  ```

```
  
* ```c
  //test.c
  #include<stdio.h>
  
  void main()
  {
  	printf("hello rpm\n");
  }
```

* 打包该文件夹：tar czvf test-0.0.1.tar.gz ./test-0.0.1/

* 创建./SPECS/test.spec配置文件．

* ```SPARQL
  Name:test
  Version:0.0.1
  Release:        1%{?dist}
  Summary:        123
  
  Source0:        %{name}-%{version}.tar.gz			#源码文件名
  
  Requires:gcc>=1.1.1
  License: GPL
  
  BuildRoot:%{_tmppath}/%{name}-%{version}-%{release}-root
  
  %description
  test
  
  
  %prep
  %setup -q
  
  
  %build						#编译时需要执行什么命令．
  make %{?_smp_mflags}
  
  %install					#make install时需要执行什么命令
  make install DESTDIR=%{buildroot}
  
  
  %files						#打rpm包时哪些程序需要打包
  %doc
  /usr/bin/main_test
  ```

* 运行rpmbuild -ba /root/rpmbuild/SPECS/test.spec --nodebuginfo打包程序．

### 2.打包nginx程序

nginx更简单，不需要写程序的Makefile文件，其他步骤和上边一样．除了spec配置文件．

```SPARQL
Name:test				#需要修改程序名称
Version:0.0.1			#需要修改程序版本号
Release:        1%{?dist}
Summary:        123

Source0:        %{name}-%{version}.tar.gz

Requires:gcc>=1.1.1
License: GPL

%description
test


%prep
%setup -q


%build
./configure --without-http_rewrite_module --without-http_gzip_module
make %{?_smp_mflags}


%install
%make_install


%files
%doc
/usr/local/nginx/*					#需要查看Makefile查看程序将安装到哪个目录下，就将哪个目录下的文件打包．



%changelog
```



## 三、rpm安装相关命令汇总

### 1.rpm安装操作：

rpm -i 需要安装的包文件名

举例如下：

```sh
rpm -i example.rpm #安装 example.rpm 包；

rpm -iv example.rpm #安装 example.rpm 包并在安装过程中显示正在安装的文件信息；

rpm -ivh example.rpm #安装 example.rpm 包并在安装过程中显示正在安装的文件信息及安装进度；
```

### 2.rpm查询操作

命令：

rpm -q …

附加查询命令：

a 查询所有已经安装的包以下两个附加命令用于查询安装包的信息；

i 显示安装包的信息；

l 显示安装包中的所有文件被安装到哪些目录下；

s 显示安装版中的所有文件状态及被安装到哪些目录下；以下两个附加命令用于指定需要查询的是安装包还是已安装后的文件；

p 查询的是安装包的信息；

f 查询的是已安装的某文件信息；

举例如下：

```sh
rpm -qa | grep tomcat4 	#查看 tomcat4 是否被安装；

rpm -qip example.rpm 	#查看 example.rpm 安装包的信息；

rpm -qif /bin/df 		#查看/bin/df 文件所在安装包的信息；

rpm -qlf /bin/df 		#查看/bin/df 文件所在安装包中的各个文件分别被安装到哪个目录下；
```

### 3.rpm卸载操作

命令：

rpm -e 需要卸载的安装包

在卸载之前，通常需要使用rpm -q …命令查出需要卸载的安装包名称。

举例如下：

```sh
rpm -e tomcat4 #卸载 tomcat4 软件包
```

### 4.rpm升级操作

命令：

rpm -U 需要升级的包

举例如下：

rpm -Uvh example.rpm 升级 example.rpm 软件包

### 5.rpm 验证操作

命令：

rpm -V 需要验证的包

举例如下：

```sh
rpm -Vf /etc/tomcat4/tomcat4.conf
```

输出信息类似如下：

S.5....T c /etc/tomcat4/tomcat4.conf

其中，S 表示文件大小修改过，T 表示文件日期修改过。限于篇幅，更多的验证信息请您参考rpm 帮助文件：man rpm

RPM 的其他附加命令:

```sh
--force 强制操作 如强制安装删除等；

--requires 显示该包的依赖关系；

--nodeps 忽略依赖关系并继续操作；
```

