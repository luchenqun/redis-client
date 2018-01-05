# Redis Client 使用说明

## 功能描述
* 依赖Redis官方提供的C库客户端[hiredis](https://github.com/redis/hiredis)
* 支持单主机(single node mode)与集群模式(cluster mode)
* 支持管道(pipeline)模式
* 使用连接池
* 线程安全
* 自动连接
* 不支持Windows

## 使用示例
* 如果你要运行该main.cpp，请先执行命令 `sudo cp ./lib/linux/libhiedis.a /usr/lib/` 将库拷贝到系统库目录。再执行 `mkdir build && cd build && cmake .. && make && ./redis-client` 即可完成编译运行。
* 如果你要在 `cpp-ethereum` 项目中使用此功能。比如，你想在 `libjujsonrpc` 中使用。那么请在 `libjujsonrpc` 的 `CMakeLists.txt` 中添加头文件 `target_include_directories(jujsonrpc PRIVATE ${HIREDIS_INCLUDE_DIR})` 以及库文件 `target_link_libraries(jujsonrpc ${HIREDIS_LIBRARY})` 。当然，有些可能因为他所包含的文件已经包含了该头文件与库，不需要添加。需要注意的是，因为把Redis作为一项服务，所以已经在文件 `jueth/main.cpp` 文件中已经调用了 `redis->Initialize("127.0.0.1", chainParams.m_redis_port, 2, 100)` 从而执行Redis客户端连接服务端等一些初始化工作。所以你在其他地方再使用Redis做操作的时候，不再需要执行初始化的工作。

## 代码示例
```cpp
#include <string>
#include "RedisClient.hpp"

int main(int argc, char **argv) {
    CRedisClient* redisCli = CRedisClient::Instance();

    if (!redisCli->Initialize("127.0.0.1", 6379, 2, 10))
    {
        std::cout << "connect to redis failed" << std::endl;
        return -1;
    }

    std::string strKey = "name";
    std::string strVal;
    if (redisCli->Get(strKey, &strVal) == RC_SUCCESS)
    {
        std::cout << strKey << " has value " << strVal << std::endl;
        return 0;
    }
    else
    {
        std::cout << "request failed" << std::endl;
        return -1;
    }

    return 0;
}
```