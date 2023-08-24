#ifndef memory_H
#define memory_H
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <unistd.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/mman.h>


#include "setting.h"

typedef struct pool{
    int status;
    void *address;
}Pool;

static void *(*real_malloc)(size_t) = NULL;
static void *(*real_calloc)(size_t, size_t) = NULL;
static void *(*real_realloc)(void *, size_t) = NULL;
static void (*real_free)(void *) = NULL;


void init(void);
void init_check(void);
void log_init(void);

void pool_init(void);
bool has_pool(void);
bool fetch_pool(void **ret);
void update_pool_status(int poolID, int status);
bool get_poolID(void *ptr, int *poolID);



void hook_init(void);
void *malloc(size_t size);
void *calloc(size_t num, size_t size);
void *realloc(void *ptr, size_t size);
void free(void *ptr);

#endif