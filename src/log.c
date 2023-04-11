/*
 *    log.c    --    Logging functions
 *
 *    Authored by Karl "p0lyh3dron" Kreuze on February 27, 2022
 *    Refactored by Karl "p0lyh3dron" Kreuze on April 11, 2023
 *
 *    Taken from: https://github.com/p0lyh3dron/libchik/blob/bfba7e11ea13c414b3f63228a6c7c8168e26c0b4/log.c
 *
 *    This file defines the logging functions.
 */
#include "log.h"

#include <stdarg.h>

FILE *_log_file = (FILE *)0x0;

/*
 *    Sets the color of the log messages.
 *
 *    @param log_color_e color   The color to set.
 */
void log_set_color(log_color_e color) {
    switch (color) {
    case LOG_COLOR_CLEAR:
        fprintf(stderr, "\033[0m");
        break;
    case LOG_COLOR_BLACK:
        fprintf(stderr, "\033[30m");
        break;
    case LOG_COLOR_RED:
        fprintf(stderr, "\033[31m");
        break;
    case LOG_COLOR_GREEN:
        fprintf(stderr, "\033[32m");
        break;
    case LOG_COLOR_YELLOW:
        fprintf(stderr, "\033[33m");
        break;
    case LOG_COLOR_BLUE:
        fprintf(stderr, "\033[34m");
        break;
    case LOG_COLOR_MAGENTA:
        fprintf(stderr, "\033[35m");
        break;
    case LOG_COLOR_CYAN:
        fprintf(stderr, "\033[36m");
        break;
    case LOG_COLOR_LIGHT_GREY:
        fprintf(stderr, "\033[37m");
        break;
    case LOG_COLOR_DARK_GREY:
        fprintf(stderr, "\033[90m");
        break;
    case LOG_COLOR_LIGHT_RED:
        fprintf(stderr, "\033[91m");
        break;
    case LOG_COLOR_LIGHT_GREEN:
        fprintf(stderr, "\033[92m");
        break;
    case LOG_COLOR_LIGHT_YELLOW:
        fprintf(stderr, "\033[93m");
        break;
    case LOG_COLOR_LIGHT_BLUE:
        fprintf(stderr, "\033[94m");
        break;
    case LOG_COLOR_LIGHT_MAGENTA:
        fprintf(stderr, "\033[95m");
        break;
    case LOG_COLOR_LIGHT_CYAN:
        fprintf(stderr, "\033[96m");
        break;
    case LOG_COLOR_WHITE:
        fprintf(stderr, "\033[97m");
        break;
    }
}

/*
 *    Logs a message to the console.
 *
 *    @param char *message            The message to log.
 *    @param ...                    The arguments to the message.
 */
void log_msg(const char *message, ...) {
    va_list args_va;
    char    buf[MAX_LOG_MESSAGE_LENGTH];

    va_start(args_va, message);
    vsnprintf(buf, MAX_LOG_MESSAGE_LENGTH, message, args_va);
    va_end(args_va);

    printf("%s", buf);
    if (_log_file)
        fprintf(_log_file, "%s", buf);
}

/*
 *    Logs a note to the console.
 *
 *    @param char *note            The note to log.
 *    @param ...                 The arguments to the note.
 */
void log_note(const char *note, ...) {
    va_list args_va;
    char    buf[MAX_LOG_MESSAGE_LENGTH];

    va_start(args_va, note);
    log_set_color(LOG_COLOR_LIGHT_CYAN);
    vsnprintf(buf, MAX_LOG_MESSAGE_LENGTH, note, args_va);
    va_end(args_va);

    fprintf(stderr, "note | %s", buf);
    if (_log_file)
        fprintf(_log_file, "note | %s", buf);

    log_set_color(LOG_COLOR_CLEAR);
}

/*
 *    Logs a warning to the console.
 *
 *    @param char *warning            The warning to log.
 *    @param ...                    The arguments to the warning.
 */
void log_warn(const char *warning, ...) {
    va_list args_va;
    char    buf[MAX_LOG_MESSAGE_LENGTH];

    va_start(args_va, warning);
    log_set_color(LOG_COLOR_MAGENTA);
    vsnprintf(buf, MAX_LOG_MESSAGE_LENGTH, warning, args_va);
    va_end(args_va);

    fprintf(stderr, "warning | %s", buf);
    if (_log_file)
        fprintf(_log_file, "warning | %s", buf);

    log_set_color(LOG_COLOR_CLEAR);
}

/*
 *    Logs an error to the console.
 *
 *    @param char *error            The error to log.
 *    @param ...                  The arguments to the error.
 */
void log_error(const char *error, ...) {
    va_list args_va;
    char    buf[MAX_LOG_MESSAGE_LENGTH];

    va_start(args_va, error);
    log_set_color(LOG_COLOR_LIGHT_RED);
    vsnprintf(buf, MAX_LOG_MESSAGE_LENGTH, error, args_va);
    va_end(args_va);

    fprintf(stderr, "error | %s", buf);
    if (_log_file)
        fprintf(_log_file, "error | %s", buf);

    log_set_color(LOG_COLOR_CLEAR);
}

/*
 *    Logs a fatal error to the console.
 *
 *    @param char *fatal            The fatal error to log.
 *    @param ...                  The arguments to the fatal error.
 */
void log_fatal(const char *fatal, ...) {
    va_list args_va;
    char    buf[MAX_LOG_MESSAGE_LENGTH];

    va_start(args_va, fatal);
    log_set_color(LOG_COLOR_LIGHT_RED);
    vsnprintf(buf, MAX_LOG_MESSAGE_LENGTH, fatal, args_va);
    va_end(args_va);

    fprintf(stderr, "fatal | %s", buf);
    if (_log_file)
        fprintf(_log_file, "fatal | %s", buf);

    log_set_color(LOG_COLOR_CLEAR);

    exit(EXIT_FAILURE);
}

/*
 *    Opens a file for logging.
 *
 *    @param char *file            The file to open.
 */
void log_open_file(const char *file) {
    _log_file = fopen(file, "w");

    if (_log_file == (FILE *)0x0)
        VLOGF_WARN("Failed to open log file: %s", file);
}

/*
 *    Closes the file opened for logging.
 */
void log_close_file(void) {
    if (_log_file != (FILE *)0x0)
        fclose(_log_file);
}
