#ifndef TEST_PIPELINE_H
#define TEST_PIPELINE_H
#include "TestClient.hpp"

class CTestPipeline : public CTestClient
{
public:
    CTestPipeline();
    bool StartTest(const std::string &strHost);
};

#endif
