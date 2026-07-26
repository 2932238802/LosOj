# LosOj — Online Judge System

C++17 实现的在线评测系统，支持代码提交、编译、运行和判题。

## 架构

两个独立 HTTP 服务（基于 [cpp-httplib](https://github.com/yhirose/cpp-httplib)）：

```
┌─────────────┐     POST /compile     ┌────────────────┐
│             │ ──────────────────►   │  编译服务器 x3   │
│  ojServer   │                       │  (8081/8082/8083)│
│  (8080)     │ ◄──────────────────── │                │
│    MVC      │     返回结果           │ fork + exec    │
└─────────────┘                       └────────────────┘
     │
     ├── MySQL (MYSQL_MODEL)
     └── 文件 (FILE_MODEL)
```

### ojServer — 前端服务 (端口 8080)

- MVC 模式：`OjController` / `OjModel` / `OjView`
- 路由：
  - `GET /all_questions` — 题目列表
  - `GET /question/{id}` — 题目详情
  - `POST /judge/{id}` — 提交代码判题
- 模板渲染：[inja](https://github.com/pantor/inja)
- 前端：Ace 编辑器，分栏布局
- 负载均衡：`conf/serviceHost.conf` 配置多个编译服务器

### compileServer — 编译服务 (端口 8081-8083)

- 接收 `POST /compile`
- 流程：写入临时文件 → `fork()` + `execlp("g++", ...)` 编译 → `fork()` + `setrlimit()` 运行
- 错误信号中文映射：SIGSEGV → 段错误, SIGXCPU → CPU 超时等

### 共享库 (`common/`)

| 模块 | 功能 |
|------|------|
| LosFile | 文件读写、生成唯一文件名 |
| LosPath | 临时文件路径（源文件、可执行文件、输入输出等） |
| LosTime | 时间戳 |
| LosLog | 日志输出（`DEBUG_TEST` 始终开启） |
| LosStr | 字符串分割 |

## 构建

依赖 [vcpkg](https://github.com/microsoft/vcpkg) 安装，路径硬编码在子目录的 `makefile` 中。

```bash
# 构建 ojServer
cd ojServer
make build       # 增量构建
make rebuild     # cmake 配置 + 构建
make clean       # 删除 build 目录
make run         # 运行 ./OjServer

# 构建 compileServer（同上，二进制为 CompileServer）
cd compileServer
make build
make run
```

### 依赖

| 服务 | 依赖 |
|------|------|
| ojServer | Boost, inja, nlohmann_json, jsoncpp, libmysql |
| compileServer | jsoncpp, httplib |

## 配置

### 题目存储

在 `ojServer/ojModel.h` 中通过 `#define` 切换：

```cpp
#define MYSQL_MODEL  // MySQL 模式（默认）
// #define FILE_MODEL  // 文件模式
```

- **MySQL 模式**：连接 `Losoj` 数据库的 `oj_questions` 表
- **文件模式**：读取 `questions/questions.list`，题目描述和代码在 `questions/{id}/` 下

### 编译服务器

`ojServer/conf/serviceHost.conf`：

```
127.0.0.1:8081
127.0.0.1:8082
127.0.0.1:8083
```

## 代码风格

`.clang-format`：LLVM 风格，Allman 大括号，4 空格缩进，100 列限制，左对齐指针。

## 测试

手动编译的测试程序在 `test/` 目录下，无测试框架。

```bash
cd test/myInja && make
./run
```

## 项目结构

```
LosOj/
├── ojServer/          # 前端服务
│   ├── main.cc        # 入口，路由注册
│   ├── ojController.* # 控制器 + 负载均衡
│   ├── ojModel.*      # 数据模型（MySQL/文件）
│   ├── ojView.*       # 视图（inja 模板渲染）
│   ├── common/        # 共享库
│   ├── conf/          # 编译服务器配置
│   ├── questions/     # 题目数据（文件模式）
│   ├── template/      # inja HTML 模板
│   ├── static/        # 前端资源
│   └── wwwroot/       # 静态文件根目录
├── compileServer/     # 编译服务
│   ├── main.cc        # 入口
│   ├── compiler.*     # 编译（fork + g++）
│   ├── runner.*       # 运行（fork + rlimit）
│   ├── compileAndRun.* # 编排
│   └── common/        # 共享库
└── test/              # 手动测试
```
