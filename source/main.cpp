#include "facerecognize.h"
#include <QApplication>
#include <QCoreApplication>
#include <QTextCodec>
#include <QDebug>
int main(int argc, char *argv[])
{
    QApplication::addLibraryPath("./plugins");
    QApplication a(argc, argv);
    QTextCodec *pcodec=QTextCodec::codecForName("gb2312");
    QTextCodec::setCodecForCStrings(pcodec);
    QTextCodec::setCodecForLocale(pcodec);
    QTextCodec::setCodecForTr(pcodec);
    faceRecognize w;
    w.setWindowTitle("faceRecognizer");
    w.show();

    return a.exec();
}
