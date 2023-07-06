# Light-Json-Parser-Qt

---

## JSON Formatter and Parser

这是一个基于Qt的JSON格式化和解析工具。它提供了一个用户界面，可以输入JSON字符串并对其进行格式化和压缩操作，同时还可以将JSON字符串解析为树状结构进行展示。

### 环境搭建

为了能够成功运行和构建该项目，需要满足以下要求：

- Qt框架：确保您的计算机上已经安装了Qt框架。可以从Qt官方网站（https://www.qt.io/） 下载并安装适用于您的操作系统的Qt版本。（作者在 m1-arm64 的环境下创建并成功运行）
- 如果是Windows系统可能出现一些编译问题，有可能是MSVC等编译器未设置系统环境变量等，请自行解决

### 项目部署和启动

1. 克隆代码仓库：

```shell
git clone https://github.com/Zongjy/Light-Json-Parser-Qt.git
```

2. 进入项目目录：

```shell
cd Light-Json-Parser-Qt
```

3. 打开终端，并执行以下命令来构建项目：

```shell
qmake
make
```

4. 构建成功后，执行以下命令来启动应用程序：

```shell
./test.app
```

5. 删除中间文件:

```shell
make clean
```

### 使用说明

启动应用程序后，将看到一个简单的用户界面。可以在输入框中输入要处理的JSON字符串，并点击相应的按钮来执行格式化、压缩等操作。解析后的JSON树状结构将显示在树形视图中。

- 格式化按钮：将JSON字符串格式化为带缩进和换行的格式。
- 压缩按钮：将JSON字符串压缩为紧凑格式。
- 全部展开按钮：展开树形视图中的所有节点。
- 全部收缩按钮：收缩树形视图中的所有节点。
- 选择文件按钮：选择并读取本地的JSON文件。
- 保存文件按钮：将格式化后的JSON保存到文件中。
- 清空按钮：清空输入框和树形视图。
- 复制按钮：将输入框中的内容复制到剪贴板。

### 示例

这里是一个使用该工具的示例：

1. 输入以下JSON字符串：

```json
{
  "name": "John Doe",
  "age": 30,
  "email": "johndoe@example.com",
  "address": {
    "street": "123 Main St",
    "city": "New York",
    "country": "USA"
  },
  "interests": ["programming", "reading", "traveling"]
}
```

2. 点击格式化按钮，将JSON字符串格式化为带缩进和换行的格式。

3. 点击全部展开按钮，展开树形视图中的所有节点。

4. 点击保存格式化后的JSON文件按钮，将格式化后的JSON保存到文件中。
