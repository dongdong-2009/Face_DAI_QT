#ifndef NETWORK_H
#define NETWORK_H

#include <stdio.h>

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

#include "global.h"
#include "TSingleIns.h"
#include "facerec.h"

#define SERVER_PORT     8080
#define IMG_WIDTH       640
#define IMG_HEIGHT      480
#define PACKAGE_NUM     2
#define BUFFER_SIZE     IMG_HEIGHT*IMG_WIDTH*3/PACKAGE_NUM //default format "CV_8UC3"
#define MAX_EVENT_NUMBER 		1024
#define MAX_EPOLL_FD	10

struct sentbuf
{
    char buf[BUFFER_SIZE];
    int flag;
};

struct node {  
	int n_number;  
	int fd;
	struct node *n_next;  
};    

class server_network: public TSingleIns<server_network>
{
public:
    explicit server_network(void);
    ~server_network();
    
    int receiveFromClient(int sockfd,cv::Mat& image);
    int sendResultToClient(int sockfd, int result);
    int sendIMageToClient(cv::Mat image);
	void handleEpollEvent();
	int initSocket(void);
	void TEST();
	
private:
    void keepSocketConnect(void);
    void closeSocket(int sfd);
    int  connectServer(char* ip, int port);
	void setnonblocking(int sfd);
	void addfd(int epollfd, int fd, bool oneshot); 
	//static void cleanup_handler(void *arg);  
	//static void* thread_func(void *arg);
	
private:
    int listenfd;
    int clientfd;
    int epollfd;// server context 
    struct epoll_event events[MAX_EVENT_NUMBER];
    struct sockaddr_in  local_addr;

    struct sentbuf sentData;
    struct sentbuf recvData;
    //struct node *p; 
 	//struct node *headnode;
 	vector<SCliContext> fdsinfo;
 	//faceRecognize *mp_faceRec; 
};

#endif
