/*
 *    log.c    --    Logging functions
 *
 *    Authored by Karl "p0lyh3dron" Kreuze on February 27, 2022
 *
 *    This file defines the logging functions.
 */
#include "log.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
/*
 *    Logs a note ( low severity, doesn't change program execution )
 *
 *    @param const s8 *
 *        The message to log.
 *    @param ...
 *        The arguments to the message.
 */
void log_note(const s8 *spMsg, ...) {
    va_list vaArgs;
    s8      pBuffer[1024];
    va_start(vaArgs, spMsg);
    vsprintf(pBuffer, spMsg, vaArgs);

    fprintf(stderr, "\033[0;33mNOTE: ");
    fprintf(stderr, "%s", pBuffer);
    fprintf(stderr, "\033[0m");

    va_end(vaArgs);
}
/*
 *    Logs a warning ( medium severity, may change program execution )
 *
 *    @param const s8 *
 *        The message to log.
 *    @param ...
 *        The arguments to the message.
 */
void log_warning(const s8 *spMsg, ...) {
    va_list vaArgs;
    s8      pBuffer[1024];
    va_start(vaArgs, spMsg);
    vsprintf(pBuffer, spMsg, vaArgs);

    fprintf(stderr, "\033[0;34mWARNING: ");
    fprintf(stderr, "%s", pBuffer);
    fprintf(stderr, "\033[0m");

    va_end(vaArgs);
}
/*
 *    Logs an error ( high severity, likely will change program execution )
 *
 *    @param const s8 *
 *        The message to log.
 *    @param ...
 *        The arguments to the message.
 */
void log_error(const s8 *spMsg, ...) {
    va_list vaArgs;
    s8      pBuffer[1024];
    va_start(vaArgs, spMsg);
    vsprintf(pBuffer, spMsg, vaArgs);

    fprintf(stderr, "\033[0;35mERROR: ");
    fprintf(stderr, "%s\n", pBuffer);
    fprintf(stderr, "\033[0m");

    va_end(vaArgs);
}
/*
 *    Logs a fatal error ( highest severity, program will exit )
 *
 *    @param const s8 *
 *        The message to log.
 *    @param ...
 *        The arguments to the message.
 */
void log_fatal(const s8 *spMsg, ...) {
    va_list vaArgs;
    s8      pBuffer[1024];
    va_start(vaArgs, spMsg);
    vsprintf(pBuffer, spMsg, vaArgs);

    fprintf(stderr, "\033[0;31mFATAL: ");
    fprintf(stderr, "%s", pBuffer);
    fprintf(stderr, "\033[0m");

    va_end(vaArgs);
}
/*
 *    Logs a debug message ( disabled by default )
 *
 *    @param const s8 *
 *        The message to log.
 *    @param ...
 *        The arguments to the message.
 */
void log_debug(const s8 *spMsg, ...) {
#if DEBUG
    va_list vaArgs;
    s8      pBuffer[1024];
    va_start(vaArgs, spMsg);
    vsprintf(pBuffer, spMsg, vaArgs);

    fprintf(stderr, "\033[0;36mDEBUG: ");
    fprintf(stderr, "%s", pBuffer);
    fprintf(stderr, "\033[0m");

    va_end(vaArgs);
#endif
}
/*
 *    Logs a verbose message ( disabled by default )
 *
 *    @param const s8 *
 *        The message to log.
 *    @param ...
 *        The arguments to the message.
 */
void log_verbose(const s8 *spMsg, ...) {
    va_list vaArgs;
    s8      pBuffer[1024];
    va_start(vaArgs, spMsg);
    vsprintf(pBuffer, spMsg, vaArgs);

    fprintf(stderr, "%s", pBuffer);

    va_end(vaArgs);
}