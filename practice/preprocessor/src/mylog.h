#ifndef __MY_LOG_H__
#define __MY_LOG_H__

#ifndef CONFIG_LOG_LEVEL
#define CONFIG_LOG_LEVEL 4
#endif

enum class LOG_LEVELS
{
    DEBUG = 1,
    INFO = 2,
    WARN = 3,
    ERROR = 4,
    NO_LOG = 5
};

// TODO
#define LOG(...)

#endif // __MY_LOG_H__
