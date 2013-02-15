#include <iostream>
#include <string>
#include <utility>

struct LogWriter {
   ~LogWriter() { std::cout << buffer; }
   std::string buffer;
};

struct Log {
   LogWriter message() { return LogWriter(); }
};

template<class T>
LogWriter&& operator<<( LogWriter&& out, const T& t_) {
   out.buffer += t_;
   return std::forward<LogWriter>(out);
}

LogWriter&& operator<<( LogWriter&& out, const int i) {
   out << std::string("int");
   return std::forward<LogWriter>(out);
}

int main( int argc, char* argv[]) {
   Log log;

   log.message() << std::string("asdf") << std::string("foo");
}
