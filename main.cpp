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
