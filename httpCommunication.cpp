#include "public.h"
#include <sys/stat.h>
#include <fstream>
void* creatHttpServer(void *param)
{
    // 1. 创建通信套接字
    int server = socket(AF_INET,SOCK_STREAM,0);
    if(server<0){
      std::cerr<<"socket创建失败"<<std::endl;
      return nullptr;
    }
    //2. 绑定ip和端口
    struct sockaddr_in local;
    local.sin_family = AF_INET;        //协议家族
    local.sin_port = htons(8111);       //绑定端口，接收发送给该端口的所有数据
    local.sin_addr.s_addr = INADDR_ANY;    //IP地址不限，不论哪个IP地址给服务端发数据，只要端口号对应，全部接收

    if (bind(server,(struct sockaddr*)&local,sizeof(local))<0)
    {
      std::cerr<<"服务端绑定失败"<<std::endl;
      return nullptr;
    }
    //3. 将套接字设为监听状态，监听来自客户端的请求
    int ret = listen(server,5);
    if(ret < 0){
      std::cerr<<"listen调用失败"<<std::endl;
      return nullptr;
    }
    // 4. 通过死循环不断接受请求
    for(;;)
    {
        struct sockaddr_in peer;
        socklen_t len = sizeof(peer);
        int new_sock = accept(server,(struct sockaddr*)&peer,&len);
        if (new_sock<0){
            continue;
        }
        const char *client_ip = inet_ntoa(peer.sin_addr);
        uint16_t client_port = ntohs(peer.sin_port);
        std::cout<<"客户端ip:"<<client_ip<<" port:"<<client_port<<"已连接...."<<std::endl;

        //while(1)
        {
            char buffer[1024];
            memset(buffer,0,sizeof(buffer));
            //服务端接收浏览器发来的请求
            ssize_t s = recv(new_sock, buffer, sizeof(buffer), 0);
            if (s > 0)
            {
            buffer[s] = 0;
            std::cout << buffer;
//            std::string http_response = "http/1.0 200 OK\n";            //构建响应
//            http_response += "Set-Cookie: id=lds\n";                    //设置Cookie信息
//            http_response += "Set-Cookie: passwd=123456\n";
//            http_response += "\n";

//            send(new_sock,http_response.c_str(),http_response.size(),0);     //服务器发送响应
            /***************************请求行、请求头部分****************************/
            std::string http_response = "http/1.0 200 OK\n";
            http_response += "Content-Type:text/html;charset=utf-8\n";    //说明响应正文是一个html文件
            http_response += "Content-Length:";    //描述响应正文的大小

            struct stat st;
            stat("./webroot/index.html", &st); //使用stat函数来获取指定路径下文件的大小
            http_response += std::to_string(st.st_size);
            http_response += "\n"; // Content-Length：xxx 所在行的 ‘\n’
            http_response += "\n"; //空行

            /***************************请求正文部分****************************/
            std::ifstream in("./webroot/index.html");
            if (!in.is_open())
            {
              std::cerr << "文件打开失败" << std::endl;
            }
            else
            {
              std::string content;
              std::string line;
              while (std::getline(in,line)) //按行读取文件内容
              {
                content += line;  //将读取到的文件内容拼接到content中
              }
              http_response += content;
            }
            send(new_sock,http_response.c_str(),http_response.size(),0);  //发送响应
            }
            else if(s==0){
                //std::cout<<"客户端已退出..."<<std::endl;
            }
            else{
                std::cerr<<"读取出错..."<<std::endl;
                break;
            }
        }
        close(new_sock);
    }
    close(server);
}
void* creatHttpClient(void *param)
{

}
