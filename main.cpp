#include <string>
#include <thread>
#include "test/TestBase.hpp"
#include "test/TestGeneric.hpp"
#include "test/TestString.hpp"
#include "test/TestList.hpp"
#include "test/TestSet.hpp"
#include "test/TestHash.hpp"
#include "test/TestZset.hpp"
#include "test/TestConcur.hpp"
#include "test/TestPipeline.hpp"

using namespace std;

int main(int argc, char **argv) {
    const std::string strHost = "127.0.0.1";

    do{
        cout << "=============CTestPipeline=============" << endl;
        CTestPipeline testPipeline;
        if (!testPipeline.StartTest(strHost))
            break;

        cout << "=============CTestBase=============" << endl;
        CTestBase testBase;
        if (!testBase.StartTest(strHost))
            break;

        cout << "=============ThreadTest=============" << endl;
        cout << "多线程测试，在TestBase里面只开了10个连接，用20个线程同时去读Redis数据" << endl;
        CRedisClient* r = testBase.getRC();
        std::string strKey = "name";
        std::string strVal = "luchenqun";
        cout << "Set ret = " << r->Set(strKey, strVal) << endl;
        std::vector<std::thread> tv;
        for(int j = 1; j <= 20 ; ++j) {
            std::thread t([&r, &strKey, &strVal]{
                strVal = "";
                int ret = r->Get(strKey, &strVal);
                std::cout << "thread id = " << std::this_thread::get_id() << ", request ret = " << ret << ", val = " << strVal << std::endl;
            });
            tv.push_back(std::move(t));
        }
        for(auto &thread : tv) {
            thread.join();
        }

        cout << "=============CTestGeneric=============" << endl;
        CTestGeneric testKeys;
        if (!testKeys.StartTest(strHost))
            break;

        cout << "=============CTestString=============" << endl;
        CTestString testStr;
        if (!testStr.StartTest(strHost))
            break;

        cout << "=============CTestList=============" << endl;
        CTestList testList;
        if (!testList.StartTest(strHost))
            break;

        cout << "=============CTestSet=============" << endl;
        CTestSet testSet;
        if (!testSet.StartTest(strHost))
            break;

        cout << "=============CTestHash=============" << endl;
        CTestHash testHash;
        if (!testHash.StartTest(strHost))
            break;

        cout << "=============CTestZset=============" << endl;
        CTestZset testZset;
        if (!testZset.StartTest(strHost))
            break;

        cout << "=============CTestConcur=============" << endl;
        CTestConcur testConcur;
        if (!testConcur.StartTest(strHost))
            break;
    }while(false);

    cout << "test end" << endl;

    return 0;
}
