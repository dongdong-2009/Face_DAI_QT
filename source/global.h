#ifndef GLOBALHEAD
#define GLOBALHEAD

#ifdef __cplusplus
# define MP_BGN_C_DECLS    extern "C" {
# define MP_END_C_DECLS    }
#else
# define MP_BGN_C_DECLS
# define MP_END_C_DECLS
#endif

//MP_BGN_C_DECLS
//time
#include <time.h>
#include <sys/time.h>

//other header
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <pthread.h>
#include <ctime>

//add opencv head
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/opencv.hpp>
#include <opencv2/opencv_modules.hpp>

#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"

//add seetaface header
#include "include/face_identification.h"
#include "include/recognizer.h"
#include "include/face_detection.h"
#include "include/face_alignment.h"
#include "include/math_functions.h"

typedef void                    mpVOID;
typedef bool                    mpBOOL;
typedef char                    mpCHAR;
typedef unsigned char           mpBYTE;
typedef int                     mpINT;
typedef unsigned int            mpUINT;
typedef long                    mpLONG;
typedef unsigned long           mpULONG;
typedef unsigned short          mpWORD;
typedef unsigned int            mpDWORD;
typedef unsigned long long      mpQWORD;
typedef float                   mpFLOAT;
typedef double                  mpDOUBLE;

using namespace std;
using namespace cv;
using namespace seeta;

#define MP_MALLOC(s)            MP_Malloc(s)
#define MP_FREE(p)              MP_Free(p)

#define mpTRUE                  1
#define mpFALSE                 0
#define mpNULL                  0

#define QMESSAGEBOX_WARNING         "Warning"
#define QFILEDIALOG_SAVEIMAGE       "Save Image"
#define QFILEDIALOG_OPENIMAGE       "Open Image"
#define QFILEDIALOG_SUPPORTTYPE     "Image File(*.jpg *png)"
#define QFILEDIALOG_CURRENTPATH     "."
#define FACE_MODES_DIR   "/models/"
#define FACE_CASCADE_DIR   "./cascade/"

#define FACE_SET_PREDICTION_VALUE   0.7
#define FACE_DELAY_DISPLAY_INTERVAL 50
#define TM_THRESHOLD	0.4
#define MIN_FACE_DETECT_NUMBER  1
#define NO_FACE_DETECT_NUMBER  0

#define FACE_ALIVE_DETECT_MAX_CONTOURS  20
#define FACE_ALIVE_DETECT_MIN_CONTOURS  0
#define FACE_LEFT_EYE_POINT     1
#define FACE_RIGHT_EYE_POINT    0

#define TPL_WIDTH 		16
#define TPL_HEIGHT 		12
#define WIN_WIDTH		TPL_WIDTH * 2
#define WIN_HEIGHT		TPL_HEIGHT * 2
#define BLINK_EYE_DELAY_SET     8000

#define ALIVE_CHECK_HEAD_X_POINT    200
#define ALIVE_CHECK_HEAD_Y_POINT    50
#define ALIVE_CHECK_HEAD_HIGHT_POINT    200
#define ALIVE_CHECK_HEAD_WIDTH_POINT    200
#define ALIVE_CHECK_MAX_X_POINT 400
#define ALIVE_CHECK_MAX_Y_POINT 250

#define ALIVE_DIFFER_X_LENGTH   5
#define ALIVE_DIFFER_Y_LENGTH   5

#define CALIBRATION_DEFAULT_VALUE   100
#define EYE_PERCENT_TOP_PARAMS    25
#define EYE_PERCENT_SIDE_PARAMS   13
#define EYE_PERCENT_HEIGHT_PARAMS 30
#define EYE_PERCENT_WIDTH_PARAMS  35

#define PRT_INFO(fmt, ...) MP_Log(__FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)

template <typename T>
///<summary>From (for example double) value makes string with just whole numbers</summary>
std::string to_string_with_precision(const T a_value, const int n = 6)
{
    std::ostringstream out;
    out << std::setprecision(n) << a_value;
    return out.str();
}

typedef struct
{
    std::vector<seeta::FaceInfo> faces;
    seeta::FacialLandmark points[5];
} SFaceInfo;

typedef struct _tagLogTime
{
    struct tm logSec;
    mpINT logMSec;
} LogTime;

typedef struct
{
   string face_name;
   float prediction;

   void init()
   {
       face_name.clear();
       prediction  = 0;
   }
} SFaceData;

typedef struct
{
    clock_t eyeCloseTime;
    bool eyeOpen;
    int blinkNumber;

    void init()
    {
        eyeOpen = true;
        blinkNumber = 0;
    }
}SEyeTrackInfo;

/**
 * @ingroup SUB_GROUP_LOG
 * @brief
 * @param   pszFile in which file
 * @param   iLine   in which line
 * @param   pFunction   in which function
 * @parma   format  output format
 */
mpVOID MP_Log(const mpCHAR * pszFile, mpINT iLine, const mpCHAR * pFunction,const mpCHAR * format, ...);


//MP_END_C_DECLS

#endif // GLOBALHEAD

