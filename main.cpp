#include <string>
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
