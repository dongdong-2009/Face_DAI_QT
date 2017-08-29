#include "global.h"


mpVOID * MP_Malloc(size_t s)
{
    return malloc(s);
}

mpVOID MP_Free(mpVOID * p)
{
    free(p);
}

mpVOID MUGetLocalTime(LogTime * pLogTime)
{
    if (pLogTime == mpNULL) return;

    struct timeval tv;
    struct tm localTime;

    memset(&tv, 0x00, sizeof(tv));
    memset(&localTime, 0x00, sizeof(localTime));

    gettimeofday(&tv, mpNULL);
    localtime_r(&tv.tv_sec, &localTime);

    memcpy(&(pLogTime->logSec), &localTime, sizeof(localTime));
    pLogTime->logMSec = tv.tv_usec;
}

// hh:mm:ss.xxx flie.cpp <line>:main() format
mpVOID MP_Log(const mpCHAR * pszFile, mpINT iLine, const mpCHAR * pFunction,const mpCHAR * format, ...)
{
    int len = 0;
    va_list args;// = mpNULL;
    char * prefix = mpNULL;
    char * buffer = mpNULL;
#ifdef MP_LOG_DBG
    LogTime nowTime;
    memset(&nowTime, 0x00, sizeof(nowTime));
    MUGetLocalTime(&nowTime);
    #define PREFIX "(%02d%02d%02d_%02d:%02d:%02d.%02d)%s<%d>:%s" \
    , nowTime.logSec.tm_year + 1900 \
    , nowTime.logSec.tm_mon + 1 \
    , nowTime.logSec.tm_mday \
    , nowTime.logSec.tm_hour \
    , nowTime.logSec.tm_min \
    , nowTime.logSec.tm_sec \
    , nowTime.logMSec \
    , pszFile \
    , iLine \
    ,pFunction

#else
    #define PREFIX "%s<%d>:%s()",pszFile, iLine,pFunction
#endif

    FILE * pNullFile = fopen("/dev/null", "w");
    if (pNullFile == mpNULL) return;

    len = fprintf(pNullFile, PREFIX);
    len++;        // terminate

    prefix = (mpCHAR *)MP_MALLOC(len);
    if (prefix == NULL)
    {
        fclose(pNullFile);
        return;
    }

    memset(prefix, 0x00, len);
    snprintf(prefix, len, PREFIX);

    va_start(args, format);
    int len2 = vfprintf(pNullFile, format, args) + 2; // LF and terminating
    va_end(args);
    fclose(pNullFile);
    int total = len+len2;
    buffer = (mpCHAR *)MP_MALLOC(total);
    if (buffer == NULL)
    {
        va_end(args);
        MP_FREE(prefix);
        return;
    }

    memset(buffer, 0x00, len+len2);
    strncpy(buffer, prefix, len);
    mpCHAR * temp = buffer + len - 1;
    va_start(args, format);
    vsnprintf(temp, len2, format, args);
    va_end(args);
   // strcat(buffer, "\n");
    printf("%s", buffer);
    MP_FREE(buffer);
    MP_FREE(prefix);
}

