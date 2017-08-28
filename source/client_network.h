#ifndef CLIENT_NETWORK
#define CLIENT_NETWORK
#include "global.h"

#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>

#define SERVER_IP       "192.168.1.210"
//#define SERVER_IP       "127.0.0.1"
#define SERVER_PORT     8080
#define IMG_WIDTH       640
#define IMG_HEIGHT      480
#define PACKAGE_NUM     32
#define BUFFER_SIZE     IMG_HEIGHT*IMG_WIDTH*3/PACKAGE_NUM //default format "CV_8UC3"

struct sentbuf
{
    char buf[BUFFER_SIZE];
    int flag;
};

class client_network
{
public:
    explicit client_network(void);
    ~client_network();
    int receiveResultFromServer(cv::Mat& image);
    int sendMatDataToServer(cv::Mat image);

private:
    int initSocket(void);
    void setSocketOption(int sfd);
    void keepSocketConnect(void);
    void disconnectSocket();
    int connectServer(char* ip, int port);

private:
    int sockfd;
    struct sentbuf sentData;
    struct sentbuf recvData;
};

#endif // CLIENT_NETWORK

