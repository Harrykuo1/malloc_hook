#include "memory.h"
#include "debug.h"

extern int event_fd;

void hook_init(){
    #if LOG_MODE
        write(event_fd, "Init hook\n", sizeof("Init hook"));
    #endif

    real_malloc = (void *(*)(size_t))dlsym(RTLD_NEXT, "malloc");
    real_calloc = (void *(*)(size_t, size_t))dlsym(RTLD_NEXT, "calloc");
    real_realloc = (void *(*)(void *, size_t))dlsym(RTLD_NEXT, "realloc");
    real_free = (void (*)(void *))dlsym(RTLD_NEXT, "free"); 

    return;
}

void *malloc(size_t size){
    char buf[BUF_LEN];
    init_check();

    DEBUG_CMD("Malloc %lu\n", size);

    void *ret;
    int poolID;
    if(size <= POOL_SIZE && fetch_pool(&ret)){
        #if LOG_MODE
            snprintf(buf, BUF_LEN, "Malloc  pool %p %lu\n", ret, size);
            write(event_fd, buf, strlen(buf));
        #endif
    }
    else{
        ret = (*real_malloc)(size);

        #if LOG_MODE
            snprintf(buf, BUF_LEN, "Malloc  libc %p %lu\n", ret, size);
            write(event_fd, buf, strlen(buf));
        #endif
    }

    return ret;
}

void *calloc(size_t num, size_t size){
    char buf[BUF_LEN];
    init_check();

    DEBUG_CMD("Calloc %d %lu\n", num, size);

    void *ret;
    int poolID;
    if(num * size <= POOL_SIZE && fetch_pool(&ret)){
        // memset(ret, 0, num * size);
        #if LOG_MODE
            snprintf(buf, BUF_LEN, "Calloc  pool %p %lu\n", ret, num*size);
            write(event_fd, buf, strlen(buf));
        #endif
    }
    else{
        ret = (*real_calloc)(num, size);

        #if LOG_MODE
            snprintf(buf, BUF_LEN, "Calloc  libc %p %lu\n", ret, num*size);
            write(event_fd, buf, strlen(buf));
        #endif
    }

    return ret;
}

void *realloc(void *ptr, size_t size){
    char buf[BUF_LEN];
    init_check();

    DEBUG_CMD("Realloc %p %lu\n", ptr, size);

    void *ret;
    int poolID;
    if(get_poolID(ptr, &poolID)){
        if(size <= POOL_SIZE){
            ret = ptr;

            #if LOG_MODE
                snprintf(buf, BUF_LEN, "Realloc pool1 %p %lu\n", ret, size);
                write(event_fd, buf, strlen(buf));
            #endif
        }
        else{
            ret = (*real_malloc)(size);
            memcpy(ret, ptr, size);
            free(ptr);

            #if LOG_MODE
                snprintf(buf, BUF_LEN, "Realloc pool2 %p %lu\n", ret, size);
                write(event_fd, buf, strlen(buf));
            #endif
        }
    }
    else{
        ret = (*real_realloc)(ptr, size);

        #if LOG_MODE
            snprintf(buf, BUF_LEN, "Realloc libc %p %lu\n", ret, size);
            write(event_fd, buf, strlen(buf));
        #endif
    }

    return ret;
}

void free(void *ptr){
    char buf[BUF_LEN];
    init_check();
    
    DEBUG_CMD("Free %p\n", ptr);

    if(ptr == NULL){
        return;
    }

    int poolID;
    if(get_poolID(ptr, &poolID)){
        #if LOG_MODE
            snprintf(buf, BUF_LEN, "Free    pool %p\n", ptr);
            write(event_fd, buf, strlen(buf));
        #endif

        update_pool_status(poolID, 2);
    }
    else{
        #if LOG_MODE
            snprintf(buf, BUF_LEN, "Free    libc %p\n", ptr);
            write(event_fd, buf, strlen(buf));
        #endif

        (real_free)(ptr);
    }

    return;
}