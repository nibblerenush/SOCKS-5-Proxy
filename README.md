# SOCKS-5-Proxy
This is the simple socks5 proxy using [boost asio stackless coroutines](https://www.boost.org/doc/libs/1_71_0/doc/html/boost_asio/overview/core/coroutine.html).

## Dependencies
* boost >= 1.69
* openssl >= 1.0.2 (in future)

## Build
```cmake -DBOOST_INCLUDEDIR=<includedir> -DBOOST_LIBRARYDIR=<librarydir>```

## Launch
```./SOCKS-5-Proxy <configuration file>```
