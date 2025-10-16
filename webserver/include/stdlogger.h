//
//  stdlogger.h
//  webserver
//
//  Created by Victor Padron on 15/10/25.
//

static void stdout_log(Logger *self, LogLevel level, const char *fmt, ...);
static void stdout_destroy(Logger *self);
Logger *create_stdout_logger(void);
