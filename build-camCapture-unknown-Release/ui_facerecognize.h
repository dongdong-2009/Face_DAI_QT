/********************************************************************************
** Form generated from reading UI file 'facerecognize.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FACERECOGNIZE_H
#define UI_FACERECOGNIZE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_faceRecognize
{
public:
    QGroupBox *groupBox;
    QLabel *label_DisplayCam;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_ShowResult;
    QGroupBox *groupBox_4;
    QVBoxLayout *verticalLayout_4;
    QLabel *ShowSnap;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_3;
    QPushButton *OpenImage;
    QPushButton *OpenCamera;
    QPushButton *CaptureImage;

    void setupUi(QWidget *faceRecognize)
    {
        if (faceRecognize->objectName().isEmpty())
            faceRecognize->setObjectName(QString::fromUtf8("faceRecognize"));
        faceRecognize->resize(1040, 649);
        groupBox = new QGroupBox(faceRecognize);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(0, 10, 810, 640));
        label_DisplayCam = new QLabel(groupBox);
        label_DisplayCam->setObjectName(QString::fromUtf8("label_DisplayCam"));
        label_DisplayCam->setGeometry(QRect(5, 10, 800, 600));
        groupBox_3 = new QGroupBox(faceRecognize);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(830, 20, 201, 80));
        verticalLayout_3 = new QVBoxLayout(groupBox_3);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        label_ShowResult = new QLabel(groupBox_3);
        label_ShowResult->setObjectName(QString::fromUtf8("label_ShowResult"));
        label_ShowResult->setEnabled(true);

        verticalLayout_3->addWidget(label_ShowResult);

        groupBox_4 = new QGroupBox(faceRecognize);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        groupBox_4->setGeometry(QRect(830, 120, 211, 196));
        verticalLayout_4 = new QVBoxLayout(groupBox_4);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        ShowSnap = new QLabel(groupBox_4);
        ShowSnap->setObjectName(QString::fromUtf8("ShowSnap"));

        verticalLayout_4->addWidget(ShowSnap);

        groupBox_2 = new QGroupBox(faceRecognize);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(820, 440, 210, 161));
        gridLayout_3 = new QGridLayout(groupBox_2);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        OpenImage = new QPushButton(groupBox_2);
        OpenImage->setObjectName(QString::fromUtf8("OpenImage"));

        gridLayout_3->addWidget(OpenImage, 2, 0, 1, 1);

        OpenCamera = new QPushButton(groupBox_2);
        OpenCamera->setObjectName(QString::fromUtf8("OpenCamera"));

        gridLayout_3->addWidget(OpenCamera, 0, 0, 1, 1);

        CaptureImage = new QPushButton(groupBox_2);
        CaptureImage->setObjectName(QString::fromUtf8("CaptureImage"));

        gridLayout_3->addWidget(CaptureImage, 1, 0, 1, 1);


        retranslateUi(faceRecognize);

        QMetaObject::connectSlotsByName(faceRecognize);
    } // setupUi

    void retranslateUi(QWidget *faceRecognize)
    {
        faceRecognize->setWindowTitle(QApplication::translate("faceRecognize", "faceRecognize", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QString());
        label_DisplayCam->setText(QApplication::translate("faceRecognize", "<html><head/><body><p align=\"center\"><br/></p></body></html>", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("faceRecognize", "Recognize Results", 0, QApplication::UnicodeUTF8));
        label_ShowResult->setText(QApplication::translate("faceRecognize", "<html><head/><body><p align=\"center\"><br/></p></body></html>", 0, QApplication::UnicodeUTF8));
        groupBox_4->setTitle(QApplication::translate("faceRecognize", "Capture Screen", 0, QApplication::UnicodeUTF8));
        ShowSnap->setText(QApplication::translate("faceRecognize", "<html><head/><body><p align=\"center\"><br/></p></body></html>", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("faceRecognize", "Operation", 0, QApplication::UnicodeUTF8));
        OpenImage->setText(QApplication::translate("faceRecognize", "Select Image", 0, QApplication::UnicodeUTF8));
        OpenCamera->setText(QApplication::translate("faceRecognize", "Open Camera", 0, QApplication::UnicodeUTF8));
        CaptureImage->setText(QApplication::translate("faceRecognize", "Capture Image", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class faceRecognize: public Ui_faceRecognize {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FACERECOGNIZE_H
