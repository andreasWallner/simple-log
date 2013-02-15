#include "LogWriter.h"
#include "Log.h"

#include <sstream>

LogWriter::LogWriter( Log* logger, LogLevel ll)
: ll(ll)
, logger( logger) {}

LogWriter::~LogWriter() {
	if( buffer.size())
		logger->log( ll, buffer);
}

void LogWriter::append( const std::string& str) {
	buffer += str;
}

