#include <async_logging>
#include <muduo_server>

#include <HttpContext.hpp>
#include <HttpResponse.hpp>

void on_connection(const muduo::TcpConnectionPtr& conn){
  LOG_DEBUG << "new conn from " << conn->peerAddress().toIpPort();
}

void onRequest(const muduo::TcpConnectionPtr& conn, const http::HttpRequest& req)
{
  const std::string& connection = req.getHeader("Connection");
  bool close = connection == "close" || connection == "Close" ||
    (req.getVersion() == http::HttpRequest::kHttp10 && connection != "Keep-Alive");
  http::HttpResponse response(close);
  response.setStatusCode(http::HttpResponse::k200Ok);
  response.setBody("welcome!\n");
  muduo::Buffer buf;
  response.appendToBuffer(&buf);
  LOG_DEBUG << "http buffer :\n" << buf.peek();
  conn->send(&buf);
  if (response.closeConnection())
  {
    conn->shutdown();
  }
}

void on_message(const muduo::TcpConnectionPtr& conn, muduo::Buffer* buffer, ssize_t len){
    LOG_DEBUG << "on message : " << len << " bytes " << buffer->peek();
 
    LOG_TRACE << "readAbleBytes:" << buffer->readableBytes() << " len:" << len;

    http::HttpContext context;

    if (!context.parseRequest(buffer, TimeStamp::now()))
    {
        conn->send("HTTP/1.1 400 Bad Request\r\n\r\n");
        conn->shutdown();
    }

    if (context.gotAll())
    {
        LOG_TRACE << "request version: " << context.request().getVersion();
        LOG_TRACE << "request method : " << context.request().methodString();
        LOG_TRACE << "request path   : " << context.request().path();
        onRequest(conn, context.request());
        context.reset();
    }

    LOG_TRACE << "readAbleBytes:" << buffer->readableBytes() << " len:" << len;
}

int main(){
  Logger::setLogLevel(Logger::TRACE);

  muduo::EventLoop loop;

  InetAddress localAddr(8080);
  muduo::TcpServer tcp_server(&loop, localAddr);

  tcp_server.setConnectionCallBack(std::bind(on_connection, std::placeholders::_1));
  tcp_server.setMessageCallBack(std::bind(on_message, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
  tcp_server.start();

  loop.loop();

}
