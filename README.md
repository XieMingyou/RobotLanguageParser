# RobotLanguageParser
RobotLanguageParser
Copyright (C) 20018 - 2020 by XieMingyou. All rights reserved.

这是本人自主开发的机器人语言解析器程序，本人保留版权。

程序功能简介：  
- 实现变量文件和程序文件的编译：利用flex和bison工具辅助词法和语法分析，采用树形结构建立符号表和抽象语义分析树，支持数据类型错误、参数不匹配、变量未定义等10余种词法语法错误类型的判断和提示；
- 实现子语句的解析：通过遍历和递归语义分析树实现对10余种变量定义语句和运动、设置、系统功能、流程控制等四类指令组的30余种控制指令语句的解析；

打开RobotLanguageParser程序：  
RobotLanguageParser.sln
