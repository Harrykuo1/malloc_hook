#include "memory.h"

extern int event_fd, pool_fd;

Pool **pools;
int pool_ptr;



void pool_init(){
    char buf[BUF_LEN];
    #if LOG_MODE
        write(event_fd, "Init pool\n", sizeof("Init pool"));
    #endif

    pool_ptr = 0;
    pools = (Pool **)mmap(NULL, sizeof(Pool*) * POOL_NUM, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0); 
    for(int i=0;i<POOL_NUM;i++){
        pools[i] = (Pool *)mmap(NULL, sizeof(Pool), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
        pools[i]->status = 0;
        pools[i]->address = mmap(NULL, POOL_SIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
        if (pools[i]->address == MAP_FAILED) {
            #if LOG_MODE
                snprintf(buf, BUF_LEN, "Init pool error %d\n", i);
                write(event_fd, buf, strlen(buf));
            #endif
            perror("mmap failed\n");
            exit(1);
        }
        #if LOG_MODE
            snprintf(buf, BUF_LEN, "%p\n", pools[i]->address);
            write(pool_fd, buf, strlen(buf));
        #endif
    }

    return;
}

bool has_pool(void){
    if(pool_ptr < POOL_NUM){
        return true;
    }
    return false;
}

pthread_mutex_t fetch_pool_mutex;
bool fetch_pool(void **ptr){
    bool ret = false;
    // pthread_mutex_lock(&fetch_pool_mutex);
    if(has_pool()){
        int poolID = pool_ptr++;
        *ptr = pools[poolID]->address;
        update_pool_status(poolID, 1);
        ret = true;
    }
    // pthread_mutex_unlock(&fetch_pool_mutex);
    return ret;
}

void update_pool_status(int poolID, int status){
    pools[poolID]->status = status;
    return;
}

bool get_poolID(void *ptr, int *poolID){
    #ifdef CRAZY_OPTIMIZATION 
        if((char *)ptr < (char *)0x6f0000000000){
            return false;
        }
    #endif
    
    // for(int i=0;i<pool_ptr;i++){
    //     if(pools[i]->address == ptr){
    //         *poolID = i;
    //         return true;
    //     }
    // }
    // return false;

    int l=0, r=pool_ptr-1, mid; 
    bool leftSmaller = ((unsigned long *)pools[l]->address < (unsigned long *)pools[r]->address);
   
    while(l<=r){ 
        mid = l + (r - l) / 2;
        if((unsigned long *)pools[mid]->address == (unsigned long *)ptr){
            *poolID = mid;
            return true;
        }
        else if((unsigned long *)ptr < (unsigned long *)pools[mid]->address){
            leftSmaller ? (r = mid - 1) : (l = mid + 1);
        }
        else{
            leftSmaller ? (l = mid + 1) : (r = mid - 1);
        }
    }
    return false;
}