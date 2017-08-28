#ifndef GLOBAL_H
#define GLOBAL_H

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
#include <fcntl.h>

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

#define MP_MALLOC(s)            MP_Malloc(s)
#define MP_FREE(p)              MP_Free(p)
#define MP_NEW  				new
#define MP_DELETE       		delete

#define mpTRUE                  1
#define mpFALSE                 0
#define mpNULL                  0

#define FACE_MODES_DIR   		"/models/"
#define FACE_CASCADE_DIR   		"./cascade/"

#define MAX_NAME_TEXT_LENTH 30
#define USLEEP_PARAMS_MS  1000
#define FACE_SET_PREDICTION_VALUE   0.7

#define PRT_INFO(fmt, ...) MP_Log(__FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)

/* clear an object (array, structure, etc.) */
#define memset0(object) do {                                            \
    if(sizeof(object) == sizeof(void*)) {                               \
      printf("memset0: object may be a pointer instead of an struct/array\n"); \
    }                                                                   \
    memset(&(object), 0, sizeof(object));                               \
  } while(0)
  
using namespace std;
using namespace cv;
using namespace seeta;

typedef struct
{
    std::vector<seeta::FaceInfo> faces;
    seeta::FacialLandmark points[5];
}SFaceInfo;

typedef struct
{
    seeta::ImageData img_data_color;
    seeta::ImageData img_data_gray;
}SDetectData;

typedef struct _tagLogTime
{
    struct tm logSec;
    mpINT logMSec;
} LogTime;

typedef struct 
{
	int fd;
	int errCnt;
}SCliContext;


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


