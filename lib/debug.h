/*
    This file is for a better debug method.
    You can set the macro DEBUG_AT_CMD as true at setting.h to turn on the debug mode.
    
    if the macro DEBUG_AT_CMD is setting as true:
    You can use the macro DEBUG_CMD to ouput the error message at terminal.
    Example:
        DEBUG_CMD("Debug information");
        DEBUG_CMD("Output a number %d", number);
    It is used like function "printf", but the debug information will write to stderr.
    
    if the macro DEBUG_AT_CMD is not setting or setting as false:
    the macro DEBUG_CMD will not do anything.

    another macro DEBUG_LOG is same as DEBUG_CMD, but it need to pass a file descriptor
    and the macro will use write function to write it to file.

    About the buffer size
    The debug method also use buffer, you can use macro DEBUG_BUFFSIZE to change the buffer size.
    The default buffer size is 4096 bytes.
*/

#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include <errno.h>
#include "setting.h"


// you can give the macro at setting.h to change the buffer size
// or you can give it while compiling.
// the default buffer setting size is 4096.
#ifndef DEBUG_BUFFSIZE
    #define DEBUG_BUFFSIZE 4096
#endif

static char debug_buffer[DEBUG_BUFFSIZE];

#ifdef DEBUG_AT_CMD
    #if DEBUG_AT_CMD
        #define DEBUG_CMD(fmt,...)  { \
                                        snprintf(debug_buffer, DEBUG_BUFFSIZE, fmt, ##__VA_ARGS__); \
                                        int res = write(2, debug_buffer, strlen(debug_buffer)); \
                                        if (res != strlen(debug_buffer)) \
                                            perror("Write fail"); \
                                    }
    #else
        // define as nothing
        #define DEBUG_CMD(fmt,...) 
    #endif
#else
    // define as nothing
    #define DEBUG_CMD(fmt,...) 
#endif

#ifdef LOG_MODE
    #if LOG_MODE
        #define DEBUG_LOG(fd,fmt,...)   { \
                                            snprintf(debug_buffer, DEBUG_BUFFSIZE, fmt, ##__VA_ARGS__); \
                                            int res = write(fd, debug_buffer, strlen(debug_buffer)); \
                                            if (res != strlen(debug_buffer)) \
                                                perror("Write fail"); \
                                        }   
    #else
        // define as nothing
        #define DEBUG_LOG(fd,fmt,...)
    #endif
#else
    // define as nothing
    #define DEBUG_LOG(fd,fmt,...)
#endif

#endif