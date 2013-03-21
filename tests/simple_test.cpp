#include <cppunit/extensions/HelperMacros.h>
#include "../Log.h"

#include <utility>
#include <string>
#include <iostream>

struct test {
   int i;
   std::string s;
};

LogWriter&& operator<<( LogWriter&& out, const test& t);
LogWriter&& operator<<( LogWriter&& out, const test& t) {
   std::forward<LogWriter>(out) << "[" << t.i << "," << t.s << "]";
   return std::forward<LogWriter>(out);
}

class simple_test : public CppUnit::TestFixture
{
   CPPUNIT_TEST_SUITE(simple_test);
   CPPUNIT_TEST(simple);
   CPPUNIT_TEST_SUITE_END();

private:
   bufferingChainLink* loggedMessages;

public:
   void setUp() 
   {
      loggedMessages = new bufferingChainLink(LL_debug);
      logger.setOutputChain(loggedMessages);
   }

   void tearDown()
   {
   }

   void simple()
   {
		char foo[] = "foo";
		test t = { 5, "foo" };
		unsigned char x = 222;
		logger.message(LL_debug) << foo;
      logger.message(LL_error) << "a" << "b" << "c";
      logger.message(LL_debug) << t;
		logger.message(LL_debug) << x;

      auto messages = loggedMessages->getMessages();
      CPPUNIT_ASSERT_MESSAGE( "number", messages.size() == 4);
		CPPUNIT_ASSERT_MESSAGE( "0", messages.at(0) == std::make_pair( LL_debug, std::string("foo")));
      CPPUNIT_ASSERT_MESSAGE( "1", messages.at(1) == std::make_pair( LL_error, std::string("abc")));
      CPPUNIT_ASSERT_MESSAGE( "2", messages.at(2) == std::make_pair( LL_debug, std::string("[5,foo]")));
		CPPUNIT_ASSERT_MESSAGE( "3", messages.at(3) == std::make_pair( LL_debug, std::string("222")));
   }
};

CPPUNIT_TEST_SUITE_REGISTRATION(simple_test);
