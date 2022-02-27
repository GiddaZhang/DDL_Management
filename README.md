# DDL_Management

## 功能与界面

![image](https://github.com/GiddaZhang/Image/blob/main/main.jpg)

![image](https://github.com/GiddaZhang/Image/blob/main/sub.jpg)

## 建议

- 写注释
- 勤提交更改，勤`git commit -m"写明说明文字"`
- 别人更改后用`git pull`指令更新本地文件

## 进展

2-27：

- 确定功能

2-24：

- DDL类新增常引用公有成员用于外部只读访问

2-23：

- DDL类新增
  - 紧迫度接口`GetUrgency()`返回百分数，以及各成员接口
  - 成员：完成度，完成状态、提交状态、前驱和后继任务、任务名称、任务时长、结束时间
- DDL类基类改为`QDateTime`

2-22：

- 明确代码规范
- 初步构思功能

2-21：

- 初步定义好`DDL`类的数据，但无接口
- 在不同设备上读取用户数据`user.txt`测试通过

## 可预见的下一步

- 丰富完善`DDL`类
- 设计`user.txt`的编码方式用于存储用户DDL信息
- 设计界面
