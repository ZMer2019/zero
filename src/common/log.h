//
// Created by edward on 2021/11/16.
//

#ifndef ZERO_LOG_H
#define ZERO_LOG_H

#include <ctime>
#include <cerrno>
#include <cstring>
#include <cstdio>
#define TIME_FMT "%Y-%m-%d %H:%M:%S"

#define LOG(fmt, ...)                                                                           \
do{                                                                                             \
    time_t t;                                                                                   \
    struct tm *tm;                                                                              \
    char timestr[64] = {0};                                                                     \
    time(&t);                                                                                   \
    tm = localtime(&t);                                                                         \
    strftime(timestr, sizeof(timestr), TIME_FMT, tm);                                           \
    char buff[4096] = {0};                                                                      \
    sprintf(buff, "[%s, %s:%d]: " fmt, timestr, __FUNCTION__, __LINE__, ##__VA_ARGS__);         \
    printf("%s", buff);                                                                         \
}while(0)

#endif //ZERO_LOG_H
