#include "facedetect.h"

facedetect::facedetect()
{
    InitFunction();
}
 
facedetect::~facedetect()
{
 
}
  
mpVOID facedetect::InitFunction()
{
    cout<<"Load CascadeClassifierl..."<<endl;
    if(access( FACE_CASCADE_DIR, R_OK) == 0)
    {
     	string cascadeName = FACE_CASCADE_DIR;
     	cascadeName += "haarcascade_frontalface_alt.xml";
     	
		if( !cascFace.load( cascadeName ) )
		{
		    cerr << "ERROR: Could not load classifier cascade" << endl;
		    return;
		}
    }
    else
    {
        cout <<"please check your resource floder!"<<endl;
        return;
    }
}

mpBOOL facedetect::FaceDetection(Mat grayImage)
{
  	if(grayImage.empty()) return mpFALSE;
    
    return  FindFaceWithImage(grayImage, cascFace);               
}

mpBOOL facedetect::FindFaceWithImage(Mat grayImage, CascadeClassifier cascFace)
{
    mpBOOL bRet = mpFALSE;
    if(cascFace.empty())
    {
        cout<< "Is your cascade classifier successfully loaded?" <<endl;
        exit(0);
    }

    vector<cv::Rect> faces;
    cascFace.detectMultiScale(grayImage, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE | CV_HAAR_FIND_BIGGEST_OBJECT, Size(150, 150), Size(300, 300));

    if (faces.size() > 0) 
    {
        bRet = mpTRUE;
    }
	return bRet;
}

