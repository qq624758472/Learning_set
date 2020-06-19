# docker的使用

1. <https://hub.docker.com/>  docker容器地址，类似github.

2. docker images   //列出本地镜像。

3. ``` 
   docker ps -a    //查看所有容器。运行和不运行的都显示。
   ```

4. docker search ubuntu   //查找ubuntu镜像。

5. docker pull ubuntu            //将名为ubuntu的镜像下载到本地。

6. docker pull qq624758472/ubuntu:latest   自己的乌邦图容器pull到本地。

7. docker tag ubuntu qq624758472/ubuntu                                           

8. docker push qq624758472/ubuntu

   这里要注意一个问题,给自己镜像命名的时候格式应该是: docker注册用户名/镜像名,比如我的docker用户名为 test123,那么我的镜像tag就为 test123/whalesay,不然是push不上去的

9. docker run -it ubuntu /bin/bash     //启动镜像。    这个好像也是创建。

10. docker run -itd --name ubuntu-test qq624758472/ubuntu  /bin/bash    创建镜像并在后台运行。

11. docker ps   列出所有正在运行的容器。

12. docker attach   进入正在后台运行的镜像中，exit会退出。 

13. ```
    docker exec -it 243c32535da7 /bin/bash   进入正在后台运行的镜像中，exit不会退出。
    ```

14. docker start ID   后台启动已经存在的容器。

15. docker rm -f 1e560fca3906   删除容器。



## 登录上传docker 镜像。 

1. 登录docker login

2. docker commit -m 'my python 3' ubuntu-test qq624758472/ubuntu:v0.0.1         将ubuntu-test容器打包成名字为qq624758472/ubuntu版本号是v0.0.1的镜像。

3. docker push qq624758472/ubuntu      将qq624758472/ubuntu 名字的镜像上传。

4. ```
   docker rmi hello-world   删除名字为hello-world的镜像。
   ```




# centos 下安装docker

### 使用 Docker 仓库进行安装

在新主机上首次安装 Docker Engine-Community 之前，需要设置 Docker 仓库。之后，您可以从仓库安装和更新 Docker。

**设置仓库**

安装所需的软件包。yum-utils 提供了 yum-config-manager ，并且 device mapper 存储驱动程序需要 device-mapper-persistent-data 和 lvm2。

```shell
sudo yum install -y yum-utils \
  device-mapper-persistent-data \
  lvm2
```

使用以下命令来设置稳定的仓库。

$ **sudo** yum-config-manager \
    --add-repo \
    https:**//**download.docker.com**/**linux**/**centos**/**docker-ce.repo

### 安装 Docker Engine-Community

安装最新版本的 Docker Engine-Community 和 containerd，或者转到下一步安装特定版本：

```shell
$ sudo yum install docker-ce docker-ce-cli containerd.io
```

如果提示您接受 GPG 密钥，请选是。

> **有多个 Docker 仓库吗？**
>
> 如果启用了多个 Docker 仓库，则在未在 yum install 或 yum update 命令中指定版本的情况下，进行的安装或更新将始终安装最高版本，这可能不适合您的稳定性需求。

Docker 安装完默认未启动。并且已经创建好 docker 用户组，但该用户组下没有用户。

**要安装特定版本的 Docker Engine-Community，请在存储库中列出可用版本，然后选择并安装：**

1、列出并排序您存储库中可用的版本。此示例按版本号（从高到低）对结果进行排序。

$ **yum list** docker-ce --showduplicates **|** **sort** -r

docker-ce.x86_64  3:18.09.1-3.el7                     docker-ce-stable
docker-ce.x86_64  3:18.09.0-3.el7                     docker-ce-stable
docker-ce.x86_64  18.06.1.ce-3.el7                    docker-ce-stable
docker-ce.x86_64  18.06.0.ce-3.el7                    docker-ce-stable

2、通过其完整的软件包名称安装特定版本，该软件包名称是软件包名称（docker-ce）加上版本字符串（第二列），从第一个冒号（:）一直到第一个连字符，并用连字符（-）分隔。例如：docker-ce-18.09.1。

```shell
$ sudo yum install docker-ce-<VERSION_STRING> docker-ce-cli-<VERSION_STRING> containerd.io
```

启动 Docker。

```shell
$ sudo systemctl start docker
```

通过运行 hello-world 映像来验证是否正确安装了 Docker Engine-Community 。

```shell
$ sudo docker run hello-world
```