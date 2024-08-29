#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <stdio.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

extern bool log_enabled;
extern FILE *log_file;

void log_message(const char *format, ...);
void log_end();

void open_log_file(const char *fname);

#endif