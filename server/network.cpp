#include "network.h"

server_network::server_network(void)
{
    initSocket();//list client socket
    fdsinfo.clear();
}

server_network::~server_network()  {

}

void server_network::setnonblocking(int fd)
{
	int flag = fcntl(fd, F_GETFL);
	if( flag == -1) {
		 PRT_INFO("error getfl!\n");
		 return ;
	}
	flag|= O_NONBLOCK;
	if(fcntl(fd, F_SETFL, flag) < 0) {
	 	PRT_INFO("error setfl!\n");
		return ;
	}
}

void server_network::addfd(int epollfd, int fd, bool oneshot) 
{
    epoll_event event;
	event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET;
    if (oneshot) {
          event.events |= EPOLLONESHOT;
    }
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event) == -1) {
	   closeSocket(epollfd);
       PRT_INFO("epoll_ctl: listenfd\n");
       return ;
    }
    setnonblocking(fd);
}

int server_network::initSocket(void)
{
    // Set up listening socket, 'listenfd' (socket(), bind(), listen()) 
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == listenfd)
    {
        PRT_INFO("socket error\n");
        return -1;
    }
    
    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(SERVER_PORT);
    address.sin_family = AF_INET;
	
	int i = 1;
	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &i, sizeof(i));
	
    if(-1 == bind(listenfd,(struct sockaddr*) &address, sizeof(address)))
    {
        PRT_INFO("bind error\n");
        return -1;
    }
    
    if(-1 == listen(listenfd, 5))
    {
        PRT_INFO("listen error\n");
        return -1;
    }
    PRT_INFO("server fd %d listen client fd...\n",listenfd);
    
    if ( (epollfd = epoll_create(MAX_EVENT_NUMBER) ) == -1) 
	{
        PRT_INFO("epoll_create\n");
        return -1;
    }
    addfd(epollfd, listenfd, false);
}

void server_network::TEST()
{
	if(fdsinfo.size() < 1)
	{
		return;//protect
	}
	
	vector<SCliContext>::iterator itor;
	for(itor = fdsinfo.begin(); itor != fdsinfo.end(); itor++)
	{
		if( (*itor).fd < 1) {
			continue;
		}
		
		int fd = (*itor).fd;
		Mat cImage;
	    int iRet = receiveFromClient(fd,cImage);
		if(iRet == -1) 
		{
			if( (*itor).errCnt < 3) {
				(*itor).errCnt ++;
			 	PRT_INFO("recv error fd =%d\n",fd);
				break;
			}
			
			char showName[MAX_NAME_TEXT_LENTH] = {0};
			sprintf(showName,"client%d_image",fd);
			cv::destroyWindow(showName);
	
			if (epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, NULL) == -1) {
			    PRT_INFO("epoll_ctl: delete\n");
			}
			close(fd);//disconnect client socket
			PRT_INFO("fd:%d\n",(*itor).fd);
			itor = fdsinfo.erase(itor);	
			itor --;
			continue;
		}
		
		if( faceRecognize::GetInstance()->detectClientImage(cImage) ) {
			sendResultToClient(fd, 1);
		} 
		else {
			sendResultToClient(fd, 0);		
		}
		
		if( !cImage.empty() ) {
			char showName[MAX_NAME_TEXT_LENTH] = {0};
			sprintf(showName,"client%d_image",*itor);
			imshow(showName, cImage);
			waitKey(10);
		}
	}
}

void server_network::handleEpollEvent()
{	
	int nfd = epoll_wait(epollfd, events, MAX_EVENT_NUMBER, -1);
    if ( nfd == 0 ) {
        PRT_INFO("epoll_pwait\n");
        return;
    }    
    PRT_INFO("nfd:%d\n", nfd);
    
    for (int i = 0; i < nfd; i++) 
	{
        if (events[i].data.fd == listenfd) 
		{
			socklen_t addrlen = sizeof(struct sockaddr_in);	
			clientfd = accept(listenfd, (struct sockaddr*)&local_addr, &addrlen);
			if( clientfd == -1 )
			{
				 PRT_INFO("accept\n");
                 continue;
			}		
			PRT_INFO("get client connection from %s fd = %d\n",
			(char*) inet_ntoa(local_addr.sin_addr), clientfd);
			
            addfd(epollfd, clientfd, false);
            
            SCliContext tmpContext;
            memset0(tmpContext);
            tmpContext.fd = clientfd;
 			fdsinfo.push_back(tmpContext);
        }
	    else if(events[i].events & EPOLLIN)
		{	
		    PRT_INFO("EPOLLIN\n");
			Mat cImage;
			int iRet = receiveFromClient(events[i].data.fd, cImage);
			if(iRet == -1)
			{
				char showName[MAX_NAME_TEXT_LENTH] = {0};
				sprintf(showName,"client%d_image",events[i].data.fd);
				cv::destroyWindow(showName);
					
				if (epoll_ctl(epollfd, EPOLL_CTL_DEL, events[i].data.fd, NULL) == -1) 
				{
				    PRT_INFO("epoll_ctl: delete\n");
				}
				close(events[i].data.fd);
			}

			mpBOOL bRet = faceRecognize::GetInstance()->detectClientImage(cImage);
			if(bRet) {
				sendResultToClient(events[i].data.fd, 1);
			} 
			else {
				sendResultToClient(events[i].data.fd, 0);		
			}
			if( !cImage.empty() ) {
				char showName[MAX_NAME_TEXT_LENTH] = {0};
				sprintf(showName,"client%d_image",events[i].data.fd);
				imshow(showName, cImage);
		    	cv::waitKey(10);
			}
        } 
        else {
       	    PRT_INFO("something else happened\n");
        }
     }//end of for cond
            
}

//receive original image
int server_network::receiveFromClient(int sockfd,cv::Mat& image)
{
  	cv::Mat img(IMG_HEIGHT,IMG_WIDTH,CV_8UC3,cv::Scalar(0));
  	
	mpINT iRet =-1;
    if(sockfd < 0)
    {
        PRT_INFO("sockfd < 0\n");
        return iRet;
    }
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(sockfd, &rfds);

    struct timeval tv;
    tv.tv_sec = 3;
    tv.tv_usec = 0;

    int ret = select(sockfd+1, &rfds,NULL, NULL, &tv);
    switch(ret)
    {
        case -1:
            PRT_INFO("select error!\n");
            break;
        case 0:
            PRT_INFO("recv time out!\n");
            iRet = -1;
            break;
        default:
            if(FD_ISSET(sockfd,&rfds) > 0)
            {
                FD_CLR(sockfd, &rfds);
                iRet = 0;
                
				memset(&recvData, 0, sizeof(recvData));
				for(int i = 0; i < PACKAGE_NUM; i++)
				{
					int pos = 0;
					int needRecv = sizeof(recvData);
					while(pos < needRecv)
					{
						int len = recv(sockfd, (char*)(&recvData.buf) + pos, needRecv - pos, 0);
						if(len == -1 && errno != EINTR) {
							PRT_INFO("signal isn't interrupt!\n");
							return -1;
						} 
						else {
							pos += len;
						}
					}
						
					int count = count + recvData.flag;
					int num1 = IMG_HEIGHT / PACKAGE_NUM *i;
					for(int j = 0; j< IMG_HEIGHT / PACKAGE_NUM; j++)
					{
						int num2 = j * IMG_WIDTH * 3;
						uchar* ucdata = img.ptr<uchar>(j+num1);
						for(int k =0; k < IMG_WIDTH * 3; k++)
						{
							ucdata[k] = recvData.buf[num2 + k];
						}
					}

					if(recvData.flag == 2)
					{
						if(count == PACKAGE_NUM +1)
						{
							image = img;
							iRet = 0;
							break;
						}
					}	
						
				}
			}
			break;
	}//end of switch
	return iRet;
}
 
int server_network::sendResultToClient(int sockfd, int result)
{
 	PRT_INFO("\n");
    if(sockfd < 0)
    {
        PRT_INFO("sockfd == -1\n");
        return -1;
    }

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
				sentData.flag = result;
                if(send(sockfd, (char*)(&sentData),sizeof(sentData),0) < 0)
                 {
                     PRT_INFO("send error,signal isn't interrupt!\n");
                     return -1;
                 }
            }//end of if
            break;
    }//end of switch
    return 0;
}

void server_network::closeSocket(int socket)
{
 	if(socket > 0) 
 	{
 		close(socket);
 		socket = -1;
	}
 }

