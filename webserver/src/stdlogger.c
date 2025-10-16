//
//  stglogger.c
//  webserver
//
//  Created by Victor Padron on 15/10/25.
//

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "log.h"

static void stdout_log(Logger *self, LogLevel level, const char *fmt, ...) {
    (void)self; // unused
    va_list args;
    va_start(args, fmt);

    // get time details
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    
    const char *level_str[] = {"DEBUG", "INFO", "WARN", "ERROR"};
    fprintf(stdout, "[%s] - ", level_str[level]);
    fprintf(stdout, "%d-%02d-%02d %02d:%02d:%02d - ", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    vfprintf(stdout, fmt, args);
    fprintf(stdout, "\n");

    va_end(args);
}

static void stdout_destroy(Logger *self) {
    free(self);
}

Logger *create_stdout_logger(void) {
    Logger *logger = malloc(sizeof(Logger));
    logger->log = stdout_log;
    logger->destroy = stdout_destroy;
    logger->context = NULL;
    return logger;
}
