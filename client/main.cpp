#include "facedetect.h"
#include "network.h"

int main(int argc, char* argv[])
{
	cout<<"Init Video Capture..."<<endl;
	VideoCapture capture(0);
	if( !capture.isOpened() )  
	{
		cout << "Capture from camera 0 didn't work." << endl;
		return -1;
	}
	cout << "Video capturing has been started ..." << endl;
	
	cv::Mat frame(IMG_HEIGHT,IMG_WIDTH,CV_8UC3,cv::Scalar(0));
	Network* mp_network = Network::GetInstance();
	for(;;)
	{
		capture >> frame;
		if( frame.empty() )
			break;
		
		if(-1 != mp_network->sendMatToServer(frame) ) {
			mp_network->receiveFromServer();
		}
		
		usleep(50*USLEEP_PARAMS_MS);//100ms
	}
	
	Network::ReleaseInstance(); 
	return 0;
}
