#ifndef PTHREADMUTEX_H
#define	PTHREADMUTEX_H

#include <pthread.h>

/**
 * Wraps a pthread_mutex
 */
class PThreadMutex {
public:
    PThreadMutex();
    void lock();
    void unlock();
private:
    pthread_mutex_t mutex;
};

#endif	/* PTHREADMUTEX_H */

