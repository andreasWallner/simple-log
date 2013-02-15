include_directories( ${CMAKE_SOURCE_DIR} )

message("Building tests:")

file(GLOB log_test_src RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*_test.cpp")

add_executable(LogUnitTester test_runner.cpp ${log_test_src})
target_link_libraries(LogUnitTester log cppunit)

foreach(TB ${log_test_src})
   get_filename_component(TestName ${TB} NAME_WE)
   message("* ${TestName}")
   add_test(${TestName} LogUnitTester ${TestName})
endforeach()
