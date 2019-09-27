cmake_minimum_required(VERSION 3.0.0)
project(SOCKS-5-Proxy)

set(sources
Exception.cpp
RequestHandshake.cpp
Request.cpp
Reply.cpp
ReplyHandshake.cpp
Server.cpp
main.cpp
)

# set(sources
#   ErrorPrinter.cpp
#   main.cpp
#   Socks5Exception.cpp
#   Socks5Reply.cpp
#   Socks5ReplyHandshake.cpp
#   Socks5ReplyUsernamePassword.cpp
#   Socks5Request.cpp
#   Socks5RequestHandshake.cpp
#   Socks5RequestUsernamePassword.cpp
#   Socks5Server.cpp
#   Socks5SessionBase.cpp
#   Socks5SessionFactory.cpp
#   Socks5SessionNoAuthentication.cpp
#   Socks5SessionUsernamePassword.cpp
#   Socks5Version.cpp)

set(headers
Exception.hpp
RequestHandshake.hpp
Request.hpp
Reply.hpp
ReplyHandshake.hpp
Version.hpp
Server.hpp)

# set(headers
#   ErrorPrinter.h
#   Socks5Exception.h
#   Socks5Reply.h
#   Socks5ReplyHandshake.h
#   Socks5ReplyUsernamePassword.h
#   Socks5Request.h
#   Socks5RequestHandshake.h
#   Socks5RequestUsernamePassword.h
#   Socks5Server.h
#   Socks5SessionBase.h
#   Socks5SessionFactory.h
#   Socks5SessionNoAuthentication.h
#   Socks5SessionUsernamePassword.h
#   Socks5Version.h)

set(Boost_USE_STATIC_LIBS OFF)
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

if(WIN32)
  add_compile_definitions(_WIN32_WINNT=0x0501)
endif()

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

#target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIRS})
#target_link_libraries(${PROJECT_NAME} PRIVATE ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
