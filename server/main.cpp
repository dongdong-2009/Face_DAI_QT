#include "global.h"
#include "network.h"
#include "facerec.h"

int main(int argc, char* argv[])
{  
   faceRecognize::GetInstance();
   server_network::GetInstance();
   for(;;) 
   {
  		server_network::GetInstance()->handleEpollEvent(); 
  		//server_network::GetInstance()->TEST();
  		usleep(50*USLEEP_PARAMS_MS);//50ms
    }
    server_network::ReleaseInstance(); 
    faceRecognize::ReleaseInstance();
	return 0;
}

