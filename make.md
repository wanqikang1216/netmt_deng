`只要make看到一个[.o]文件，它就会自动的把[.c]文件加在依赖关系中`

**获取当前目录下的所有C文件**

SRC = $(wildcard *.c)



- PRIVATE - 目录被添加到目标（库）的包含路径中。
- INTERFACE - 目录没有被添加到目标（库）的包含路径中，而是链接了这个库的其他目标（库或者可执行程序）包含路径中
- PUBLIC - 目录既被添加到目标（库）的包含路径中，同时添加到了链接了这个库的其他目标（库或者可执行程序）的包含路径中

生成静态库

`add_library(hello_library STATIC  src/Hello.cpp)`

生成动态库

`add_library(hello_library SHARED src/Hello.cpp)`

