# Socket

**单线程Socket编程：**

server：socket -> bind(绑定地址) -> listen（监听） -> accept(blocking, 3 handshark) -> send

client:     socket -> connnect(blocking, 3 handshark) -> rev

![img](F:\Github\github\Socket\picture\Socket.jpg)