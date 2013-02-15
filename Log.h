#ifndef Log_h_
#define Log_h_

#include "LogWriter.h"
#include "LogLevel.h"

#include <fstream>
#include <vector>
#include <utility>

/*!
 * \file Log.h
 * \ingroup Logging
 * 
 * Log.h provides all information to use the standard logging facilities
 */ 


class outputChain;

//! basic logging facility
/*!
 * Log provides a basic, but very flexible logging facility.
 * At program start, one Log object called logger is created.
 * 
 * The basic idea behing Log is to specify a chain of output Objects.
 * Every log message is forwarded to each of those objects, but an object
 * can tell Log to stop forwarding messages. It would also be possible to specify
 * a filter output object, which basically watches the log level and stop relaying the
 * messages. 
 *
 * The output chain of an Log object will be deleted by specifying a new output chain,
 * or on deletion of the Log object.
 *
 * To specify an output chain, just create the chain in the following manner:
 *
 * Logging with Log should be threadsafe, as long as the output links are written in an threadsafe
 * manner. Thread safety in Log is achieved through buffering a log message in a temporary until
 * the whole log message is known. The temporary logs the held log message on destruction.
 * The logging section for this buffered message is guarded to only allow on
 * message being logged at any given time.
 * While logging is threadsafe, switching the output chain is (ATM) not! But threadsafety could
 * be provided by implementing some sort of RCU, providing a buffer for the new chain,
 * which only overrides the current chain if no thread is writing at that moment (e.g. atomic
 * counter at the beginning of log)
 *
 * \code
 * setOutputChain( new link1(..., new link2(...)));
 * \endcode
 *
 * To log a message simply use a loggers message() method
 * \code
 * logger.message(LL_info) << "log message with log level info";
 * \endcode
 *
 * \sa LogWriter
 * \sa outputChain
 */
class Log {
	friend class LogWriter;
public:
	Log();
	/*!
	 * destructs the attached output chain
	 */
	~Log();
	
	//! log a messages
	/*!
	 * Creates a temporary object to log to.
	 * The log object writes the log on destruction.
	 * 
	 * \param ll the log level of the message
	 */
	LogWriter message( LogLevel ll);

	//! specifiy to which media the output should be written
	/*!
	 * When calling setOutputChain the ownership of the outputChain
	 * is transferred to the Log object. It will take care of the destruction
	 * of the outputChain.
	 */
	void setOutputChain( outputChain* output);

private:
	//! send a log message to the output links
	void log( LogLevel ll, const std::string& str);
	//! the current output chain
	outputChain* output;
};

//! a logger which can be used application wide provided for ease of use
//! if needed, others can be instanciated
extern Log logger;

//! base class for all output links
/*!
 * outputChain specifies the basic interface for all outputChain objects
 * It provides a non virtual interface (NVI) to overload.
 * 
 * Every output link object has to override real_log. That function should write
 * the specified log message to the given facility.
 */
class outputChain {
	friend class Log;
public:
	/*!
	 * Create the output link with the specified minimal log level.
	 * Also specify wheter the message should be propageted to the next link.
	 * If doPropagate is set to false, Log will stop calling links after this link
	 * wrote the log message. 
	 * nextLink is used to concatenate the list of output links, the ownership of a
	 * supplied 'child' goes over to the output object. It will handle destruction
	 * of the supplied object.
	 *
	 * \param minimumLL the minimal log level which should be logged
	 * \param doPropagate determine wheter the next logger should be called if 
	 * 	the message was logged
	 * \param nextLink the next output link
	 */
	outputChain( LogLevel minimumLL, bool doPropagate, outputChain* nextLink);
	
	//! change minimumLL
	/*!
	 * All messages with a log level smaller than the set minimum log level 
	 * are discarded.
	 *
	 * \param minimumLL the new minimal log loeve
	 */
	void setLogLevel( LogLevel minimumLL);

	//! retrieve the minimum log level
	LogLevel logLevel() const;
	virtual ~outputChain();
protected:
	//! NVI for logging a message
	void log( LogLevel ll, const std::string& str);

	//! method which should be overloaded by concrete links 
	/*!
	 * \param ll log level of the message
	 * \param str log message which should be written
	 */
	virtual void real_log( LogLevel ll, const std::string& str) = 0;
private:
	//! save if log message should be propagated to next link object
	bool doPropagate;
	//! next link object
	outputChain* nextLink;
	//! minimal log level which should be logged
	LogLevel minimumLL;
};

//! an output link writing to std::cout
class coutChainLink : public outputChain {
public:
	coutChainLink( LogLevel minimumLL, bool doPropagate = 1, outputChain* nextLink = 0);
	~coutChainLink();
protected:
	void real_log( LogLevel ll, const std::string& str);
};

//! an output link writing to std::cerr
class cerrChainLink : public outputChain {
public:
	cerrChainLink( LogLevel minimumLL, bool doPropagate = 1, outputChain* nextLink = 0);
	~cerrChainLink();
protected:
	void real_log( LogLevel ll, const std::string& str);
};

//! an output link writing to a file
class fileChainLink : public outputChain {
public:
	/*!
	 * Create the output link with the specified minimal log level.
	 * Also specify wheter the message should be propageted to the next link.
	 * If doPropagate is set to false, Log will stop calling links after this link
	 * wrote the log message. 
	 * nextLink is used to concatenate the list of output links, the ownership of a
	 * supplied 'child' goes over to the output object. It will handle destruction
	 * of the supplied object.
	 *
	 * \param filename the file to open and write to
	 * \param overrideFile determinte wheter the file should be overwritten or be appended to
	 * \param minimumLL the minimal log level which should be logged
	 * \param doPropagate determine wheter the next logger should be called if 
	 * 	the message was logged
	 * \param nextLink the next output link
	 */
	fileChainLink( const std::string& filename, bool overrideFile, LogLevel minimumLL, bool doPropagate = 1, outputChain* nextLink = 0);
	~fileChainLink();
protected:
	std::ofstream file;
	void real_log( LogLevel ll, const std::string& str);
};

//! an output link writing to the syslog deamon
class syslogChainLink : public outputChain {
public:
	/*!
	 * Create the output link with the specified minimal log level.
	 * Also specify wheter the message should be propageted to the next link.
	 * If doPropagate is set to false, Log will stop calling links after this link
	 * wrote the log message. 
	 * nextLink is used to concatenate the list of output links, the ownership of a
	 * supplied 'child' goes over to the output object. It will handle destruction
	 * of the supplied object.
	 *
	 * \param progName the program name which should be displayed be the syslog daemon
	 * \param minimumLL the minimal log level which should be logged
	 * \param doPropagate determine wheter the next logger should be called if 
	 * 	the message was logged
	 * \param nextLink the next output link
	 */
	syslogChainLink( const std::string& progName, LogLevel minimumLL, bool doPropagate = 1, outputChain* nextLink = 0);
	~syslogChainLink();
protected:
	std::string progName;
	void real_log( LogLevel ll, const std::string& str);
};

//! an output link that buffers all log messages that it saw
/*! It is mainly thought to be used in test code, where we want to verify the
 *  logged message. In normal productive code it should only be used with great
 *  care, since the memory usage of this link grow with every logged message!
 *
 *  Since this class is not intended for productive use, access to buffer etc.
 *  are not threadsafe!
 */
class bufferingChainLink : public outputChain {
public:
   typedef std::vector<std::pair<LogLevel, std::string>> LogBuffer;

   bufferingChainLink( LogLevel minimumLL, bool doPropagate = 1, outputChain* nextLink = 0);
   ~bufferingChainLink() {}

   //! returns all logged messages since last clear(), not threadsafe!
   const LogBuffer& getMessages();
   //! empties buffer, not threadsafe()
   void clear();
protected:
   LogBuffer buffer;
   void real_log( LogLevel ll, const std::string& str);
};


#endif // Log_h_
