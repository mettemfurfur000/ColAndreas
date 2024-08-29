#include "include/logger.hpp"

bool log_enabled = false;
FILE *log_file = NULL;

void log_message(const char *format, ...)
{
    if (!log_enabled)
        return;

    va_list args;
    va_start(args, format);
    vfprintf(log_file, format, args);
    va_end(args);
    fprintf(log_file, "\n");
    // fflush(log_file);

    return;
}

void log_end()
{
    if (!log_enabled)
        return;
    // fflush(log_file);
    fclose(log_file);
    log_file = NULL;
    log_enabled = false;
}

int is_file_exist(const char *fname)
{
    FILE *file;
    if (file = fopen(fname, "r"))
    {
        fclose(file);
        return 1;
    }
    return 0;
}

void open_log_file(const char *fname)
{
    // char buf_name[256] = {};
    // strcpy(buf_name, fname);
    // strrchr(buf_name, '.')[0] = '\0';
    // int log_latest = 0;

    // for (;; log_latest++)
    // {
    //     sprintf(buf_name, "%s.%d", fname, log_latest);
    //     if (is_file_exist(buf_name))
    //         break;
    // }

    // strncpy(buf_name + strlen(buf_name), ".txt", 4);

    log_file = fopen(fname, "w");
    if (!log_file)
    {
        log_enabled = false;
        printf("Failed to open log file: %s\n", fname);
        printf("Logging disabled.\n");
    }
    else
    {
        log_enabled = true;
        printf("Logging enabled.\n");
    }
}