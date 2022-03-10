# DDL_Management

## 建议

- 写注释
- 勤提交更改，勤`git commit -m"写明说明文字"`
- 别人更改后用`git pull`指令更新本地文件

## 进展
3-9:
- 界面已经基本按预期实现
- 欠缺的功能：一组前驱后继被全部移除，右边的组要及时补过来。明天解决这个事情
- 之后就是Menu的功能了


3-5:
- 删除功能开发最大的技术障碍被解决
- 可以预见明天的工作将是先完善删除功能，然后开始细化到前驱、后继等功能
- 已经逐渐成型

3-4:
- 可以通过鼠标单击创建ddl了
- 解决了一些嵌套问题
- ddl删除功能正在开发

2-28:
- 可以预见到接下来MVC中V的主要部分是对MainWindow文件进行操作。
- 创建了ddl_block类（继承自QLabel）用于在界面中调用ddl
- 暂时不考虑界面美观问题

2-25：

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
