
#ifndef DEBUG
#include <stdio.h>
#include <string.h>
const unsigned BUFFER_SIZE = 10;
static char print_msg[BUFFER_SIZE];
static unsigned long lastRefreshTime = 0;
static const unsigned long LOG_REFRESH_INTERVAL = 1000;
#define PRINT_LOGS() { \
        if(millis() - lastRefreshTime > LOG_REFRESH_INTERVAL) {  \
            lastRefreshTime = millis();  \
            Serial.println(print_msg);  \
        }  \
    }
#define PRINT_LOGS_TEST() { \
    printf("%s",print_msg); \
}
#define LOG(msg) { \
    int len = (int)strlen(msg); \
    printf("Size:%d\n", len); \
    for(int i=0; i<len; i++) {\
        printf("%c", (char) msg[i]); \
    } \
    printf("\n"); \
}
#else
#define LOG(msg)
#define PRINT_LOGS()
#endif
