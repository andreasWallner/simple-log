#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

int main( int argc, char* argv[]) {
   CppUnit::TestFactoryRegistry& registry = CppUnit::TestFactoryRegistry::getRegistry();
   CppUnit::Test* test_to_run = registry.makeTest();
   CppUnit::TextUi::TestRunner runner;

   if(argc == 2)
      test_to_run = test_to_run->findTest(argv[1]);
	else if (argc > 2)
		return 1;

   runner.addTest(test_to_run);
   return !runner.run();
}
