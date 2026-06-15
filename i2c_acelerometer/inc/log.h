#ifndef LOG_H
#define LOG_H

/*includes----------------------------------------------*/

#include <stdint.h>
#include <stdio.h>

/*defines----------------------------------------------*/

#define INFO_TEMP "\033[0;32m%s:%s\n\e[0m"
#define ERROR_TEMP "\e[31m%s: [%s] [%d] %s\n\e[0m"

/*macros----------------------------------------------*/

#define INFO_LOG(TAG, message, ...) \
    do { \
        uint8_t formatted_message[256]; \
        snprintf(formatted_message, sizeof(formatted_message), message, ##__VA_ARGS__); \
        printf(INFO_TEMP, TAG, formatted_message); \
    }while (0)

#define ERROR_LOG(TAG, message) \
    do { \
        printf(ERROR_TEMP, TAG, __FILE__, __LINE__, message); \
    }while (0)

#endif // LOG_H