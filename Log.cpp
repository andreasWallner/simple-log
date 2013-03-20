#include "Log.h"

#include <syslog.h>
#include <iostream>
#include <stdexcept>

Log logger;

Log::Log()
: output(0) {
}

Log::~Log() {
	delete this->output;
}

LogWriter Log::message( LogLevel ll) {
 	return LogWriter( this, ll);
}

void Log::setOutputChain( outputChain* output) {
	delete this->output;
	this->output = output;
}

void Log::log( LogLevel ll, const std::string& str) {
	std::lock_guard<std::mutex> lk(m_m);
	if( output)
		output->log( ll, str);
}

outputChain::outputChain( LogLevel minimumLL, bool doPropagate, outputChain* nextLink) {
	this->doPropagate = doPropagate;
	this->nextLink = nextLink;
	this->minimumLL = minimumLL;
}

outputChain::~outputChain() {
	delete nextLink;
}

void outputChain::setLogLevel( LogLevel ll) {
	minimumLL = ll;
}

LogLevel outputChain::logLevel() const {
	return minimumLL;
}

void outputChain::log( LogLevel ll, const std::string& str) {
	if( ll <= minimumLL)
		real_log( ll, str);
	
	if( nextLink && ( ll > minimumLL || (ll <= minimumLL && doPropagate) ) )	
		nextLink->log( ll, str);
}

coutChainLink::coutChainLink( LogLevel minimumLL, bool doPropagate /* = 1*/, outputChain* nextLink /* = 0*/)
: outputChain( minimumLL, doPropagate, nextLink) {}

coutChainLink::~coutChainLink() {}

void coutChainLink::real_log( LogLevel /* ll */, const std::string& str) {
	std::cout << str << std::endl;
}

cerrChainLink::cerrChainLink( LogLevel minimumLL, bool doPropagate /* = 1*/, outputChain* nextLink /* = 0*/) 
: outputChain( minimumLL, doPropagate, nextLink) {}

cerrChainLink::~cerrChainLink() {}

void cerrChainLink::real_log( LogLevel /* ll */, const std::string& str) {
	std::cerr << str << std::endl;
}

fileChainLink::fileChainLink( const std::string& filename, bool overrideFile, LogLevel minimumLL, bool doPropagate /* = 1*/, outputChain* nextLink /* = 0*/)
: outputChain( minimumLL, doPropagate, nextLink) {
	if( overrideFile)
		file.open(filename.c_str(), std::ios_base::out);
	else
		file.open(filename.c_str(), std::ios_base::app);
	if( !file.is_open())
		throw std::invalid_argument("could not open log file");
}

fileChainLink::~fileChainLink() {
	file.close();
}

void fileChainLink::real_log( LogLevel /* ll */, const std::string& str) {
	file << str << std::endl;
}

syslogChainLink::syslogChainLink( const std::string& progName, LogLevel minimumLL, bool doPropagate /* = 1*/, outputChain* nextLink /* = 0*/)
: outputChain( minimumLL, doPropagate, nextLink) {
	this->progName = progName;
	openlog( this->progName.c_str(), LOG_CONS | LOG_PID, LOG_USER);
	syslog( LOG_INFO, "started logging");
}

syslogChainLink::~syslogChainLink() {
	syslog( LOG_INFO, "ended logging");
	closelog();
}

void syslogChainLink::real_log( LogLevel ll, const std::string& str) {
	syslog( ll, "%s", str.c_str());
}

bufferingChainLink::bufferingChainLink( LogLevel minimumLL, bool doPropagate /* = 1 */, outputChain* nextLink /* = 0 */)
: outputChain( minimumLL, doPropagate, nextLink) {}

const bufferingChainLink::LogBuffer& bufferingChainLink::getMessages() {
   return buffer;
}

void bufferingChainLink::clear() {
   buffer.clear();
}

void bufferingChainLink::real_log( LogLevel ll, const std::string& str) {
   buffer.push_back( make_pair( ll, str));
}

