#include "network.h"

Network::Network(void)
{
   initSocket();
}

Network::~Network()
{
    disconnectSocket();
    Config::ReleaseInstance();
}

int Network::initSocket(void)
{
	memset0(m_configData);
    Config::GetInstance()->ReadConfigData(m_configData);
    sockfd = -1;
}

void Network::SetSocketOption(int sfd)
{
    int keepAlive = 1;
    int keepIdle = 60;
    int keepInterval = 5;
    int keepCount = 3;
    int reuseAddr = 1;

    setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &reuseAddr, sizeof(reuseAddr));
    setsockopt(sfd, SOL_SOCKET, SO_KEEPALIVE, &keepAlive, sizeof(keepAlive));
    setsockopt(sfd, SOL_TCP, TCP_KEEPIDLE, (void*) &keepIdle,sizeof(keepIdle));
    setsockopt(sfd, SOL_TCP, TCP_KEEPINTVL, (void*) &keepInterval,sizeof(keepInterval));
    setsockopt(sfd, SOL_TCP, TCP_KEEPCNT, (void *) &keepCount,sizeof(keepCount));
}

void Network::keepSocketConnect(void)
{
    sockfd = connectServer(m_configData.serverIp, m_configData.serverPort);
    if(sockfd < 0) {
        PRT_INFO("can't connect server host!\n");      
    }
    else {
        PRT_INFO("alreay connect server host,sockfd = %d.\n",sockfd);
    }
}

int Network::connectServer(char* ip, int port)
{
    PRT_INFO("init Socket!\n");
    int sfd = -1;
    signal(SIGPIPE,SIG_IGN);

    if((sfd = socket(AF_INET,SOCK_STREAM,0)) ==-1)
    {
        PRT_INFO("socket error\n");
        return -1;
    }

    struct sockaddr_in server_addr;
  	memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);
    SetSocketOption(sfd);
    
    PRT_INFO("try connect server...\n");
    if(connect(sfd,(struct sockaddr *)&server_addr,sizeof(struct sockaddr)) == -1)
    {
        PRT_INFO("connect error\n");
        close(sfd);
        return -1;
    }
    PRT_INFO("connect server sucessful!\n");
    return sfd;
}

int Network::receiveFromServer(void)
{
    if(sockfd < 0)
    {
        keepSocketConnect();
    }
    else
    {
        fd_set rfds;
        FD_ZERO(&rfds);
        FD_SET(sockfd, &rfds);

        struct timeval tv;
        tv.tv_sec = 6;
        tv.tv_usec = 0;

        int ret = select(sockfd+1, &rfds,NULL, NULL, &tv);
        switch(ret)
        {
            case -1:
                PRT_INFO("select error!\n");
                break;
            case 0:
                PRT_INFO("recv time out!\n");
                disconnectSocket();
                break;
            default:
                if(FD_ISSET(sockfd,&rfds) > 0)
                {
                    FD_CLR(sockfd, &rfds);
                    memset0(recvData);
                    int len = recv(sockfd, (struct sentbuf*)&recvData, sizeof(recvData), 0);
                    if(len == -1 && errno != EINTR) {
					    PRT_INFO("signal isn't interrupt!\n");
					    return -1;
					}  
					else if(len == 0) {		
						PRT_INFO("can not recv server feedback!\n");
					    disconnectSocket();
						return -1;
		            }
		            
		           	if(recvData.flag == 1) 
		                PRT_INFO("recv face\n");
		            else 
		              	PRT_INFO("No face\n");
                }//end of if
                break;
        }//end of switch
        return 0;
   }//end of else
}

int Network::sendMatToServer(cv::Mat image)
{
    if(image.empty())	{
        PRT_INFO("image is empty!\n");
        return -1;
    }

    if(image.cols != IMG_WIDTH || image.rows != IMG_HEIGHT || image.type() != CV_8UC3)
    {
        PRT_INFO("The image WIDTH(%d), HEIGHT(%d), type == CV_8UC3\n",image.cols, image.rows);
        return -1;
    }

    if(sockfd < 0)
    {
        PRT_INFO("sockfd == -1\n");
        keepSocketConnect();
        sleep(1);//add delay 1s to connection
        return -1;
    }
    else
    {
        fd_set wfds;
        FD_ZERO(&wfds);
        FD_SET(sockfd, &wfds);

        struct timeval tv;
        tv.tv_sec=0;
        tv.tv_usec=0;

        int ret = select(sockfd+1, NULL, &wfds, NULL, &tv);
        switch(ret)
        {
            case -1:
                PRT_INFO("select error!\n");
                break;
            case 0:
                break;
            default:
                if( FD_ISSET(sockfd,&wfds) )
                {
                    FD_CLR(sockfd, &wfds);
                    for(int i=0; i < PACKAGE_NUM; i++)
                    {
                        int num1 = IMG_HEIGHT / PACKAGE_NUM * i;
                        for(int j = 0; j< IMG_HEIGHT / PACKAGE_NUM; j++)
                        {
                            int num2 = j * IMG_WIDTH * 3;
                            uchar* ucdata = image.ptr<uchar>(j + num1);
                            for(int k = 0; k< IMG_WIDTH * 3; k++)
                            {
                                sentData.buf[num2 + k] = ucdata[k];
                            }
                        }

                        if(i == PACKAGE_NUM -1)
                            sentData.flag = 2;
                        else
                            sentData.flag = 1;
						
						int len = send(sockfd, (char*)(&sentData),sizeof(sentData),0);
                    	if(len == -1 && (errno != EINTR || errno != EAGAIN ||
                    					 errno !=EWOULDBLOCK) ) {
                            PRT_INFO("send error,signal isn't interrupt!\n");
                            disconnectSocket();
                            return -1;
                        }               
                    }//end of for

                }//end of if
                break;
        }//end of switch
        
    }//end of else
    return 0;
}

void Network::disconnectSocket()
{
    PRT_INFO("\n");
    if(sockfd > 0)
    {
        close(sockfd);
        sockfd = -1;
    }
}

