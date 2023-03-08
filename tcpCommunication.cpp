#include "public.h"

void* creatTcpServer(void *param)
{
    // 1. 创建通信套接字
    int server = socket(AF_INET,SOCK_STREAM,0);
    if(server<0){
      std::cerr<<"socket创建失败"<<std::endl;
      close(server);
      return nullptr;
    }
    //2. 绑定ip和端口
    struct sockaddr_in local;
    local.sin_family = AF_INET;        //协议家族
    local.sin_port = htons(8880);       //绑定端口，接收发送给该端口的所有数据
    local.sin_addr.s_addr = INADDR_ANY;    //IP地址不限，不论哪个IP地址给服务端发数据，只要端口号对应，全部接收

    if (bind(server,(struct sockaddr*)&local,sizeof(local))<0)
    {
      std::cerr<<"服务端绑定失败"<<std::endl;
      close(server);
      return nullptr;
    }
    //3. 将套接字设为监听状态，监听来自客户端的请求
    int ret = listen(server,5);
    if(ret < 0){
      std::cerr<<"listen调用失败"<<std::endl;
      close(server);
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

        while(1)
        {
            char buffer[1024];
            memset(buffer,0,sizeof(buffer));
            int s = read(new_sock,buffer,sizeof(buffer)-1);        //读取客户端发来的数据
            if(s>0){
                buffer[s]=0;
                //std::cout<<"client# "<<buffer<<std::endl;

                std::string msg = "服务端收到了客户端的消息: ";
                msg.append(buffer);
                write(new_sock,msg.c_str(),msg.size());            //将接收到的内容返回给客户端
            }
            else if(s==0){
                std::cout<<"客户端已退出..."<<std::endl;
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

void* creatTcpClient(void *param)
{
    //1. 创建客户端套接字
    int client = socket(AF_INET, SOCK_STREAM, 0);
    if (client < 0)
    {
        std::cerr << "socket创建失败" << std::endl;
        return nullptr;
    }
    //2. 准备好服务端的地址信息
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    uint16_t port = 8880;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");//192.168.5.105

    //3. 连接服务端
    if(connect(client,(struct sockaddr*)&server,sizeof(server))<0){
        std::cerr<<"客户端连接失败"<<std::endl;
        return nullptr;
    }
    std::cout<<"连接成功..."<<std::endl;

    while (1)
    {
        char buffer[1024];
        std::cout<<"client# ";
        //fgets(buffer,sizeof(buffer),stdin);        //从键盘获取输入
        //write(client,buffer,sizeof(buffer));        //发送给服务端
        string msg;
        cin>>msg;
        write(client,msg.c_str(),msg.size());

        ssize_t s = read(client,buffer,sizeof(buffer)-1);
        if(s>0){
            buffer[s]=0;
            std::cout<<"server# "<<buffer<<std::endl;
        }

    }
    close(client);
}
