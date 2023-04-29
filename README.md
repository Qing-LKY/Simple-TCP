# Simple TCP server/client

## 题目要求

编写TCP网络交互客户端和服务器端实例：

- 客户端读取某个文件并将内容发送给服务器端
- 服务器端读取客户端消息之后，将客户端消息、客户端消息长度写入本地文件，同时将这些信息返回给客户端
- 实现Ctrl+C信号的捕获并正常退出
- 服务器端接收连接时使用新的进程处理，实现并发处理
- 了解服务器端的设置（地址、端口、侦听队列）

## 使用方式

编译:

```bash
make
```

使用：

```text
Usage: ./simple-tcp [-s [-q 5]] [-f FILE] [-a 0.0.0.0] [-p 8888]
 -s, --server      start as a server
 -q, --queue NUM   set max number of pending connections (ignore if client)
 -a, --addr IPv4   set address (numbers-and-dots notation)
 -p, --port NUM    set port
 -f, --file FILE   upload filepath (ignore when server)
 -h, --help        display this message and exit
```

示例：

```bash
# 服务端 开启服务
./simple-tcp -s -q 5 -a 127.0.0.1 -p 5555
# 客户端 上传内容 test.txt
./simple-tcp -a 127.0.0.1 -p 5555 -f test.txt
```

## 注意事项

作为服务端时，需要保证执行目录下有 logs/ 文件夹。
