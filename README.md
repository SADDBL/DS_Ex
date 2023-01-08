# 华中科技大学人工智能与自动化学院自数据结构上机实验

该上机实验主要面向韩守东老师班级的学生，为他们编写代码提供算法和数据结构设计方面的参考。本代码同样参考和借鉴了学长学姐的代码。本代码实现的任务有：随机取点+分水岭算法，四原色填色与可视化，堆排序和折半查找+可视化；后两个任务需要在实现第一个任务的基础上进行。

## 软件依赖

+ 编译器使用Visual Studio 2019
+ 编程语言以C语言为主，使用了少量C++语言
+ 第三方库使用OpenCv库，具体安装方法和流程可自行上网搜索

## 文件说明

才此只简述文件内容，具体算法请查看实验报告。

### main.cpp

主函数文件，除了包含主要流程外，分水岭算法的实现也在该文件。

### mission1.cpp和mission1.h

实现随机取点，算法基于快速泊松圆盘采样算法（可参考：有人知道泊松盘采样算法吗？看了几天没看懂？ - 派大西的回答 - 知乎 https://www.zhihu.com/question/276554643/answer/2399127501     参考论文已经放到仓库）。

### mission2.cpp和mission2.h

实现四原色填色与可视化。

### mission3.cpp和mission3.h

实现堆排序、折半查找和可视化。

### 1.jpg

本实验使用的图片，分辨率为591×608。

## 使用

只需要更改图片路径（我使用的是绝对路径），需要修改的文件有main.cpp、mission2.cpp和mission3.cpp。修改后直接运行即可。