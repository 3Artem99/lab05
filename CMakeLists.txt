cmake_minimum_required(VERSION 3.4)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS "Build tests" OFF)

if(BUILD_TESTS)
  add_compile_options(--coverage)

  # Add Google Test and Google Mock subdirectories
  add_subdirectory(third-party/googletest)
  add_subdirectory(third-party/googlemock)

  # Set include directories for Google Test and Google Mock
  include_directories(third-party/googletest/include)
  include_directories(third-party/googlemock/include)

  # Add banking library
  project(banking)
  add_library(banking STATIC ${CMAKE_CURRENT_SOURCE_DIR}/banking/Transaction.cpp ${CMAKE_CURRENT_SOURCE_DIR}/banking/Account.cpp)
  target_include_directories(banking PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/banking)
  target_link_libraries(banking gcov)

  # Add mock-objects for Account and Transaction classes
  add_library(mock_account STATIC ${CMAKE_CURRENT_SOURCE_DIR}/tests/test1.cpp)
  target_include_directories(mock_account PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/tests)
  target_link_libraries(mock_account gmock_main)

  add_library(mock_transaction STATIC ${CMAKE_CURRENT_SOURCE_DIR}/tests/test2.cpp)
  target_include_directories(mock_transaction PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/tests)
  target_link_libraries(mock_transaction gmock_main)

  # Add tests for Account class
  add_executable(test_account ${CMAKE_CURRENT_SOURCE_DIR}/tests/test1.cpp)
  target_link_libraries(test_account banking mock_account gtest_main)
  add_test(NAME test_account COMMAND test_account)

  # Add tests for Transaction class
  add_executable(test_transaction ${CMAKE_CURRENT_SOURCE_DIR}/tests/test2.cpp)
  target_link_libraries(test_transaction banking mock_account mock_transaction gtest_main)
  add_test(NAME test_transaction COMMAND test_transaction)
endif()
