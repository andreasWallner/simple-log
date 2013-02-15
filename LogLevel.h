#ifndef LogLevel_h_
#define LogLevel_h_

/*!
 * \file LogLevel.h
 * \ingroup Logging
 * 
 * This file should not be included directly, as it is provided by Log.h
 */

//! The severity level of a log message
enum LogLevel {
	LL_emerg,		///< Emergency log level should be used for panic conditions that compromise the state of the system
	LL_alert,		///< For conditions that have to be corrected immediately, such as a corrupted database
	LL_critical,	///< Warning about critical errors, like harddrive/flash errors
	LL_error,		///< For errors
	LL_warning,		///< For warnings
	LL_notice,		///< For conditions that are not errors, but require special handling
	LL_info,		///< Informational messages
	LL_debug		///< Only used for messages required for debugging the program, those should be removed if the error is found
};

#endif // LogLevel_h_