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
		
		inline std::string real_stringify( const std::string& item) {
			return item;
		}
		
		inline std::string real_stringify( const char& item) {
			return std::string( 1, item);
		}
		
		inline std::string real_stringify( const unsigned char& item) {
			return std::string( 1, item);
		}
		
		inline std::string real_stringify( const bool& item) {
			return ( item ? "true" : "false");
		}
		
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
