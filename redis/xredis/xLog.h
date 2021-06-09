//
// Created by admin on 2021/6/9.
//

#ifndef XREDIS_XLOG_H
#define XREDIS_XLOG_H

#include <stdint.h>
#include <stdio.h>

#define LOG_LEVEL_ERROR 0
#define LOG_LEVEL_WARN 1
#define LOG_LEVEL_INFO 2
#define LOG_LEVEL_DEBUG 3

#define xredis_error(...) log_message(LOG_LEVEL_ERROR, __VA_ARGS__)
#define xredis_warn(...)  log_message(LOG_LEVEL_WARN, __VA_ARGS__)
#define xredis_info(...)  log_message(LOG_LEVEL_INFO, __VA_ARGS__)
#define xredis_debug(...) log_message(LOG_LEVEL_DEBUG, __VA_ARGS__)

#define DATA_BUF 4096

void log_message(uint32_t level, const char* fmt, ...);

void set_log_level(uint32_t level, void (* emit)(int level, const char* line));

#endif //XREDIS_XLOG_H
