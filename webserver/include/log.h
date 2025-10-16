//
//  log.h
//  webserver
//
//  Created by Victor Padron on 15/10/25.
//

typedef enum {
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR
} LogLevel;

// I come from java, I like verbose.
// Here's a kind of 'interfacy' way of creating a Logger.
// We'll implement different types of loggers, like stdout, file, json formatted, etc..

typedef struct Logger {
    void (*log)(struct Logger *self, LogLevel level, const char *fmt, ...);
    void (*destroy)(struct Logger *self);
    void *context; // optional: e.g., FILE* or state
} Logger;
