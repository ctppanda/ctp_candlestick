#include <iostream>
#include "ThostFtdcMdApi.h"
#include "MdSpi.h"
#include "Simulator.h"
#include "TraderAgent.h"
#include "Poco/Thread.h"
#include "Util.h"
#include "Config.h"
#include "ConnectionPool.h"






using namespace std;
using namespace Pheux::Core;


CThostFtdcMdApi* pUserApi;
int iRequestID = 0;



int main(void)
{
    logger = CreateFileLogger("test.log", "Log1");

    // 1, for functional testing, including the data persistence
    //Simulator::Instance()->FunctionalTest();

    // 2, for real-time mock through internal tick generator
	
    Simulator::Instance()->RealTimeMock();

    // 3, for back-testing from historical data
    //Simulator::Instance()->BackTesting2();

    // 4, test trader agent
    //TraderAgent agent;
    //agent.Init();
    //Poco::Thread::sleep(2000);
    //agent.GetSettlementInfo();
    //Poco::Thread::sleep(2000);
    //agent.GetAccountInfo(false);
    //Poco::Thread::sleep(2000);
    //agent.GetPositionInfo("cu1305", false);
    //Poco::Thread::sleep(2000);
    //Order* o = agent.SendOrder("cu1305", Order::Buy, 1, 54950);
    ////Poco::Thread::sleep(5000);
    //while(1);

    // 5, for real-time mock through MdApi
	//Config::Instance()->parse_xml();
	//ConnPool::GetInstance();
	//getchar();
    //Simulator::Instance()->RealTimeMock2();
    return 0;
}
