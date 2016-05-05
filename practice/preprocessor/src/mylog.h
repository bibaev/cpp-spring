#ifndef __MY_LOG_H__
#define __MY_LOG_H__

#ifndef CONFIG_LOG_LEVEL
#define CONFIG_LOG_LEVEL 0
#endif

enum class LOG_LEVELS {
    DEBUG = 1,
    INFO = 2,
    WARN = 3,
    ERROR = 4,
    NO_LOG = 5
};

// TODO
#include <cstdio>
#define LOG(level, format, ...) do { \
    if (level >= static_cast<LOG_LEVELS>(CONFIG_LOG_LEVEL)) { \
        printf("[%s : %d : %s ] " format "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__);\
    } \
} while(0, 0)

#endif // __MY_LOG_H__
