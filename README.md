# Redis Client 使用说明

## 功能描述
* 依赖Redis官方提供的C库客户端[hiredis](https://github.com/redis/hiredis)
* 支持单主机(single node mode)与集群模式(cluster mode)
* 支持管道(pipeline)模式
* 使用连接池
* 线程安全
* 自动连接
* 不支持Windows

## 使用准备
* 如果你要运行该main.cpp，请先执行命令 `sudo cp ./lib/linux/libhiedis.a /usr/lib/` 将库拷贝到系统库目录。再执行 `mkdir build && cd build && cmake .. && make && ./redis-client` 即可完成编译运行。
* 如果你要在 `cpp-ethereum` 项目中使用此功能。比如，你想在 `libjujsonrpc` 中使用。那么请在 `libjujsonrpc` 的 `CMakeLists.txt` 中添加头文件 `target_include_directories(jujsonrpc PRIVATE ${HIREDIS_INCLUDE_DIR})` 以及库文件 `target_link_libraries(jujsonrpc ${HIREDIS_LIBRARY})` 。当然，有些可能因为他所包含的文件已经包含了该头文件与库，不需要添加。还有，不要忘记添加需要使用的头文件`#include "libjuredisclient/RedisClient.h"`。需要注意的是，因为把Redis作为一项服务，所以已经在文件 `jueth/main.cpp` 文件中已经调用了 `redis->Initialize("127.0.0.1", chainParams.m_redis_port, 2, 100)` 从而执行Redis客户端连接服务端等一些初始化工作。所以你在其他地方再使用Redis做操作的时候，只需要使用代码`CRedisClient::Instance()`获取一个指针实例之后就可以直接使用，不再需要执行初始化的工作。当然，如果你需要连接与默认不一样的Redis地址与端口，那么你需要使用`CRedisClient()`构造一个实例，再用此实例进行初始化才能调用Redis的操作。

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

## API 使用说明
先说一下调用接口的一些返回值，在后面文档均用RequestRet描述。   

| Macro definition | Value | Description     |
| :---------------- | :----- | :--------------- |
| RC_RESULT_EOF    | 5     | info            |
| RC_NO_EFFECT     | 4     | info            |
| RC_OBJ_NOT_EXIST | 3     | info            |
| RC_OBJ_EXIST     | 2     | info            |
| RC_PART_SUCCESS  | 1     | info            |
| RC_SUCCESS       | 0     | 调用成功        |
| RC_PARAM_ERR     | -1    | info            |
| RC_REPLY_ERR     | -2    | info            |
| RC_RQST_ERR      | -3    | info            |
| RC_NO_RESOURCE   | -4    | info            |
| RC_PIPELINE_ERR  | -5    | info            |
| RC_NOT_SUPPORT   | -6    | Redis服务不支持 |
| RC_SLOT_CHANGED  | -100  | info            |

### static CRedisClient* Instance()
获取单例指针。

### CRedisClient()
构造函数，构造一个CRedisClient对象。

### bool Initialize(const std::string &strHost, int nPort, int nTimeout, int nConnNum)
初始化链接。

### **[Redis Key 用于管理键](http://www.redis.cn/commands.html#generic)**
### int Del(const std::string &strKey, long *pnVal = nullptr, Pipeline ppLine = nullptr)
[删除键](http://www.redis.cn/commands/del.html)。strKey：键。pnVal：被删除键的个数。ppLine：管道。

### int Dump(const std::string &strKey, std::string *pstrVal, Pipeline ppLine = nullptr)
[序列化给定 key ，并返回被序列化的值](http://www.redis.cn/commands/dump.html)。strKey：键。pstrVal：序列化后的值。ppLine：管道。

### int Exists(const std::string &strKey, long *pnVal, Pipeline ppLine = nullptr)
[key是否存在](http://www.redis.cn/commands/exists.html)。strKey：键。pnVal：若为1则key存在，为0则不存在。ppLine：管道。

### int Expire(const std::string &strKey, long nSec, long *pnVal = nullptr, Pipeline ppLine = nullptr)
[设置key的过期时间，超过时间后，将会自动删除该key](http://www.redis.cn/commands/expire.html)。strKey：键。nSec：过期时间，单位秒。pnVal：1 如果成功设置过期时间，0 如果key不存在或者不能设置过期时间。ppLine：管道。

### int Expireat(const std::string &strKey, long nTime, long *pnVal = nullptr, Pipeline ppLine = nullptr)
[设置key的过期时间，超过时间后，将会自动删除该key](http://www.redis.cn/commands/expireat.html)。参考Expire，不同的是命令接受的时间参数是 UNIX 时间戳 Unix timestamp。

### int Keys(const std::string &strPattern, std::vector<std::string *pvecVal)
[查找所有符合给定模式pattern（正则表达式）的 key](http://www.redis.cn/commands/keys.html)。strPattern：键的正则表达模式。pvecVal：所有符合条件的key。

### int Persist(const std::string &strKey, long *pnVal = nullptr, Pipeline ppLine = nullptr)
[移除给定key的生存时间，将这个 key 从『易失的』(带生存时间 key )转换成『持久的』(一个不带生存时间、永不过期的 key )](http://www.redis.cn/commands/persist.html)。strKey：键。pnVal：当生存时间移除成功时，返回 1。如果 key 不存在或 key 没有设置生存时间，返回 0 。ppLine：管道。

### int Pexpire(const std::string &strKey, long nMilliSec, long *pnVal = nullptr, Pipeline ppLine = nullptr)
[设置key的过期时间，超过时间后，将会自动删除该key](http://www.redis.cn/commands/pexpire.html)。参考Expire，不同的是它以毫秒为单位设置 key 的生存时间。

### int Pexpireat(const std::string &strKey, long nMilliTime, long *pnVal = nullptr, Pipeline ppLine = nullptr)
[设置key的过期时间，超过时间后，将会自动删除该key](http://www.redis.cn/commands/pexpireat.html)。参考Pexpire，不同的是它以毫秒为单位设置 key 的过期 UNIX 时间戳。

### int Pttl(const std::string &strKey, long *pnVal, Pipeline ppLine = nullptr)
[以毫秒为单位返回 key 的剩余生存时间](http://www.redis.cn/commands/pttl.html)。strKey：键。pnVal：如果key不存在返回-2，如果key存在且无过期时间返回-1 。ppLine：管道。

### int Pttl(const std::string &strKey, long *pnVal, Pipeline ppLine = nullptr)
[以毫秒为单位返回 key 的剩余生存时间](http://www.redis.cn/commands/pttl.html)。strKey：键。pnVal：如果key不存在返回-2，如果key存在且无过期时间返回-1 。ppLine：管道。

### int Randomkey(std::string *pstrVal, Pipeline ppLine = nullptr)
[从当前数据库返回一个随机的key](http://www.redis.cn/commands/randomkey.html)。pstrVal：返回的键值。ppLine：管道。

### int Rename(const std::string &strKey, const std::string &strNewKey)
[将key重命名为newkey，如果key与newkey相同，将返回一个错误RC_REPLY_ERR。如果newkey已经存在，则值将被覆盖](http://www.redis.cn/commands/rename.html)。strKey：旧键值。strNewKey：新键值。

### int Renamenx(const std::string &strKey, const std::string &strNewKey)
[当且仅当 newkey 不存在时，将 key 改名为 newkey](http://www.redis.cn/commands/renamenx.html)。strKey：旧键值。strNewKey：新键值。如果修改失败（如当 key 不存在时， newKey已存在），返回RC_REPLY_ERR。

### int Restore(const std::string &strKey, long nTtl, const std::string &strVal, Pipeline ppLine = nullptr)
[反序列化给定的序列化值，并将它和给定的 key 关联](http://www.redis.cn/commands/restore.html)。strKey：旧键值。nTtl：毫秒为单位为 key 设置生存时间。ppLine：管道。


## 接口一览
```C++
/* interfaces for generic */

int Scan(long *pnCursor, const std::string &strPattern, long nCount, std::vector<std::string> *pvecVal)
int Ttl(const std::string &strKey, long *pnVal, Pipeline ppLine = nullptr)
int Type(const std::string &strKey, std::string *pstrVal, Pipeline ppLine = nullptr)

/* interfaces for string */
int Append(const std::string &strKey, const std::string &strVal, long *pnVal = nullptr, Pipeline ppLine = nullptr)
int Bitcount(const std::string &strKey, long *pnVal, Pipeline ppLine = nullptr)
int Bitcount(const std::string &strKey, long nStart, long nEnd, long *pnVal, Pipeline ppLine = nullptr)
int Bitop(const std::string &strDestKey, const std::string &strOp, const std::vector<std::string> &vecKey, long *pnVal = nullptr, Pipeline ppLine = nullptr)
int Bitpos(const std::string &strKey, long nBitVal, long *pnVal, Pipeline ppLine = nullptr)
int Bitpos(const std::string &strKey, long nBitVal, long nStart, long nEnd, long *pnVal, Pipeline ppLine = nullptr)
int Decr(const std::string &strKey, long *pnVal = nullptr, Pipeline ppLine = nullptr)
int Decrby(const std::string &strKey, long nDecr, long *pnVal = nullptr, Pipeline ppLine = nullptr)
int Get(const std::string &strKey, std::string *pstrVal, Pipeline ppLine = nullptr)
int Getbit(const std::string &strKey, long nOffset, long *pnVal, Pipeline ppLine = nullptr)
int Getrange(const std::string &strKey, long nStart, long nEnd, std::string *pstrVal, Pipeline ppLine = nullptr)
int Getset(const std::string &strKey, std::string *pstrVal, Pipeline ppLine = nullptr)
int Incr(const std::string &strKey, long *pnVal, Pipeline ppLine = nullptr)
int Incrby(const std::string &strKey, long nIncr, long *pnVal, Pipeline ppLine = nullptr)
int Incrbyfloat(const std::string &strKey, double dIncr, double *pdVal, Pipeline ppLine = nullptr)
int Mget(const std::vector<std::string> &vecKey, std::vector<std::string> *pvecVal)
int Mset(const std::vector<std::string> &vecKey, const std::vector<std::string> &vecVal)
int Psetex(const std::string &strKey, long nMilliSec, const std::string &strVal, Pipeline ppLine = nullptr)
int Set(const std::string &strKey, const std::string &strVal, Pipeline ppLine = nullptr)
int Setbit(const std::string &strKey, long nOffset, bool bVal, Pipeline ppLine = nullptr)
int Setex(const std::string &strKey, long nSec, const std::string &strVal, Pipeline ppLine = nullptr)
int Setnx(const std::string &strKey, const std::string &strVal, Pipeline ppLine = nullptr)
int Setrange(const std::string &strKey, long nOffset, const std::string &strVal, long *pnVal = nullptr, Pipeline ppLine = nullptr)
int Strlen(const std::string &strKey, long *pnVal, Pipeline ppLine = nullptr)

/* interfaces for list */
int Blpop(const std::string &strKey, long nTimeout, std::vector<std::string> *pvecVal)
int Blpop(const std::vector<std::string> &vecKey, long nTimeout, std::vector<std::string> *pvecVal)
int Brpop(const std::string &strKey, long nTimeout, std::vector<std::string> *pvecVal)
int Brpop(const std::vector<std::string> &vecKey, long nTimeout, std::vector<std::string> *pvecVal)
int Lindex(const std::string &strKey, long nIndex, std::string *pstrVal, Pipeline ppLine = nullptr)
int Linsert(const std::string &strKey, const std::string &strPos, const std::string &strPivot, const std::string &strVal, long *pnVal, Pipeline ppLine = nullptr)
int Llen(const std::string &strKey, long *pnVal, Pipeline ppLine = nullptr)
int Lpop(const std::string &strKey, std::string *pstrVal, Pipeline ppLine = nullptr)
int Lpush(const std::string &strKey, const std::string &strVal, long *pnVal = nullptr, Pipeline ppLine = nullptr)
int Lpush(const std::string &strKey, const std::vector<std::string> &vecVal, Pipeline ppLine = nullptr)
int Lpushx(const std::string &strKey, const std::string &strVal, long *pnVal = nullptr, Pipeline ppLine = nullptr)
int Lrange(const std::string &strKey, long nStart, long nStop, std::vector<std::string> *pvecVal, Pipeline ppLine = nullptr)
int Lrem(const std::string &strKey, long nCount, const std::string &strVal, long *pnVal = nullptr, Pipeline ppLine = nullptr)
int Lset(const std::string &strKey, long nIndex, const std::string &strVal, Pipeline ppLine = nullptr)
int Ltrim(const std::string &strKey, long nStart, long nStop, Pipeline ppLine = nullptr)
int Rpop(const std::string &strKey, std::string *pstrVal, Pipeline ppLine = nullptr)
int Rpush(const std::string &strKey, const std::string &strVal, long *pnVal = nullptr, Pipeline ppLine = nullptr)
int Rpush(const std::string &strKey, const std::vector<std::string> &vecVal, Pipeline ppLine = nullptr)
int Rpushx(const std::string &strKey, const std::string &strVal, long *pnVal = nullptr, Pipeline ppLine = nullptr)

/* interfaces for set */
int Sadd(const std::string &strKey, const std::string &strVal, long *pnVal = nullptr, Pipeline = nullptr)
int Scard(const std::string &strKey, long *pnVal, Pipeline = nullptr)
//int Sdiff(const std::vector<std::string> &vecKey, std::vector<std::string> *pvecVal, Pipeline ppLine = nullptr)
//int Sinter(const std::vector<std::string> &vecKey, std::vector<std::string> *pvecVal, Pipeline ppLine = nullptr)
int Sismember(const std::string &strKey, const std::string &strVal, long *pnVal, Pipeline ppLine = nullptr)
int Smembers(const std::string &strKey, std::vector<std::string> *pvecVal, Pipeline ppLine = nullptr)
int Spop(const std::string &strKey, std::string *pstrVal, Pipeline ppLine = nullptr)
int Srandmember(const std::string &strKey, long nCount, std::vector<std::string> *pvecVal, Pipeline ppLine = nullptr)
int Srem(const std::string &strKey, const std::string &strVal, long *pnVal = nullptr, Pipeline ppLine = nullptr)
int Srem(const std::string &strKey, const std::vector<std::string> &vecVal, long *pnVal = nullptr, Pipeline ppLine = nullptr)
//int Sunion(const std::vector<std::string> &vecKey, std::vector<std::string> *pvecVal, Pipeline ppLine = nullptr)

/* interfaces for hash */
int Hdel(const std::string &strKey, const std::string &strField, long *pnVal = nullptr, Pipeline ppLine = nullptr)
int Hexists(const std::string &strKey, const std::string &strField, long *pnVal, Pipeline ppLine = nullptr)
int Hget(const std::string &strKey, const std::string &strField, std::string *pstrVal, Pipeline ppLine = nullptr)
int Hgetall(const std::string &strKey, std::map<std::string, std::string> *pmapFv, Pipeline ppLine = nullptr)
int Hincrby(const std::string &strKey, const std::string &strField, long nIncr, long *pnVal, Pipeline ppLine = nullptr)
int Hincrbyfloat(const std::string &strKey, const std::string &strField, double dIncr, double *pdVal, Pipeline ppLine = nullptr)
int Hkeys(const std::string &strKey, std::vector<std::string> *pvecVal, Pipeline ppLine = nullptr)
int Hlen(const std::string &strKey, long *pnVal, Pipeline ppLine = nullptr)
int Hmget(const std::string &strKey, const std::vector<std::string> &vecField, std::vector<std::string> *pvecVal, Pipeline ppLine = nullptr)
int Hmget(const std::string &strKey, const std::vector<std::string> &vecField, std::map<std::string, std::string> *pmapVal)
int Hmget(const std::string &strKey, const std::set<std::string> &setField, std::map<std::string, std::string> *pmapVal)
int Hmset(const std::string &strKey, const std::vector<std::string> &vecField, const std::vector<std::string> &vecVal, Pipeline ppLine = nullptr)
int Hmset(const std::string &strKey, const std::map<std::string, std::string> &mapFv, Pipeline ppLine = nullptr)
//int Hscan(const std::string &strKey, long *pnCursor, const std::string &strMatch, long nCount, std::vector<std::string> *pvecVal)
int Hset(const std::string &strKey, const std::string &strField, const std::string &strVal, Pipeline ppLine = nullptr)
int Hsetnx(const std::string &strKey, const std::string &strField, const std::string &strVal, Pipeline ppLine = nullptr)
int Hvals(const std::string &strKey, std::vector<std::string> *pvecVal, Pipeline ppLine = nullptr)

/* interfaces for sorted set */
int Zadd(const std::string &strKey, double dScore, const std::string &strElem, long *pnVal = nullptr, Pipeline = nullptr)
int Zcard(const std::string &strKey, long *pnVal, Pipeline = nullptr)
int Zcount(const std::string &strKey, double dMin, double dMax, long *pnVal, Pipeline ppLine = nullptr)
int Zincrby(const std::string &strKey, double dIncr, const std::string &strElem, double *pdVal, Pipeline ppLine = nullptr)
int Zlexcount(const std::string &strKey, const std::string &strMin, const std::string &strMax, long *pnVal, Pipeline ppLine = nullptr)
int Zrange(const std::string &strKey, long nStart, long nStop, std::vector<std::string> *pvecVal, Pipeline ppLine = nullptr)
int Zrangewithscore(const std::string &strKey, long nStart, long nStop, std::map<std::string, std::string> *pmapVal, Pipeline ppLine = nullptr)
int Zrangebylex(const std::string &strKey, const std::string &strMin, const std::string &strMax, std::vector<std::string> *pvecVal, Pipeline ppLine = nullptr)
int Zrangebyscore(const std::string &strKey, double dMin, double dMax, std::vector<std::string> *pvecVal, Pipeline ppLine = nullptr)
int Zrangebyscore(const std::string &strKey, double dMin, double dMax, std::map<std::string, double> *pmapVal, Pipeline ppLine = nullptr)
int Zrank(const std::string &strKey, const std::string &strElem, long *pnVal, Pipeline ppLine = nullptr)
int Zrem(const std::string &strKey, const std::string &strElem, long *pnVal = nullptr, Pipeline ppLine = nullptr)
int Zrem(const std::string &strKey, const std::vector<std::string> &vecElem, long *pnVal = nullptr, Pipeline ppLine = nullptr)
int Zremrangebylex(const std::string &strKey, const std::string &strMin, const std::string &strMax, long *pnVal = nullptr, Pipeline ppLine = nullptr)
int Zremrangebyrank(const std::string &strKey, long nStart, long nStop, long *pnVal = nullptr, Pipeline ppLine = nullptr)
int Zremrangebyscore(const std::string &strKey, double dMin, double dMax, long *pnVal = nullptr, Pipeline ppLine = nullptr)
int Zrevrange(const std::string &strKey, long nStart, long nStop, std::vector<std::string> *pvecVal, Pipeline ppLine = nullptr)
int Zrevrangebyscore(const std::string &strKey, double dMax, double dMin, std::vector<std::string> *pvecVal, Pipeline ppLine = nullptr)
int Zrevrangebyscore(const std::string &strKey, double dMax, double dMin, std::map<std::string, double> *pmapVal, Pipeline ppLine = nullptr)
int Zrevrank(const std::string &strKey, const std::string &strElem, long *pnVal, Pipeline ppLine = nullptr)
int Zscore(const std::string &strKey, const std::string &strElem, double *pdVal, Pipeline ppLine = nullptr)

/* interfaces for system */
int Time(struct timeval *ptmVal, Pipeline ppLine = nullptr)

Pipeline CreatePipeline()
int FlushPipeline(Pipeline ppLine)
int FetchReply(Pipeline ppLine, long *pnVal)
int FetchReply(Pipeline ppLine, std::string *pstrVal)
int FetchReply(Pipeline ppLine, std::vector<long> *pvecLongVal)
int FetchReply(Pipeline ppLine, std::vector<std::string> *pvecStrVal)
int FetchReply(Pipeline ppLine, redisReply **pReply)
void FreePipeline(Pipeline ppLine)
```

