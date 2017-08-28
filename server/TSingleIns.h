#ifndef TSINGLEINS_H_
#define TSINGLEINS_H_

#include "global.h"

template <typename T> class TSingleIns
{
public:
    static T * GetInstance(){
        if (m_pInstance == NULL)
        {
            m_pInstance = MP_NEW T;
        }
        return m_pInstance;
    }

    static void ReleaseInstance(){
        if(m_pInstance != NULL){
            MP_DELETE m_pInstance;
            m_pInstance = NULL;
        }
    }
protected:
    virtual ~TSingleIns(){m_pInstance = NULL;}
    static T* m_pInstance;
private:
    T& operator=(const T &){
        /* disable  operator= */
    }
};

template <typename T> T* TSingleIns<T>::m_pInstance = NULL;

#endif /* TSINGLEINS_H_ */
