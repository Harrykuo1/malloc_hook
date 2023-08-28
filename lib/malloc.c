#include "memory.h"
#include "debug.h"

extern int event_fd;

// This function will get the origin memory allocation function from stanard libary.
// It will use function pointer to store it. 
void hook_init(){
    DEBUG_LOG(event_fd, "Init hook\n");

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
        DEBUG_LOG(event_fd, "Malloc  pool %p %lu\n", ret, size);
    }
    else{
        ret = (*real_malloc)(size);
        DEBUG_LOG(event_fd, "Malloc  libc %p %lu\n", ret, size);
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
        DEBUG_LOG(event_fd, "Calloc  pool %p %lu\n", ret, num*size);
    }
    else{
        ret = (*real_calloc)(num, size);

        DEBUG_LOG(event_fd, "Calloc  libc %p %lu\n", ret, num*size);
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

            DEBUG_LOG(event_fd, "Realloc pool1 %p %lu\n", ret, size);
        }
        else{
            ret = (*real_malloc)(size);
            memcpy(ret, ptr, size);
            free(ptr);

            DEBUG_LOG(event_fd, "Realloc pool2 %p %lu\n", ret, size);
        }
    }
    else{
        ret = (*real_realloc)(ptr, size);

        DEBUG_LOG(event_fd, "Realloc libc %p %lu\n", ret, size);
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
        DEBUG_LOG(event_fd, "Free    pool %p\n", ptr);
        update_pool_status(poolID, 2);
    }
    else{
        DEBUG_LOG(event_fd, "Free    libc %p\n", ptr);
        (*real_free)(ptr);
    }

    return;
}