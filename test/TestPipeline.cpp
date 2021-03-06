#include "TestPipeline.hpp"

CTestPipeline::CTestPipeline()
{
}

bool CTestPipeline::StartTest(const std::string &strHost)
{
    bool bSuccess = false;
    std::cout << "start to test pipeline" << std::endl;

    if(!m_redis.Initialize(strHost, 6379, 2, 10))
    {
        std::cout << "initialize redis client failed" << std::endl;
    }
    else
    {
        Pipeline pipeline = m_redis.CreatePipeline();
        std::string strKey = "name";
        std::string strVal = "lcq";
        std::string strNewVal = "luchenqun";
        long pnVal = 0;
        long nSec = 60;
        m_redis.Set(strKey, strVal, pipeline);
        m_redis.Get(strKey, &strVal, pipeline);
        m_redis.Setex(strKey, nSec, strNewVal, pipeline);
        m_redis.Strlen(strKey, &pnVal, pipeline);
        m_redis.Get(strKey, &strVal, pipeline);

        int ret = m_redis.FlushPipeline(pipeline);
        if(ret == RC_SUCCESS)
        {
            redisReply *pReply;
            while (m_redis.FetchReply(pipeline, &pReply) == RC_SUCCESS) {
                std::cout << "Pipeline Reply type = " << pReply->type
                          << ", integer = " << pReply->integer
                          << ", len = " << pReply->len
                          << ", str = " << (pReply->len > 0 ? pReply->str : "")
                          << ", elements = " << pReply->elements << std::endl;
            }
            m_redis.FreePipeline(pipeline);
            bSuccess = true;
        }
        else
        {
            std::cout << "m_redis.FlushPipeline(pipeline) " << ret;
        }
    }
    std::cout << "CTestPipeline Test " << (bSuccess ? "success" : "fail") << std::endl;
    return bSuccess;
}

