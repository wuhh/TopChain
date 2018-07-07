#ifndef TIME_H_HHWU
#define TIME_H_HHWU

#include <sys/time.h>

struct Timer
{
    timeval t1;
    timeval t2;
    void start()
    {
        gettimeofday(&this->t1, NULL);
    }
    void stop()
    {
        gettimeofday(&this->t2, NULL);
    }
    float GetRuntime()
    {
        float t=(float)(t2.tv_sec-t1.tv_sec)*1.0+(float)(t2.tv_usec-t1.tv_usec)/1000000.0;
        return t;
    }
};

#endif
