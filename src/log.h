/*
 *    log.h    --    Logging functions
 *
 *    Authored by Karl "p0lyh3dron" Kreuze on February 27, 2022
 * 
 *    This file declares the logging functions.
 */
#pragma once

#include "types.h"
/*
 *    Logs a note ( low severity, doesn't change program execution )
 *
 *    @param const s8 *
 *        The message to log.
 *    @param ...
 *        The arguments to the message.
 */
void log_note( const s8 *spMsg, ... );
/*
 *    Logs a warning ( medium severity, may change program execution )
 *
 *    @param const s8 *
 *        The message to log.
 *    @param ...
 *        The arguments to the message.
 */
void log_warning( const s8 *spMsg, ... );
/*
 *    Logs an error ( high severity, likely will change program execution )
 *
 *    @param const s8 *
 *        The message to log.
 *    @param ...
 *        The arguments to the message.
 */
void log_error( const s8 *spMsg, ... );
/*
 *    Logs a fatal error ( highest severity, program will exit )
 *
 *    @param const s8 *
 *        The message to log.
 *    @param ...
 *        The arguments to the message.
 */
void log_fatal( const s8 *spMsg, ... );
/*
 *    Logs a debug message ( disabled by default )
 *
 *    @param const s8 *
 *        The message to log.
 *    @param ...
 *        The arguments to the message.
 */
void log_debug( const s8 *spMsg, ... );
/*
 *    Logs a verbose message ( disabled by default )
 *
 *    @param const s8 *
 *        The message to log.
 *    @param ...
 *        The arguments to the message.
 */
void log_verbose( const s8 *spMsg, ... );