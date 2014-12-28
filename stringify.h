#ifndef stringify_h_
#define stringify_h_

#include <string>
#include <sstream>
namespace LIB {

	namespace stringify_internal {
		template<typename T>
		inline std::string real_stringify( const T& item) {
			std::ostringstream s;
			s << item;
			return s.str();
		}
		
		//! a string is good as it is
		inline std::string real_stringify( const std::string& item) {
			return item;
		}
		
		//! interpret a signed char as a 'x' character
		inline std::string real_stringify( const char& item) {
			return std::string( 1, item);
		}
		
		//! interpret an unsigned char as a number
		inline std::string real_stringify( const unsigned char& item) {
			return real_stringify((int)item);
		}
		
		//! replace bools by their string 'equivalences'
		inline std::string real_stringify( const bool& item) {
			return ( item ? "true" : "false");
		}
		
		//! interpret a char* as a c-string
		inline std::string real_stringify( const char* const item) {
			return std::string(item);
		}
	} // namespace stringify_internal

	template<typename T>
	inline std::string stringify( const T& item) {
		return stringify_internal::real_stringify(item);
	}
} // namespace LIB

#endif // stringify_h_
