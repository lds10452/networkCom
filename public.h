#ifndef PUBLIC_H
#define PUBLIC_H
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
using namespace std;

void* creatUdpServer(void *param);
void* creatUdpClient(void *param);

void* creatTcpServer(void *param);
void* creatTcpClient(void *param);

void* creatHttpServer(void *param);
void* creatHttpClient(void *param);
#endif // PUBLIC_H
