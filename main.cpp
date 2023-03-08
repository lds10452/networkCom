#include "public.h"
#include <pthread.h>
//#define USE_UDP
//#define USE_TCP
int main()
{
    pthread_t sTid,cTid;
#ifdef USE_UDP
    pthread_create(&sTid,nullptr,creatUdpServer,nullptr);
    pthread_create(&cTid,nullptr,creatUdpClient,nullptr);
#elif defined USE_TCP
    pthread_create(&sTid,nullptr,creatTcpServer,nullptr);
    pthread_create(&cTid,nullptr,creatTcpClient,nullptr);
#else
    pthread_create(&sTid,nullptr,creatHttpServer,nullptr);
    //pthread_create(&cTid,nullptr,creatHttpClient,nullptr);
#endif
    pthread_join(sTid,nullptr);
    //pthread_join(cTid,nullptr);
    cout<<"process exit.."<<endl;
    return 0;
}
