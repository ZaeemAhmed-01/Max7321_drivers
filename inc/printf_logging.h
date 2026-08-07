#pragma once 


typedef enum LogLevel
{
    LOG_INFO = 0,
    LOG_SUCCESS,
    LOG_WARNING,
    LOG_ERROR
} LogLevel_t;

static const char *logLevelToString(LogLevel_t level)
{
    switch(level)
    {
        case LOG_INFO:    return "INFO";
        case LOG_SUCCESS: return "SUCCESS";
        case LOG_WARNING: return "WARNING";
        case LOG_ERROR:   return "ERROR";
        default:          return "UNKNOWN";
    }
}


static const char *logLevelToSymbol(LogLevel_t level)
{
    switch(level)
    {
        case LOG_INFO:    return "~~~";
        case LOG_SUCCESS: return "+++";
        case LOG_WARNING: return "***";
        case LOG_ERROR:   return "!!!";
        default:          return "---";
    }
}

#ifdef USE_PRINTF_LOGGING
    #define LOG(level, msg)   printf("%s [%s] [%s:%d] %s %s\n", \
                                     logLevelToSymbol(level),    \
                                     logLevelToString(level),    \
                                     __FILE__,                   \
                                     __LINE__,                   \
                                     msg,                        \
                                     logLevelToSymbol(level))

#endif