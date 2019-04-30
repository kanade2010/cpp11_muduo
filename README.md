# cpp11_muduo
c++11 implementation of muduo net lib and muduo asynclogging.

original author github: https://github.com/chenshuo/muduo


## 整理的muduo网络库的c++11版

因为SimpleMuduo不是稳定版，于是新建了一个仓库维护下这个c++11版。
小bug会在这里修复，SimpleMuduo不再更新，有兴趣的话可以一起维护一下。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190430180209604.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE3MzA4MzIx,size_16,color_FFFFFF,t_70)


```cpp
#include <async_logging>
#include <muduo_server>

void on_connection(const muduo::TcpConnectionPtr& conn){
  LOG_DEBUG << "new conn from " << conn->peerAddress().toIpPort();
}

void on_message(const muduo::TcpConnectionPtr& conn, muduo::Buffer* buffer, ssize_t len){
  LOG_DEBUG << "on message : " << len << " bytes " << buffer->peek();
  buffer->retrieve(len);
}

int main(){

  Logger::setLogLevel(Logger::DEBUG);

  muduo::EventLoop loop;

  InetAddress localAddr(8080);
  muduo::TcpServer tcp_server(&loop, localAddr);

  tcp_server.setConnectionCallBack(std::bind(on_connection, std::placeholders::_1));
  tcp_server.setMessageCallBack(std::bind(on_message, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
  //tcp_server.setCloseCallBack(std::bind(on_close, std::placeholders::_1));
  tcp_server.start();

  loop.loop();

  getchar();
}
```

![Alt text](https://img-blog.csdnimg.cn/20190429233902624.png)

-------------------2019/4/30
## 添加http处理代码及测试样例 : 
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190430175347932.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE3MzA4MzIx,size_16,color_FFFFFF,t_70)
