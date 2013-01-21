#include "PThreadMutex.h"

PThreadMutex::PThreadMutex() {
    pthread_mutex_init(&mutex, NULL);
}

void PThreadMutex::lock() {
    pthread_mutex_lock(&mutex);
}


void PThreadMutex::unlock() {
    pthread_mutex_unlock(&mutex);
}
