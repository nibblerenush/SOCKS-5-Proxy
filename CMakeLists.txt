cmake_minimum_required(VERSION 3.0.0)
project(SOCKS-5-Proxy)

set(sources
  src/Exception.cpp
  src/ReplyHandshake.cpp
  src/ReplySocks.cpp
  src/ReplyUnamePasswd.cpp
  src/RequestHandshake.cpp
  src/RequestLogin.cpp
  src/RequestSocks.cpp
  src/RequestUnamePasswd.cpp
  src/Server.cpp
  src/Settings.cpp
  src/main.cpp)

set(headers
  src/Exception.hpp
  src/ReplyHandshake.hpp
  src/ReplySocks.hpp
  src/ReplyUnamePasswd.hpp
  src/RequestHandshake.hpp
  src/RequestLogin.hpp
  src/RequestSocks.hpp
  src/RequestUnamePasswd.hpp
  src/Server.hpp
  src/Settings.hpp
  src/Version.hpp)

set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_LIBS ON)

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
  message("BOOST VERSION: " ${Boost_VERSION_STRING})
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

#target_compile_definitions(${PROJECT_NAME} PRIVATE BOOST_ALL_DYN_LINK)
if(WIN32)
  if(${Boost_VERSION_STRING} STREQUAL "1.71.0")
    target_compile_definitions(${PROJECT_NAME} PRIVATE _WIN32_WINNT=0x0601)
  endif()
endif()
