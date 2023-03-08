#include "public.h"

void* creatUdpServer(void *param)
{
    //1. 创建服务端套接字
    int server = socket(AF_INET,SOCK_DGRAM,0);
    if(server<0)
    {
        std::cerr<<"socket创建失败"<<std::endl;
        return nullptr;
    }
    //2. 绑定IP端口号
    //  a.填充地址信息
    struct sockaddr_in local;
    local.sin_family = AF_INET;           //选择协议家族(通信方式)AF_INET
    uint16_t port = 8080;
    local.sin_port = htons(port);         //绑定端口，因为port是16位int型变量，是主机序列，需要转化为网络字节序
    local.sin_addr.s_addr = INADDR_ANY;    //如果是接收绑定指定ip的数据，那就是inet_addr("你的ip")
                                           //如果不指定，那就是INADDR_ANY，表明凡是发送到8080端口的
                                           //数据，你都选择接收，不管是服务器上哪个ip地址收到的

    //   b.使用bind函数绑定
    int ret = bind(server,(struct sockaddr*)&local,sizeof(local));
    if (ret<0)
    {
        //说明绑定失败
        std::cerr<<"bind绑定失败"<<errno<<std::endl;
        return nullptr;
    }
    cerr<<"server start_!!!!!!!!"<<endl;
    while (1)
    {
        /*********接收来自客户端的数据*********/
        char buffer[1024];                 //用于存放对方发送的数据
        struct sockaddr_in src;            //用于保存对端的地址信息
        socklen_t len = sizeof(src);       //对端地址信息的大小
        recvfrom(server,buffer,sizeof(buffer)-1,0,(struct sockaddr*)&src,&len);
        //std::cout<<"client# "<< buffer <<std::endl;

        std::string msg = "收到来自客户端的信息：";
        msg.append(buffer);
        sendto(server,msg.c_str(),msg.size(),0,(struct sockaddr*)&src,len);
    }
    close(server);
}
void* creatUdpClient(void *param)
{
    //创建客户端套接字
    int client = socket(AF_INET,SOCK_DGRAM,0);
    if(client<0){
        std::cerr << "socket创建失败" << std::endl;
        return nullptr;
    }
    //客户端要给谁发？？
    struct sockaddr_in dst;
    dst.sin_family = AF_INET;
    dst.sin_port = htons(8080);            //服务器的端口号
    dst.sin_addr.s_addr = inet_addr("127.0.0.1");   //服务器的ip地址
    cerr<<"client start_!!!!!"<<endl;
    while (1)
    {
        //从键盘获取内容
        std::string msg;
        std::cout<<"client# ";
        std::cin>>msg;

        //向服务器发送数据
        socklen_t len = sizeof(dst);
        sendto(client,msg.c_str(),msg.size(),0,(struct sockaddr*)&dst,len);

        char buffer[1024];
        //接收服务器做出的响应
        int ret = recvfrom(client,buffer,sizeof(buffer)-1,0,(struct sockaddr*)&dst,&len);
        if(ret>0){
          std::cout<<"server# "<< buffer <<std::endl;
        }
    }
    close(client);
}
