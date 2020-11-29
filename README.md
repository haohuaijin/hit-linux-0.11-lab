# linux-0.11—lab

哈工大 李治军老师操作系统课试验环境以及lab答案

## 说明
- linux-0.11代码：存放linux-0.11的源码，不过缺少kernel/chr_drv/keyboard.s文件(原因mac不区分大小写)；
- 以x.开头：各个实验需要更改的文件；
-  实验报告及笔记：各个实验的实验报告和一些笔记。


## 实验完成进度
- [x] 操作系统的启动
- [x] 系统调用
- [x] 进程运行轨迹的跟踪与统计
- [x] 基于内核栈切换的进程切换
- [x] 信号量的实现和应用
- [x] 地址映射与共享
- [x] 终端设备的控制
- [x] proc文件系统的实现
## 实验环境
Ubuntu 18.04.4 LTS

## 依赖的包
 
 ```
 gcc-3.4
 bin86
 libc6-dev-i386
 build-essential
 libsm6:i386
 libx11-6:i386
 libxpm4:i386
 ```
 
 ## 编译linux
 在linux-0.11文件夹下执行：
 `make all`
 
 ## 运行linux
 在oslab目录下执行：
 `./run`
