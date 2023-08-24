#include "memory.h"

extern int event_fd;
extern Pool **pools;

void init(){
    log_init();
    hook_init();
    pool_init();
    #if LOG_MODE
        write(event_fd, "Init end\n", sizeof("Init end"));
    #endif
    return;
}

pthread_mutex_t init_check_mutex;

void init_check(){
    if(pools != NULL){
        return;
    }
    pthread_mutex_lock(&init_check_mutex);
    if(pools == NULL){
        init();
    }
    pthread_mutex_unlock(&init_check_mutex);
}