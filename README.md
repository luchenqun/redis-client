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
更多测试用例，请看[test](test/)目录。

## 接口一览
static CRedisClient* Instance()  
CRedisClient()  
// nTimeout: 连接超时时间，单位秒。nConnNum：连接池数目   
bool Initialize(const std::string &strHost, int nPort, int nTimeout, int nConnNum)  
bool IsCluster()  

Pipeline CreatePipeline()  
int FlushPipeline(Pipeline ppLine)  
int FetchReply(Pipeline ppLine, long *pnVal)  
int FetchReply(Pipeline ppLine, std::string *pstrVal)  
int FetchReply(Pipeline ppLine, std::vector\<long\> *pvecLongVal)  
int FetchReply(Pipeline ppLine, std::vector\<std::string\> *pvecStrVal)  
int FetchReply(Pipeline ppLine, redisReply **pReply)  
void FreePipeline(Pipeline ppLine)  

/* interfaces for generic */  
int [Del](http://www.redis.cn/commands/del.html)(const std::string &strKey, long *pnVal = nullptr, Pipeline ppLine = nullptr)  
int [Dump](http://www.redis.cn/commands/dump.html)(const std::string &strKey, std::string *pstrVal, Pipeline ppLine = nullptr)  
int [Exists](http://www.redis.cn/commands/exists.html)(const std::string &strKey, long *pnVal, Pipeline ppLine = nullptr)  
int [Expire](http://www.redis.cn/commands/expire.html)(const std::string &strKey, long nSec, long *pnVal = nullptr, Pipeline ppLine = nullptr)  
int [Expireat](http://www.redis.cn/commands/expireat.html)(const std::string &strKey, long nTime, long *pnVal = nullptr, Pipeline ppLine = nullptr)  
int [Keys](http://www.redis.cn/commands/keys.html)(const std::string &strPattern, std::vector\<std::string\> *pvecVal)  
int [Persist](http://www.redis.cn/commands/persist.html)(const std::string &strKey, long *pnVal = nullptr, Pipeline ppLine = nullptr)  
int [Pexpire](http://www.redis.cn/commands/pexpire.html)(const std::string &strKey, long nMilliSec, long *pnVal = nullptr, Pipeline ppLine = nullptr)  
int [Pexpireat](http://www.redis.cn/commands/pexpireat.html)(const std::string &strKey, long nMilliTime, long *pnVal = nullptr, Pipeline ppLine = nullptr)  
int [Pttl](http://www.redis.cn/commands/pttl.html)(const std::string &strKey, long *pnVal, Pipeline ppLine = nullptr)  
int [Randomkey](http://www.redis.cn/commands/randomkey.html)(std::string *pstrVal, Pipeline ppLine = nullptr)  
int [Rename](http://www.redis.cn/commands/rename.html)(const std::string &strKey, const std::string &strNewKey)  
int [Renamenx](http://www.redis.cn/commands/renamenx.html)(const std::string &strKey, const std::string &strNewKey)  
int [Restore](http://www.redis.cn/commands/restore.html)(const std::string &strKey, long nTtl, const std::string &strVal, Pipeline ppLine = nullptr)  
int [Scan](http://www.redis.cn/commands/scan.html)(long *pnCursor, const std::string &strPattern, long nCount, std::vector\<std::string\> *pvecVal)  
int [Ttl](http://www.redis.cn/commands/ttl.html)(const std::string &strKey, long *pnVal, Pipeline ppLine = nullptr)  
int [Type](http://www.redis.cn/commands/type.html)(const std::string &strKey, std::string *pstrVal, Pipeline ppLine = nullptr)  
  
/* interfaces for string */  
int [Append](http://www.redis.cn/commands/append.html)(const std::string &strKey, const std::string &strVal, long *pnVal = nullptr, Pipeline ppLine = nullptr)  
int [Bitcount](http://www.redis.cn/commands/bitcount.html)(const std::string &strKey, long *pnVal, Pipeline ppLine = nullptr)  
int [Bitcount](http://www.redis.cn/commands/bitcount.html)(const std::string &strKey, long nStart, long nEnd, long *pnVal, Pipeline ppLine = nullptr)  
int [Bitop](http://www.redis.cn/commands/bitop.html)(const std::string &strDestKey, const std::string &strOp, const std::vector\<std::string\> &vecKey, long *pnVal = nullptr, Pipeline ppLine = nullptr)  
int [Bitpos](http://www.redis.cn/commands/bitpos.html)(const std::string &strKey, long nBitVal, long *pnVal, Pipeline ppLine = nullptr)  
int [Bitpos](http://www.redis.cn/commands/bitpos.html)(const std::string &strKey, long nBitVal, long nStart, long nEnd, long *pnVal, Pipeline ppLine = nullptr)  
int [Decr](http://www.redis.cn/commands/decr.html)(const std::string &strKey, long *pnVal = nullptr, Pipeline ppLine = nullptr)  
int [Decrby](http://www.redis.cn/commands/decrby.html)(const std::string &strKey, long nDecr, long *pnVal = nullptr, Pipeline ppLine = nullptr)  
int [Get](http://www.redis.cn/commands/get.html)(const std::string &strKey, std::string *pstrVal, Pipeline ppLine = nullptr)  
int [Getbit](http://www.redis.cn/commands/getbit.html)(const std::string &strKey, long nOffset, long *pnVal, Pipeline ppLine = nullptr)  
int [Getrange](http://www.redis.cn/commands/getrange.html)(const std::string &strKey, long nStart, long nEnd, std::string *pstrVal, Pipeline ppLine = nullptr)  
int [Getset](http://www.redis.cn/commands/getset.html)(const std::string &strKey, std::string *pstrVal, Pipeline ppLine = nullptr)  
int [Incr](http://www.redis.cn/commands/incr.html)(const std::string &strKey, long *pnVal, Pipeline ppLine = nullptr)  
int [Incrby](http://www.redis.cn/commands/incrby.html)(const std::string &strKey, long nIncr, long *pnVal, Pipeline ppLine = nullptr)  
int [Incrbyfloat](http://www.redis.cn/commands/incrbyfloat.html)(const std::string &strKey, double dIncr, double *pdVal, Pipeline ppLine = nullptr)  
int [Mget](http://www.redis.cn/commands/mget.html)(const std::vector\<std::string\> &vecKey, std::vector\<std::string\> *pvecVal)  
int [Mset](http://www.redis.cn/commands/mset.html)(const std::vector\<std::string\> &vecKey, const std::vector\<std::string\> &vecVal)  
int [Psetex](http://www.redis.cn/commands/psetex.html)(const std::string &strKey, long nMilliSec, const std::string &strVal, Pipeline ppLine = nullptr)  
int [Set](http://www.redis.cn/commands/set.html)(const std::string &strKey, const std::string &strVal, Pipeline ppLine = nullptr)  
int [Setbit](http://www.redis.cn/commands/setbit.html)(const std::string &strKey, long nOffset, bool bVal, Pipeline ppLine = nullptr)  
int [Setex](http://www.redis.cn/commands/setex.html)(const std::string &strKey, long nSec, const std::string &strVal, Pipeline ppLine = nullptr)  
int [Setnx](http://www.redis.cn/commands/setnx.html)(const std::string &strKey, const std::string &strVal, Pipeline ppLine = nullptr)  
int [Setrange](http://www.redis.cn/commands/setrange.html)(const std::string &strKey, long nOffset, const std::string &strVal, long *pnVal = nullptr, Pipeline ppLine = nullptr)  
int [Strlen](http://www.redis.cn/commands/strlen.html)(const std::string &strKey, long *pnVal, Pipeline ppLine = nullptr)  
  
/* interfaces for list */  
int [Blpop](http://www.redis.cn/commands/blpop.html)(const std::string &strKey, long nTimeout, std::vector\<std::string\> *pvecVal)  
int [Blpop](http://www.redis.cn/commands/blpop.html)(const std::vector\<std::string\> &vecKey, long nTimeout, std::vector\<std::string\> *pvecVal)  
int [Brpop](http://www.redis.cn/commands/brpop.html)(const std::string &strKey, long nTimeout, std::vector\<std::string\> *pvecVal)  
int [Brpop](http://www.redis.cn/commands/brpop.html)(const std::vector\<std::string\> &vecKey, long nTimeout, std::vector\<std::string\> *pvecVal)  
int [Lindex](http://www.redis.cn/commands/lindex.html)(const std::string &strKey, long nIndex, std::string *pstrVal, Pipeline ppLine = nullptr)  
int [Linsert](http://www.redis.cn/commands/linsert.html)(const std::string &strKey, const std::string &strPos, const std::string &strPivot, const std::string &strVal, long *pnVal, Pipeline ppLine = nullptr)  
int [Llen](http://www.redis.cn/commands/llen.html)(const std::string &strKey, long *pnVal, Pipeline ppLine = nullptr)  
int [Lpop](http://www.redis.cn/commands/lpop.html)(const std::string &strKey, std::string *pstrVal, Pipeline ppLine = nullptr)  
int [Lpush](http://www.redis.cn/commands/lpush.html)(const std::string &strKey, const std::string &strVal, long *pnVal = nullptr, Pipeline ppLine = nullptr)  
int [Lpush](http://www.redis.cn/commands/lpush.html)(const std::string &strKey, const std::vector\<std::string\> &vecVal, Pipeline ppLine = nullptr)  
int [Lpushx](http://www.redis.cn/commands/lpushx.html)(const std::string &strKey, const std::string &strVal, long *pnVal = nullptr, Pipeline ppLine = nullptr)  
int [Lrange](http://www.redis.cn/commands/lrange.html)(const std::string &strKey, long nStart, long nStop, std::vector\<std::string\> *pvecVal, Pipeline ppLine = nullptr)  
int [Lrem](http://www.redis.cn/commands/lrem.html)(const std::string &strKey, long nCount, const std::string &strVal, long *pnVal = nullptr, Pipeline ppLine = nullptr)  
int [Lset](http://www.redis.cn/commands/lset.html)(const std::string &strKey, long nIndex, const std::string &strVal, Pipeline ppLine = nullptr)  
int [Ltrim](http://www.redis.cn/commands/ltrim.html)(const std::string &strKey, long nStart, long nStop, Pipeline ppLine = nullptr)  
int [Rpop](http://www.redis.cn/commands/rpop.html)(const std::string &strKey, std::string *pstrVal, Pipeline ppLine = nullptr)  
int [Rpush](http://www.redis.cn/commands/rpush.html)(const std::string &strKey, const std::string &strVal, long *pnVal = nullptr, Pipeline ppLine = nullptr)  
int [Rpush](http://www.redis.cn/commands/rpush.html)(const std::string &strKey, const std::vector\<std::string\> &vecVal, Pipeline ppLine = nullptr)  
int [Rpushx](http://www.redis.cn/commands/rpushx.html)(const std::string &strKey, const std::string &strVal, long *pnVal = nullptr, Pipeline ppLine = nullptr)  
  
/* interfaces for set */  
int [Sadd](http://www.redis.cn/commands/sadd.html)(const std::string &strKey, const std::string &strVal, long *pnVal = nullptr, Pipeline = nullptr)  
int [Scard](http://www.redis.cn/commands/scard.html)(const std::string &strKey, long *pnVal, Pipeline = nullptr)  
//int Sdiff(const std::vector\<std::string\> &vecKey, std::vector\<std::string\> *pvecVal, Pipeline ppLine = nullptr);  
//int Sinter(const std::vector\<std::string\> &vecKey, std::vector\<std::string\> *pvecVal, Pipeline ppLine = nullptr);  
int [Sismember](http://www.redis.cn/commands/sismember.html)(const std::string &strKey, const std::string &strVal, long *pnVal, Pipeline ppLine = nullptr)  
int [Smembers](http://www.redis.cn/commands/smembers.html)(const std::string &strKey, std::vector\<std::string\> *pvecVal, Pipeline ppLine = nullptr)  
int [Spop](http://www.redis.cn/commands/spop.html)(const std::string &strKey, std::string *pstrVal, Pipeline ppLine = nullptr)  
int [Srandmember](http://www.redis.cn/commands/srandmember.html)(const std::string &strKey, long nCount, std::vector\<std::string\> *pvecVal, Pipeline ppLine = nullptr)  
int [Srem](http://www.redis.cn/commands/srem.html)(const std::string &strKey, const std::string &strVal, long *pnVal = nullptr, Pipeline ppLine = nullptr)  
int [Srem](http://www.redis.cn/commands/srem.html)(const std::string &strKey, const std::vector\<std::string\> &vecVal, long *pnVal = nullptr, Pipeline ppLine = nullptr)  
//int Sunion(const std::vector\<std::string\> &vecKey, std::vector\<std::string\> *pvecVal, Pipeline ppLine = nullptr);  
  
/* interfaces for hash */  
int [Hdel](http://www.redis.cn/commands/hdel.html)(const std::string &strKey, const std::string &strField, long *pnVal = nullptr, Pipeline ppLine = nullptr)  
int [Hexists](http://www.redis.cn/commands/hexists.html)(const std::string &strKey, const std::string &strField, long *pnVal, Pipeline ppLine = nullptr)  
int [Hget](http://www.redis.cn/commands/hget.html)(const std::string &strKey, const std::string &strField, std::string *pstrVal, Pipeline ppLine = nullptr)  
int [Hgetall](http://www.redis.cn/commands/hgetall.html)(const std::string &strKey, std::map<std::string, std::string> *pmapFv, Pipeline ppLine = nullptr)  
int [Hincrby](http://www.redis.cn/commands/hincrby.html)(const std::string &strKey, const std::string &strField, long nIncr, long *pnVal, Pipeline ppLine = nullptr)  
int [Hincrbyfloat](http://www.redis.cn/commands/hincrbyfloat.html)(const std::string &strKey, const std::string &strField, double dIncr, double *pdVal, Pipeline ppLine = nullptr)  
int [Hkeys](http://www.redis.cn/commands/hkeys.html)(const std::string &strKey, std::vector\<std::string\> *pvecVal, Pipeline ppLine = nullptr)  
int [Hlen](http://www.redis.cn/commands/hlen.html)(const std::string &strKey, long *pnVal, Pipeline ppLine = nullptr)  
int [Hmget](http://www.redis.cn/commands/hmget.html)(const std::string &strKey, const std::vector\<std::string\> &vecField, std::vector\<std::string\> *pvecVal, Pipeline ppLine = nullptr)  
int [Hmget](http://www.redis.cn/commands/hmget.html)(const std::string &strKey, const std::vector\<std::string\> &vecField, std::map<std::string, std::string> *pmapVal)  
int [Hmget](http://www.redis.cn/commands/hmget.html)(const std::string &strKey, const std::set\<std::string\> &setField, std::map<std::string, std::string> *pmapVal)  
int [Hmset](http://www.redis.cn/commands/hmset.html)(const std::string &strKey, const std::vector\<std::string\> &vecField, const std::vector\<std::string\> &vecVal, Pipeline ppLine = nullptr)  
int [Hmset](http://www.redis.cn/commands/hmset.html)(const std::string &strKey, const std::map<std::string, std::string> &mapFv, Pipeline ppLine = nullptr)  
//int Hscan(const std::string &strKey, long *pnCursor, const std::string &strMatch, long nCount, std::vector\<std::string\> *pvecVal);  
int [Hset](http://www.redis.cn/commands/hset.html)(const std::string &strKey, const std::string &strField, const std::string &strVal, Pipeline ppLine = nullptr)  
int [Hsetnx](http://www.redis.cn/commands/hsetnx.html)(const std::string &strKey, const std::string &strField, const std::string &strVal, Pipeline ppLine = nullptr)  
int [Hvals](http://www.redis.cn/commands/hvals.html)(const std::string &strKey, std::vector\<std::string\> *pvecVal, Pipeline ppLine = nullptr)  
  
/* interfaces for sorted set */  
int [Zadd](http://www.redis.cn/commands/zadd.html)(const std::string &strKey, double dScore, const std::string &strElem, long *pnVal = nullptr, Pipeline = nullptr)  
int [Zcard](http://www.redis.cn/commands/zcard.html)(const std::string &strKey, long *pnVal, Pipeline = nullptr)  
int [Zcount](http://www.redis.cn/commands/zcount.html)(const std::string &strKey, double dMin, double dMax, long *pnVal, Pipeline ppLine = nullptr)  
int [Zincrby](http://www.redis.cn/commands/zincrby.html)(const std::string &strKey, double dIncr, const std::string &strElem, double *pdVal, Pipeline ppLine = nullptr)  
int [Zlexcount](http://www.redis.cn/commands/zlexcount.html)(const std::string &strKey, const std::string &strMin, const std::string &strMax, long *pnVal, Pipeline ppLine = nullptr)  
int [Zrange](http://www.redis.cn/commands/zrange.html)(const std::string &strKey, long nStart, long nStop, std::vector\<std::string\> *pvecVal, Pipeline ppLine = nullptr)  
int [Zrangewithscore](http://www.redis.cn/commands/zrangewithscore.html)(const std::string &strKey, long nStart, long nStop, std::map<std::string, std::string> *pmapVal, Pipeline ppLine = nullptr)  
int [Zrangebylex](http://www.redis.cn/commands/zrangebylex.html)(const std::string &strKey, const std::string &strMin, const std::string &strMax, std::vector\<std::string\> *pvecVal, Pipeline ppLine = nullptr)  
int [Zrangebyscore](http://www.redis.cn/commands/zrangebyscore.html)(const std::string &strKey, double dMin, double dMax, std::vector\<std::string\> *pvecVal, Pipeline ppLine = nullptr)  
int [Zrangebyscore](http://www.redis.cn/commands/zrangebyscore.html)(const std::string &strKey, double dMin, double dMax, std::map<std::string, double> *pmapVal, Pipeline ppLine = nullptr)  
int [Zrank](http://www.redis.cn/commands/zrank.html)(const std::string &strKey, const std::string &strElem, long *pnVal, Pipeline ppLine = nullptr)  
int [Zrem](http://www.redis.cn/commands/zrem.html)(const std::string &strKey, const std::string &strElem, long *pnVal = nullptr, Pipeline ppLine = nullptr)  
int [Zrem](http://www.redis.cn/commands/zrem.html)(const std::string &strKey, const std::vector\<std::string\> &vecElem, long *pnVal = nullptr, Pipeline ppLine = nullptr)  
int [Zremrangebylex](http://www.redis.cn/commands/zremrangebylex.html)(const std::string &strKey, const std::string &strMin, const std::string &strMax, long *pnVal = nullptr, Pipeline ppLine = nullptr)  
int [Zremrangebyrank](http://www.redis.cn/commands/zremrangebyrank.html)(const std::string &strKey, long nStart, long nStop, long *pnVal = nullptr, Pipeline ppLine = nullptr)  
int [Zremrangebyscore](http://www.redis.cn/commands/zremrangebyscore.html)(const std::string &strKey, double dMin, double dMax, long *pnVal = nullptr, Pipeline ppLine = nullptr)  
int [Zrevrange](http://www.redis.cn/commands/zrevrange.html)(const std::string &strKey, long nStart, long nStop, std::vector\<std::string\> *pvecVal, Pipeline ppLine = nullptr)  
int [Zrevrangebyscore](http://www.redis.cn/commands/zrevrangebyscore.html)(const std::string &strKey, double dMax, double dMin, std::vector\<std::string\> *pvecVal, Pipeline ppLine = nullptr)  
int [Zrevrangebyscore](http://www.redis.cn/commands/zrevrangebyscore.html)(const std::string &strKey, double dMax, double dMin, std::map<std::string, double> *pmapVal, Pipeline ppLine = nullptr)  
int [Zrevrank](http://www.redis.cn/commands/zrevrank.html)(const std::string &strKey, const std::string &strElem, long *pnVal, Pipeline ppLine = nullptr)  
int [Zscore](http://www.redis.cn/commands/zscore.html)(const std::string &strKey, const std::string &strElem, double *pdVal, Pipeline ppLine = nullptr)  
  
/* interfaces for system */  
int [Time](http://www.redis.cn/commands/time.html)(struct timeval *ptmVal, Pipeline ppLine = nullptr)  

## API 使用详细说明
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

------------------------------------------------------------------------

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

### int Scan(long *pnCursor, const std::string &strPattern, long nCount, std::vector\<std::string\> *pvecVal)
[迭代当前数据库中的key集合](http://www.redis.cn/commands/scan.html)。pnCursor：游标。strPattern：正则表达式。nCount：数量。pvecVal：值。

### int Ttl(const std::string &strKey, long *pnVal, Pipeline ppLine = nullptr)
[以秒为单位返回 key 的剩余生存时间](http://www.redis.cn/commands/ttl.html)。strKey：键。pnVal：如果key不存在返回-2，如果key存在且无过期时间返回-1 。ppLine：管道。

### int Type(const std::string &strKey, std::string *pstrVal, Pipeline ppLine = nullptr)
[返回key所存储的value的数据结构类型，它可以返回string, list, set, zset 和 hash等不同的类型](http://www.redis.cn/commands/type.html)。strKey：键。pstrVal：类型，不存在返回none。ppLine：管道。

------------------------------------------------------------------------

### **[Redis String 字符串](http://www.redis.cn/commands.html#string)**


