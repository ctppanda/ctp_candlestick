#include <random>
#include "Exchange.h"

namespace Pheux { namespace Mock {

    template<typename T>
    T RandomWalk(T val)
    {
        // global variable to avoid fake random
        static std::default_random_engine eng;
        static std::normal_distribution<T> dist(val, val / 10);
        return dist(eng);
    }

    Exchange::Exchange()
    {
        instruments.insert("cu1305");
        instruments.insert("if9999");

        for (set<string>::iterator it = instruments.begin(); it != instruments.end(); it++)
        {
            Tick* proto = new Tick();
            proto->InstrumentID = *it;
            proto->LastPrice = 48830.0;
            proto->Volume = 200;
        }

        string inst = "ru1409";

        // init tick
        Poco::DateTime now;
        tick = new CThostFtdcDepthMarketDataField();
        strcpy(tick->InstrumentID, inst.c_str());
        tick->LastPrice = 48830.0;
        tick->Volume = 200;

        // init account
        Account* account = new Account();
        account->AccountID = USER;
        account->BrokerID = BROKER;
        account->Balance = 500000;
        account->PreBalance = 500000;
        account->Available = 500000;
        account->CurrMargin = 0;
        account->PreMargin = 0;
        account->FrozenMargin = 0;
        account->PositionProfit = 0;
        account->Commission = 0;
        account->FrozenCommission = 0;
        account->CloseProfit = 0;
        om.SetAccount(account);

        // init position
        Position* position = new Position(inst);
        position->broker = BROKER; 
        position->investor = USER; 
        om.AddPosition(inst, position);
    }

    void Exchange::GenMarketData()
    {
        while(true)
        {
            Poco::Thread::sleep(500);
            CThostFtdcDepthMarketDataField* pData = GenTick();
            Tick* _tick = new Tick(pData);

            TickEvent(this, _tick);
        }
    }

    void Exchange::GenHistoricalData(Serializer* ser)
    {
        vector<Tick*> ticks;
        ser->Deserialize(ticks);

        for (int i = 0; i < ticks.size(); i++)
        {
            Tick* tick = ticks[i];
            TickEvent(this, tick);
        }
    }

    void Exchange::Run()
    {
        Poco::RunnableAdapter<Exchange> runnable(*this, &Exchange::GenMarketData);
        Poco::Thread market_thread;
        market_thread.start(runnable);

        market_thread.join();
    }

    void Exchange::OnRspQryTradingAccount(Account* account)
    {
        account = om.GetAccount();
    }

    void Exchange::OnRspQryInvestorPosition(const string& inst, Position* position)
    {
        position = om.GetPosition(inst);
    }

    void Exchange::OnRspOrderInsert(Order* order)
    {
        // TODO: will add an order manager to control all the behaviors
        // maybe include the customized risk monitor system!
        string inst = order->instrument;
        om.InsertOrder(order);
    }

    CThostFtdcDepthMarketDataField* Exchange::GenTick()
    {
        Poco::DateTime now;
        string date = Poco::format("%02d%02d%02d", now.year(), now.month(), now.day());
        string time = Poco::format("%02d:%02d:%02d", now.hour(), now.minute(), now.second());

        strcpy(tick->TradingDay, date.c_str());
        strcpy(tick->UpdateTime, time.c_str());
        strcpy(tick->InstrumentID, INSTRUMENT);
        tick->UpdateMillisec = now.millisecond();
        tick->LastPrice = RandomWalk(tick->LastPrice);
        tick->Volume = tick->Volume;
        return tick;
    }

    Tick* Exchange::GenRandomTick(Tick* proto)
    {
        Tick* tick = new Tick(*proto);
        Poco::DateTime now;
        tick->DateTime = now;
        tick->LastPrice = RandomWalk(proto->LastPrice);
        return tick;
    }
}}
