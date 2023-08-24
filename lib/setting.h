// Parameter
#define POOL_NUM 10000
#define POOL_SIZE 8192
#define BUF_LEN 4096

// Debug mode
#define LOG_MODE true
#define DEBUG_AT_CMD false
#define CRAZY_OPTIMIZATION false

// Path
#define ABSOLUTE_PATH false

#if (ABSOLUTE_PATH)
    #define LOG_PATH "/home/harrykuo1/program/project/log/"
#else
    #define LOG_PATH "./log/"
#endif


