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

}