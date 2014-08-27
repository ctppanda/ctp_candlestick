#ifndef EXCHANGE_H
#define EXCHANGE_H

#include "Util.h"
#include "ThostFtdcMdApi.h"
#include "ThostFtdcTraderApi.h"
#include "Poco/BasicEvent.h"
#include "Poco/Delegate.h"
#include "Poco/Thread.h"
#include "Poco/RunnableAdapter.h"

#include "Tick.h"
#include "Order.h"
#include "Account.h"
#include "Position.h"
#include "OrderManager.h"
#include "Serializer.h"

namespace Pheux { namespace Mock {

    using namespace Pheux::Core;

    class Exchange
    {
    public:
        Exchange();
        ~Exchange() {}

        Poco::BasicEvent<Tick*> TickEvent;

        void GenMarketData();

        void Run();

        void OnRspQryTradingAccount(Account* account);

        void OnRspQryInvestorPosition(const string& inst, Position* position);

        void OnRspOrderInsert(Order* order);

        void GenHistoricalData(Serializer* ser);

    private:
        CThostFtdcDepthMarketDataField* tick;

        set<string> instruments;
        set<Tick*> protos;

        CThostFtdcDepthMarketDataField* GenTick();

        Tick* GenRandomTick(Tick* proto);

        // only for one user
        OrderManager om;
    };
}}

#endif
