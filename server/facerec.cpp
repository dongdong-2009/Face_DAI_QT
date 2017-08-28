#include "facerec.h"
#include "database.h"

std::string MODEL_DIR = "./models/";

faceRecognize::faceRecognize()
{
    InitFaceModel();
    LoadCascadeClassifierl();
    face_database_api::GetInstance();
}

faceRecognize::~faceRecognize()
{
    destoryFaceModel();
    face_database_api::ReleaseInstance();
    cout<<"exit face recognizer!"<<endl;
}
   
void faceRecognize::InitFaceModel()
{
    cout<<"Initialize Face Detection Model!"<<endl;
    if(access(MODEL_DIR.c_str(), R_OK) == 0)
    {
        cout<<"starting Load face detection libs..."<<endl;
        // Initialize face Detection model
        m_pFaceDet = new FaceDetection((MODEL_DIR + "face_fd_frontal_v1.0.bin").c_str());
        // Initialize face alignment model
        m_pFaceAli = new FaceAlignment((MODEL_DIR + "face_fa_v1.1.bin").c_str());
        // Initialize face Identification model
        m_pFaceIde = new FaceIdentification((MODEL_DIR + "face_fr_v1.0.bin").c_str());
        m_pFaceDet->SetMinFaceSize(80);
        m_pFaceDet->SetScoreThresh(2.f);
        m_pFaceDet->SetImagePyramidScaleFactor(0.8f);
        m_pFaceDet->SetWindowStep(4, 4);
    }
    else
    {
        cout <<"please check your resource floder,make sure the seeta bin!"<<endl;
        return;
    }
    
    cout<<"Load face detection libs finshed!"<<endl;
}

mpVOID faceRecognize::LoadCascadeClassifierl()
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

void faceRecognize::destoryFaceModel()
{
    //delete seeta pointer
    if(mpNULL != m_pFaceDet)
    {
        delete m_pFaceDet;
        m_pFaceDet = mpNULL;
    }

    if(mpNULL != m_pFaceAli)
    {
        delete m_pFaceAli;
        m_pFaceAli = mpNULL;
    }

    if(mpNULL != m_pFaceIde)
    {
        delete m_pFaceIde;
        m_pFaceIde = mpNULL;
    }
}

mpINT faceRecognize::detectFaceNumFromFrame(Mat gallery_img)
{
    if(gallery_img.empty()) return 0;
    cv::Mat gallery_img_gray;
    cv::cvtColor(gallery_img, gallery_img_gray, CV_BGR2GRAY);

    //gallery image
    seeta::ImageData img_data_color(gallery_img.cols, gallery_img.rows, gallery_img.channels());
    img_data_color.data = gallery_img.data;

    seeta::ImageData img_data_gray(gallery_img_gray.cols, gallery_img_gray.rows, gallery_img_gray.channels());
    img_data_gray.data = gallery_img_gray.data;
    img_data_gray.width = gallery_img_gray.cols;
    img_data_gray.height = gallery_img_gray.rows;
    img_data_gray.num_channels = 1;

    std::vector<seeta::FaceInfo> faces = m_pFaceDet->Detect(img_data_gray);  // Detect faces
    facesInfo.faces.clear();
    facesInfo.faces = faces;//get the face point to drawing
    int32_t num_face = static_cast<int32_t>(faces.size());
    if (num_face == 0) {
        return 0;
    } else {
        return num_face;
    }
}

mpVOID faceRecognize::startExtractFeatureFromframe(Mat gallery_img, int face_num)
{
  		if(gallery_img.empty()) return ;
        cv::Mat gallery_img_gray;
        cv::cvtColor(gallery_img, gallery_img_gray, CV_BGR2GRAY);

        //gallery image
        seeta::ImageData img_data_color(gallery_img.cols, gallery_img.rows, gallery_img.channels());
        img_data_color.data = gallery_img.data;

        seeta::ImageData img_data_gray(gallery_img_gray.cols, gallery_img_gray.rows, gallery_img_gray.channels());
        img_data_gray.data = gallery_img_gray.data;
        img_data_gray.width = gallery_img_gray.cols;
        img_data_gray.height = gallery_img_gray.rows;

        //Detect 5 facial landmarks
        //seeta::FacialLandmark points[5];
        m_pFaceAli->PointDetectLandmarks(img_data_gray, facesInfo.faces[face_num], facesInfo.points);
        /*for (int i = 0; i<5; i++)
        {
            cv::circle(gallery_img, cv::Point(points[i].x, points[i].y), 2,
            CV_RGB(0, 255, 0));
        }*/
        //Extract face identity feature
        m_pFaceIde->ExtractFeatureWithCrop(img_data_color, facesInfo.points, probe_fea);

        //Drawing the face rect
        cv::Rect face_rect;
        face_rect.x = facesInfo.faces[face_num].bbox.x;
        face_rect.y = facesInfo.faces[face_num].bbox.y;
        face_rect.width = facesInfo.faces[face_num].bbox.width;
        face_rect.height = facesInfo.faces[face_num].bbox.height;
        cv::rectangle(gallery_img, face_rect, CV_RGB(255, 0, 0), 4, 8, 0);
}


mpVOID faceRecognize::selectNameByDatabase(Mat original,int face_num)
{
    float prediction = 0.0;
    face_database_feature_info_t face_info;
	face_database_api * m_pFaceDBA = face_database_api::GetInstance();
    m_pFaceDBA->face_database_inquire_prepare();
    while(SQLITE_ROW ==  m_pFaceDBA->face_database_inquire_step())
    {
        m_pFaceDBA->face_database_inquire_info(&face_info);
        prediction = m_pFaceIde->CalcSimilarity(face_info.key_data, probe_fea);//compare
        if (prediction >= FACE_SET_PREDICTION_VALUE)
        {
            //putTextContent(original, face_num, face_info.key_name, prediction);
            break;
        }
    }//end of while
    m_pFaceDBA->face_database_inquire_shutdown();
}

///<summary>From grayscale image captured on camera will find one biggest face. If there is no face, returnValue will be empty.</summary>
cv::Rect faceRecognize::findBiggestFace(Mat grayImage, CascadeClassifier cascFace)
{
    cv::Rect returnValue;
    if(cascFace.empty())
    {
        cout<< "Is your cascade classifier successfully loaded?" <<endl;
        exit(-1);
    }
	
    vector<cv::Rect> faces;
    cascFace.detectMultiScale(grayImage, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE | CV_HAAR_FIND_BIGGEST_OBJECT, Size(150, 150), Size(300, 300));

    if (faces.size() > 0) {
        return faces[0];
    }
    else {
        return returnValue;
    }
}

///<summary> On input there are client images </summary>
mpBOOL faceRecognize::detectClientImage(Mat& detImage)
{
	if(detImage.empty()) 
	{
		cout<<"image can not be NULL!"<<endl;
		return mpFALSE;
	}
	
    cv::flip(detImage, detImage, 1);//flip image
    Mat grayImage;
    cvtColor(detImage, grayImage, CV_BGR2GRAY);
    cv::Rect face = findBiggestFace(grayImage, cascFace);
    if (face.width == 0 && face.height == 0) {
        return mpFALSE;
    }
    rectangle(detImage, face, 1234);
    return mpTRUE;
}

