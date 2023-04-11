/*
 *    log.h    --    Logging functions
 *
 *    Authored by Karl "p0lyh3dron" Kreuze on February 27, 2022
 *    Refactored by Karl "p0lyh3dron" Kreuze on April 11, 2023
 *
 *    Taken from: https://github.com/p0lyh3dron/libchik/blob/bfba7e11ea13c414b3f63228a6c7c8168e26c0b4/log.h
 *
 *    This file declares the logging functions.
 */
#pragma once

#define MAX_LOG_MESSAGE_LENGTH 1024

#include <stdio.h>
#include <stdlib.h>

#include "types.h"

typedef enum {
    LOG_COLOR_CLEAR,
    LOG_COLOR_BLACK,
    LOG_COLOR_RED,
    LOG_COLOR_GREEN,
    LOG_COLOR_YELLOW,
    LOG_COLOR_BLUE,
    LOG_COLOR_MAGENTA,
    LOG_COLOR_CYAN,
    LOG_COLOR_LIGHT_GREY,
    LOG_COLOR_DARK_GREY,
    LOG_COLOR_LIGHT_RED,
    LOG_COLOR_LIGHT_GREEN,
    LOG_COLOR_LIGHT_YELLOW,
    LOG_COLOR_LIGHT_BLUE,
    LOG_COLOR_LIGHT_MAGENTA,
    LOG_COLOR_LIGHT_CYAN,
    LOG_COLOR_WHITE,
} log_color_e;

/*
 *    Sets the color of the log messages.
 *
 *    @param log_color_e color   The color to set.
 */
void log_set_color(log_color_e color);

/*
 *    Logs a message to the console.
 *
 *    @param char *message            The message to log.
 *    @param ...                    The arguments to the message.
 */
void log_msg(const char *message, ...);

/*
 *    Logs a note to the console.
 *
 *    @param char *note            The note to log.
 *    @param ...                 The arguments to the note.
 */
void log_note(const char *note, ...);

/*
 *    Logs a warning to the console.
 *
 *    @param char *warning            The warning to log.
 *    @param ...                    The arguments to the warning.
 */
void log_warn(const char *warning, ...);

/*
 *    Logs an error to the console.
 *
 *    @param char *error            The error to log.
 *    @param ...                  The arguments to the error.
 */
void log_error(const char *error, ...);

/*
 *    Logs a fatal error to the console.
 *
 *    @param char *fatal            The fatal error to log.
 *    @param ...                  The arguments to the fatal error.
 */
void log_fatal(const char *fatal, ...);

/*
 *    Opens a file for logging.
 *
 *    @param char *file            The file to open.
 */
void log_open_file(const char *file);
/*
 *    Closes the file opened for logging.
 */
void log_close_file(void);

#define VLOGF_MSG(msg, ...) log_msg("%s: " msg, __func__, __VA_ARGS__)
#define LOGF_MSG(msg)       log_msg("%s: " msg, __func__)

#define VLOGF_NOTE(note, ...) log_note("%s: " note, __func__, __VA_ARGS__)
#define LOGF_NOTE(note)       log_note("%s: " note, __func__)

#define VLOGF_WARN(warn, ...) log_warn("%s: " warn, __func__, __VA_ARGS__)
#define LOGF_WARN(warn)       log_warn("%s: " warn, __func__)

#define VLOGF_ERR(err, ...) log_error("%s: " err, __func__, __VA_ARGS__)
#define LOGF_ERR(err)       log_error("%s: " err, __func__)

#define VLOGF_FAT(fat, ...) log_msg("%s: " fat, __func__, __VA_ARGS__)
#define LOGF_FAT(fat)       log_msg("%s: " fat, __func__)
