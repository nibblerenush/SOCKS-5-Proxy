cmake_minimum_required(VERSION 3.0.0)

project(SOCKS-5-Proxy)

set(sources
  ErrorPrinter.cpp
  main.cpp
  Socks5Exception.cpp
  Socks5Reply.cpp
  Socks5ReplyHandshake.cpp
  Socks5Request.cpp
  Socks5RequestHandshake.cpp
  Socks5Server.cpp
  Socks5SessionBase.cpp
  Socks5SessionNoAuthentication.cpp
  Socks5Version.cpp
)

set(headers
  ErrorPrinter.h
  Socks5Exception.h
  Socks5Reply.h
  Socks5ReplyHandshake.h
  Socks5Request.h
  Socks5RequestHandshake.h
  Socks5Server.h
  Socks5SessionBase.h
  Socks5SessionNoAuthentication.h
  Socks5Version.h
)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)

if(CMAKE_BUILD_TYPE MATCHES Debug)
  set(Boost_USE_DEBUG_LIBS ON)
  set(Boost_USE_RELEASE_LIBS OFF)
else()
  set(Boost_USE_DEBUG_LIBS OFF)
  set(Boost_USE_RELEASE_LIBS ON)
endif()

find_package(Boost REQUIRED COMPONENTS system regex date_time)

if(WIN32)
  add_compile_definitions(_WIN32_WINNT=0x0501)
endif()

if(${Boost_FOUND})
  message("BOOST VERSION: " ${Boost_VERSION})
  message("BOOST INCLUDE DIR: " ${Boost_INCLUDE_DIRS})
  message("BOOST LIBRARIES DIR: " ${Boost_LIBRARY_DIRS})
  message("BOOST LIBRARIES: " ${Boost_LIBRARIES})
endif()

add_executable(${PROJECT_NAME} ${sources} ${headers})
target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${Boost_LIBRARIES})
