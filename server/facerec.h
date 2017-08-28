#ifndef FACEREC_H
#define FACEREC_H

#include "global.h"
#include "database.h"
#include "network.h"
#include "TSingleIns.h"

class faceRecognize: public TSingleIns<faceRecognize>
{
public:
    explicit faceRecognize();
    ~faceRecognize();

	void InitFaceModel();
	void destoryFaceModel();

	mpINT  detectFaceNumFromFrame(Mat gallery_img);
	mpVOID startExtractFeatureFromframe(Mat gallery_img, int face_num);
	mpVOID selectNameByDatabase(Mat original,int num);
	mpVOID LoadCascadeClassifierl();
	
	mpBOOL detectClientImage(Mat& detImage);
	cv::Rect findBiggestFace(Mat matCapturedGrayImage, CascadeClassifier cascFace);    
	
private:
	seeta::FaceDetection *m_pFaceDet;
	seeta::FaceAlignment *m_pFaceAli;
	seeta::FaceIdentification *m_pFaceIde;
	
	CascadeClassifier cascFace;
	SFaceInfo facesInfo;
	
	float gallery_fea[2048];
    float probe_fea[2048];
};


#endif
