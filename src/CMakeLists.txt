cmake_minimum_required(VERSION 3.0.0)
project(SOCKS-5-Proxy)

set(sources
  Exception.cpp
  ReplyHandshake.cpp
  ReplySocks.cpp
  ReplyUnamePasswd.cpp
  RequestHandshake.cpp
  RequestSocks.cpp
  RequestUnamePasswd.cpp
  Server.cpp
  Settings.cpp
  main.cpp)

set(headers
  Exception.hpp
  ReplyHandshake.hpp
  ReplySocks.hpp
  ReplyUnamePasswd.hpp
  RequestHandshake.hpp
  RequestSocks.hpp
  RequestUnamePasswd.hpp
  Server.hpp
  Settings.hpp
  Version.hpp)

set(Boost_USE_MULTITHREADED ON)

if(CMAKE_BUILD_TYPE MATCHES Debug)
  set(Boost_USE_DEBUG_LIBS ON)
  set(Boost_USE_RELEASE_LIBS OFF)
else()
  set(Boost_USE_DEBUG_LIBS OFF)
  set(Boost_USE_RELEASE_LIBS ON)
endif()

find_package(Boost REQUIRED COMPONENTS system regex date_time log)
find_package(Threads REQUIRED)

if(${Boost_FOUND})
  message("BOOST VERSION: " ${Boost_VERSION})
  message("BOOST INCLUDE DIR: " ${Boost_INCLUDE_DIRS})
  message("BOOST LIBRARIES DIR: " ${Boost_LIBRARY_DIRS})
  message("BOOST LIBRARIES: " ${Boost_LIBRARIES})
endif()

if(${Threads_FOUND})
  message("CMAKE_THREAD_LIBS_INIT: " ${CMAKE_THREAD_LIBS_INIT})
endif()

add_executable(${PROJECT_NAME} ${sources} ${headers})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 14)

target_link_libraries(${PROJECT_NAME}
  PRIVATE
  Boost::system
  Boost::regex
  Boost::date_time
  Boost::log
  Threads::Threads)

target_compile_definitions(${PROJECT_NAME} PRIVATE BOOST_LOG_DYN_LINK=1)
if(WIN32)
  target_compile_definitions(${PROJECT_NAME} PRIVATE _WIN32_WINNT=0x0501)
endif()
