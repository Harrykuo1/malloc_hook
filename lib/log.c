#include "memory.h"

int event_fd, pool_fd, tmp_fd;

void log_init(){
    #if LOG_MODE
        event_fd = open(LOG_PATH"event.txt", O_WRONLY | O_CREAT | O_APPEND, 0640);
        pool_fd = open(LOG_PATH"pool.txt", O_WRONLY | O_CREAT | O_APPEND, 0640);
        tmp_fd = open(LOG_PATH"tmp.txt", O_WRONLY | O_CREAT | O_APPEND, 0640);
        DEBUG_LOG(event_fd, "Init log\n");
    #endif
    return;
}