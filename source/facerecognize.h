#ifndef FACERECOGNIZE_H
#define FACERECOGNIZE_H

//add QT head
#include <QWidget>
#include <QTimer>
#include <QImage>
#include <QInputDialog>
#include <QDebug>
#include <QDir>
#include <QMessageBox>
#include <QFileDialog>
#include <QPalette>

#include "face_database_api.h"
#include "client_network.h"

namespace Ui {
class faceRecognize;
}

class faceRecognize : public QWidget
{
    Q_OBJECT

public:
    explicit faceRecognize(QWidget *parent = 0);
    ~faceRecognize();
    CvHaarClassifierCascade* load_object_detector( const char* cascade_path );
    void loadCascadeClassifier();
    mpVOID InitFaceModel(mpVOID);
    mpVOID destoryFaceModel(mpVOID);

    mpVOID showImageFromUsers(Mat& frame);
    mpVOID getCurrentPath(mpVOID);
    mpBOOL inputLabelFromUsr(mpVOID);

    mpVOID extractFaceFeatureWithImage(Mat gallery_img, float* face_fea);
    mpBOOL selectNameByDatabase(float* probe_fea);
    mpINT  detectFaceNumWithFrame(Mat gallery_img);
    mpBOOL isFaceInRectArea( cv::Point curcenter);
    void showImageWithUI(Mat original);

    void headTracking( Mat matCapturedImage, CascadeClassifier cascFace, cv::Rect &detectedFaceRegion);
    void calcFlow(const Mat& flow, Mat& cflowmap, int step, int &globalMovementX, int &globalMovementY);
    cv::Rect findBiggestFace(Mat matCapturedGrayImage, CascadeClassifier cascFace);
    void eyeTracking(Mat &matCurrentEye, Mat &matPreviousEye);
    void getEyesFromFace(Mat &matFace, Mat &matLeftEye, Mat &matRightEye);
    void detectBlink(Mat &matEyePrevious, Mat &matEyeCurrent, String eye, bool &eyeOpen, int &blinkNumber, clock_t &closeTime);
    void calcFlowEyes(const Mat& flow, Mat& cflowmap, int step, int &movementX, int &movementY);
    bool getMatchFaces(Mat matCapturedGrayImage, Mat matCapturedImage,cv::Rect face,cv::Rect &detectedFaceRegion);

private slots:
    void on_OpenCamera_clicked();
    void on_CaptureImage_clicked();
    void readFrameFromCamera();
    void on_SaveImage_clicked();
    void on_OpenImage_clicked();

private:
    Ui::faceRecognize *ui;
    QTimer *m_pTimer;

    seeta::FaceDetection *m_pFaceDet;
    seeta::FaceAlignment *m_pFaceAli;
    seeta::FaceIdentification *m_pFaceIde;
    face_database_api *m_pFaceDBA;
    client_network *m_netWork;

    Mat gallery_img_color;
    SFaceData faceData;
    SFaceInfo facesInfo;

    int deviceId;
    enum camerastatus { Nothing,Opened,Closed,Captured,Max} eCameraStatus;
    enum aliveStatus { Invalid,Starting,Finshed} eAliveStatus;

    VideoCapture cap;
    cv::Mat frame;
    cv::Mat capframe;

    QString inputName;
    string scurrentpath;

    mpBOOL m_bAliveCheckStart;
    clock_t closeTime;
    cv::Point center;

    CascadeClassifier cascFace;
    cv::Rect detectedFaceRegion;

    cv::Mat previousFace;
    cv::Mat currentFace;

    SEyeTrackInfo leftEyeTrackInfo;
    SEyeTrackInfo rightEyeTrackInfo;

    int calibrationFace;
};

#endif // FACERECOGNIZE_H
