package main

import (
	"fmt"
	"os"
	"os/exec"
)

func main() {
	//输出主函数的参数
	for k,v := range os.Args{
		fmt.Printf("k:%d, v:%s\n",k,v)
	}

	if os.Args[1] == "run"{
		mySystem()
	}
}


//调用系统命令
func mySystem(){
	cmd := exec.Command("cp","/proc/cpuinfo","./")
	buf, _ := cmd.Output()
	print(string(buf))
}


//判断文件是否存在
func fileExist(){
	var a  bool
	a, _ = PathExists("/home/hao/1.c")
	fmt.Println(a)
	if a {
		fmt.Println("1")
	} else {
		fmt.Println("2")
	}
}
func PathExists(path string) (bool, error) {
	_, err := os.Stat(path)
	if err == nil {
		return true, nil
	}
	if os.IsNotExist(err) {
		return false, nil
	}
	return false, err
}