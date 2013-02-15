#ifndef LogWriter_h_
#define LogWriter_h_

#include "LogLevel.h"
#include "stringify.h"

#include <string>
#include <utility>

/*!
 * \file LogWriter.h
 * \ingroup Logging
 * 
 * This file should not be included directly, as it is provided by Log.h
 * Provides the LogWriter object, used for writing log messages
 */

class Log;

//! Temporary for collecting parts of a log message
/*!
 * A LogWriter object is used to collect the parts of a message before those
 * parts are sent to the Log object. This prevents that the Log object has to
 * be completely locked for each log message, because the collecting of the
 * data occurs in a local manner, and Log output objects which targets are not
 * thread safe have to be locked locally.
 *
 * A LogWriter is intended as a temporary cache for the parts of a log message.
 * It writes the cached string to the Log object when it is destructed.
 *
 * Only Log is allowed to create a LogWriter object. (through the Log::message() call)
 */
class LogWriter {
	friend class Log;
	template<class T>
	friend LogWriter&& operator<<( LogWriter&&, const T&);
public:
	~LogWriter();

private:
	/*!
	 * \param logger the Log object the LogWriter should write to
	 * \param ll the log level of the message held be LogWriter
	 */
	LogWriter( Log* logger, LogLevel ll);

	//! appends the given string to the string to log
	/*!
	 * append() is only called by operator<<
	 * 
	 * \param str the string which will be appended
	 */
	void append( const std::string& str);

	//! the string to log
	std::string buffer;
	//! log level of the message
	LogLevel ll;
	//! the logger the message will be written to on destruction
	Log* logger;
};

//! writes all basic types to the LogWriter
/*!
 * This operator should be overloaded to log custom objects.
 * Just declare something like
 * \code
 * LogWriter&& operator<<( LogWriter&& out, const Message& msg);
 * \endcode
 * in the Header of the Messag which should be logged.
 * Implement it in the corresponding Source file using this operator<<
 *
 * Do not forget that because of the rvalue LogWriter you have to return
 * std::forward<LogWriter>(out)
 */
template<class T>
LogWriter&& operator<<( LogWriter&& out, const T& t_) {
	out.append( LIB::stringify(t_));
	return std::forward<LogWriter>(out);
}

#endif // LogWriter_h_
