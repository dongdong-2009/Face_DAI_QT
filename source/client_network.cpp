#include "client_network.h"

client_network::client_network(void)
{
    initSocket();
}

client_network::~client_network()
{
    disconnectSocket();
}

int client_network::initSocket(void)
{
    sockfd  = -1;//socket init
    cout<<"init client Socket!"<<endl;
}

void client_network::setSocketOption(int sfd)
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

void client_network::keepSocketConnect(void)
{
    sockfd = connectServer(SERVER_IP,SERVER_PORT);
    if(sockfd < 0)
    {
        PRT_INFO("can't connect server host!\n");
    }
    else
    {
        PRT_INFO("alreay connect server host!\n");
    }
}

int client_network::connectServer(char* ip, int port)
{
     PRT_INFO("try connect server...\n");

     int sfd = -1;
     signal(SIGPIPE,SIG_IGN);

     if((sfd = socket(AF_INET,SOCK_STREAM,0)) ==-1)
     {
         PRT_INFO("socket error\n");
         close(sfd);
         sfd = -1;
     }

     struct sockaddr_in server_addr;
     bzero(&server_addr,sizeof(server_addr));
     server_addr.sin_family = AF_INET;
     server_addr.sin_port = htons(port);
     server_addr.sin_addr.s_addr = inet_addr(ip);

     setSocketOption(sfd);
     if(connect(sfd,(struct sockaddr *)&server_addr,sizeof(struct sockaddr)) == -1)
     {
         PRT_INFO("connect error\n");
         close(sfd);
         sfd = -1;
     }

     return sfd;
}

int client_network::receiveResultFromServer(cv::Mat& image)
{
    int count = 0;
    cv::Mat img(IMG_HEIGHT,IMG_WIDTH,CV_8UC3,cv::Scalar(0));

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
        tv.tv_sec = 2;
        tv.tv_usec = 0;

        //cv::namedWindow("img", CV_WINDOW_NORMAL);
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

                    memset(&recvData, 0, sizeof(recvData));
                    int needRecv = sizeof(recvData);

                    for(int i = 0; i< PACKAGE_NUM;i++)
                    {
                        int pos = 0;
                        int len0 =0;
                        while(pos < needRecv)
                        {
                            len0 = recv(sockfd, (char*)(&recvData.buf) + pos, needRecv - pos, 0);
                            if(len0 == -1 && errno != EINTR)
                            {
                                PRT_INFO("recv error,signal isn't interrupt!\n");
                                disconnectSocket();
                            }
                            pos += len0;
                        }

                        count = count + recvData.flag;

                        int num1 = IMG_HEIGHT / PACKAGE_NUM *i;
                        for(int j = 0; j< IMG_HEIGHT / PACKAGE_NUM; j++)
                        {
                            int num2 = j * IMG_WIDTH * 3;
                            uchar* ucdata = img.ptr<uchar>(j+num1);
                            for(int k =0; k< IMG_WIDTH; k++)
                            {
                                ucdata[k] = recvData.buf[num2 + k];
                            }
                        }

                        if(recvData.flag == 2)
                        {
                            if(count == 33)
                            {
                                image = img;
                               // cv::imshow("img", img );
                                return 1;
                            }
                        }
                    }

                }//end of if
                break;
           }

        return 0;
    }//end of else
}

int client_network::sendMatDataToServer(cv::Mat image)
{
    if(image.empty()){
        PRT_INFO("image is empty!\n");
        return -1;
    }
    //  cv::imshow( "img", image );
/*
    if(image.cols != IMG_HEIGHT || image.rows != IMG_WIDTH || image.type() != CV_8UC3)
    {
        PRT_INFO("image attribute error!\n");
        return -1;
    }
*/
    if(sockfd < 0)
    {
        keepSocketConnect();
        PRT_INFO("sockfd == -1\n");
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
                ///PRT_INFO("send time out!\n");//ignore the here
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

                        if(i == 31)
                            sentData.flag = 2;
                        else
                            sentData.flag = 1;

                        if(send(sockfd, (char*)(&sentData),sizeof(sentData),0) < 0)
                        {
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


void client_network::disconnectSocket()
{
    PRT_INFO("\n");
    if(sockfd)
    {
        close(sockfd);
        sockfd = -1;
    }
}
