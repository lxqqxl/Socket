# Socket

**监听套接字和已连接套接字：**

![](F:\Github\github\Socket\picture\Connected sockets and listening sockets..jpg)

**单进程Socket编程：**

server：socket -> bind(绑定地址) -> listen（监听） -> accept(blocking, 3 handshark) -> send

client:     socket -> connnect(blocking, 3 handshark) -> rev

![img](F:\Github\github\Socket\picture\Socket.jpg)

**解决单进程Server强制断开以后无法连接的问题：**

1.之前在测试的时候，当我们启动server,然后启动client，再用Ctrl+C使server终止，这时**马上**再运行server,结果是：bind error:Address already in use   这时因为虽然server的应用程序终止了，但TCP协议的连接并没有完全断开，回忆一下TCP四次挥手的过程，因此不能再监听同样的server端口。

​		在server的TCP没有连接没有完全断开之前不允许重新监听是不合理的，TCP没有完全断开是指connfd（127.0.0.1:8080）没有完全断开，而我们重新监听的是listenfd（0.0.0.0:8080），虽然占用同一个端口，但IP地址不同，connfd对应的是与某个客户端通讯的一个具体的IP地址，而Listenfd对应的是wildcard address（通配符地址）。

​		解决这一办法就是使用setsockopt函数，设置socket描述符的选项SO_REUSEADDR为1，表示允许创建端口号相同但IP地址不同的多个socket描述符，通常在socket()和bind（）调用之间调用该函数。

2.想想一个场景，当server服务器异常退出时，这会儿它处于TIME_WAIT状态（等待2msl长的时间），主动关闭的一方处于TIME_WAIT状态，而ip和端口号依然被占用，此时客户端无法连接到服务器，而服务器又无法重新启动，会造成网络数据的流失异常等多种情况，解决办法就是采用setsockopt函数。

3.函数原型，所在头文件及其返回值

```c++
#include <sys/types.h>
#include <sys/socket.h>

//函数原型
int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
//代码中调用
setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));//int opt = 1;
//on success, zero is returned. on error, -1 is returned
```

