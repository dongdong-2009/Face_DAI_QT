#ifndef NETWORK_H
#define NETWORK_H

#include "global.h"
#include "TSingleIns.h"
#include "config.h"

#define IMG_WIDTH       640
#define IMG_HEIGHT      480
#define PACKAGE_NUM     2
#define BUFFER_SIZE     IMG_HEIGHT*IMG_WIDTH*3/PACKAGE_NUM //default format "CV_8UC3"

struct sentbuf
{
    char buf[BUFFER_SIZE];
    int flag;
};

class Network:public TSingleIns<Network>
{
public:
    explicit Network(void);
    ~Network();
    int receiveFromServer(void);
    int sendMatToServer(cv::Mat image);
    void keepSocketConnect(void);
    bool isSocketStatusOk(){return sockfd < 0?false:true; }
    int getCurrentSocket();
    
private:
    void SetSocketOption(int sfd);
    int initSocket(void);
    void disconnectSocket();
    int  connectServer(char* ip, int port);

private:
    int sockfd;
    struct sentbuf sentData;
    struct sentbuf recvData;  
    OptConfig m_configData;  
};

#endif // NETWORK_HEAD
