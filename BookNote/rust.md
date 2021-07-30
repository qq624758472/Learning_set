# rust

## Linux下安装

```javascript
curl https://sh.rustup.rs -sSf | sh
```

也可以直接到这里下载https://forge.rust-lang.org/other-installation-methods.html

刚安装完需要：

```sh
source ~/.bashrc
```



## 配置环境

Linux环境：如果自动配置异常，则需手动配置。

```javascript
export PATH="$HOME/.cargo/bin:$PATH" >> ~/.bash_profile
```



## Cargo命令

```javascript
#新建一个项目：
cargo new rust-demo

#编译项目：
cargo build

#编译前检查
cargo check

#编译并运行
cargo run

#Cargo build 除了编译项目，还有一个作用就是安装依赖包，依赖包会写在Cargo.toml中
#在[dependencies]配置块中，每行一个：
[dependencies]
xxxx=1.0.0

#通常cargo build编译的是debug版本。构建发布版本：
cargo build --release
```













