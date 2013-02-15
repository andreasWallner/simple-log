#include <cppunit/extensions/HelperMacros.h>
#include <Log.h>

#include <utility>
#include <string>

struct test {
   int i;
   std::string s;
};

LogWriter&& operator<<( LogWriter&& out, const test& t);
LogWriter&& operator<<( LogWriter&& out, const test& t) {
   out << "[" << t.i << "," << t.s << "]";
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
      logger.message(LL_error) << "a" << "b" << "c";
      logger.message(LL_debug) << "c";

      // verify
      auto messages = loggedMessages->getMessages();
      CPPUNIT_ASSERT( messages.size() == 2);
      CPPUNIT_ASSERT( messages.at(0) == std::make_pair( LL_error, std::string("abc")));
      CPPUNIT_ASSERT( messages.at(1) == std::make_pair( LL_debug, std::string("c")));
   }
};

CPPUNIT_TEST_SUITE_REGISTRATION(simple_test);
