#ifndef FACEDETECT_H
#define FACEDETECT_H

#include "global.h"
#include "TSingleIns.h"

class facedetect:public TSingleIns<facedetect>
{
public:
	explicit facedetect();
    ~facedetect();
	mpVOID InitFunction();
	mpBOOL FaceDetection(Mat grayImage);
	mpBOOL FindFaceWithImage(Mat grayImage, CascadeClassifier cascFace);
	
private:
   CascadeClassifier cascFace;
};
 
#endif // FACEDETECTHEAD
