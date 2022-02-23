# DDL_Management

## 建议

- 写注释
- 勤提交更改，勤`git commit -m"写明说明文字"`
- 别人更改后用`git pull`指令更新本地文件

## 进展

2-21：

- 初步定义好`DDL`类的数据，但无接口
- 在不同设备上读取用户数据`user.txt`测试通过

2-22：

- 明确代码规范
- 初步构思功能

2-23：

- DDL类新增
  - 紧迫度接口`GetUrgency()`返回百分数
  - 预期完成时间`m_duration`
- DDL类基类改为`QDateTime`

## 可预见的下一步

- 丰富完善`DDL`类
- 设计`user.txt`的编码方式用于存储用户DDL信息
- 设计界面
