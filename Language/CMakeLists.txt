CMAKE_MINIMUM_REQUIRED(VERSION 3.5)
PROJECT(c++)        #工程名字


#[[一般情况下，我们的项目各个子项目都在一个总的项目根目录下，但有的时候，我们需要使用外部的文件夹，怎么办呢？ 
add_subdirectory命令，可以将指定的文件夹加到build任务列表中。]]
ADD_SUBDIRECTORY(c++)      
