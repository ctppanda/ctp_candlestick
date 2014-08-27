#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <map>
#include <random>
#include "Util.h"
#include "Poco/Thread.h"
#include "Tick.h"
#include "BarSeries.h"
#include "SeriesManager.h"
#include "Serializer.h"
#include "Indicator.h"
#include "IndicatorManager.h"
#include "Strategy.h"
#include "SeriesCenter.h"
#include "IndicatorCenter.h"
#include "Executor.h"
#include "MdAgent.h"
#include "Exchange.h"
#include "BarSerializer.h"
#include "Router.h"

namespace Pheux { namespace Core {

    using namespace Pheux::Mock;

    template<typename T>
    T RandomWalk(T val)
    {
        // global variable to avoid fake random
        static std::default_random_engine eng;
        static std::normal_distribution<T> dist(val, val / 10);
        return dist(eng);
    }

    class Simulator
    {
    public:
        static Simulator* Instance()
        {
            if (inst == NULL)
                inst = new Simulator();
            return inst;
        }

        void FunctionalTest()
        {
            BarSeries* s5_bars = store["cu1305"]->RegisterCustomSeries(1, Bar::SECOND, 5);
            TextSerializer ser("./");
            store["cu1305"]->InitFromHistory();

            Indicator* ma = new MA(s5_bars, 2);
            indicators["cu1305"]->RegisterIndicator(ma);

            int cnt = 0;
            while(cnt++ != 5)
                //while(true)
            {
                Poco::Thread::sleep(500);
                CThostFtdcDepthMarketDataField* pData = GenTick();

                Tick* tick = new Tick(pData);
                std::map<string, SeriesManager*>::iterator it = store.find(tick->InstrumentID);
                if (it != store.end())
                {
                    it->second->Calculate(tick);
                }
                cout << tick->ToString();

                map<string, IndicatorManager*>::iterator it2 = indicators.find(tick->InstrumentID);
                if (it2 != indicators.end())
                {
                    it2->second->Calculate(tick);
                }
            }

            // Unknown Error!
            //BarSerializer::Serialize("./", store["cu1305"]->RegistedBars());
            for (int i = 0; i < ma->Count(); i++)
            {
                cout << (*(static_cast<MA*>(ma)))[i] << endl;
            }
        }

        // real-time simulation
        void RealTimeMock()
        {
			cout << "real time mock!!" <<endl;
            Executor excutor;
            router.RegisterExecutor(&excutor);
            router.RealTimeMock();
        }

        void RealTimeMock2()
        {
			cout << "here!!" <<endl;
			Executor excutor;
            excutor.LoadStrategy();
            excutor.Init();
			cout << "here2!!" <<endl;
            MdAgent agent;

            vector<string> insts;
            insts.push_back("IF1409");
			insts.push_back("p1501");
			insts.push_back("zn1410");
			cout << "here3!!" <<endl;
            agent.Init(insts);

			cout << insts.size() <<endl;
            agent.TickEvent += Poco::delegate(&excutor, &Executor::Run);
			cout << "here3.6!!" <<endl;
            agent.Join();
			cout << "here3.7!!" <<endl;
            agent.Release();
			cout << "here4!!" <<endl;
            excutor.Exit();
            agent.TickEvent -= Poco::delegate(&excutor, &Executor::Run);
        }

        // for back-testing
        void BackTesting()
        {
            // 1, prepare the strategies information
            excutor.LoadStrategy();
            excutor.Init();

            // 2, load the historical data
            string inst = "cu1305";
            SeriesManager* sm = new SeriesManager(inst);
            sm->InitFromHistory();

            // 3, use ticks to trigger back-testing
            // TODO: we can use other period time series(S1,M1..) to trigger the testing, not only for tick
            //       so it is better to wrap an event instead of the tick!
            BarSeries* ticks = sm->TickBars;
            if (ticks == NULL || ticks->size() == 0)
            {
                cout << "Can not load the tick data. Exit." << endl;
                return;
            }
            
            // we should clone one copy, that is in the runtime, system will use the tick to create a new tickbar
            // so if we use the same pointer, it will lead to infinite loop.
            //BarSeries* ticks = protos->Clone();
            for (int i = 0; i < ticks->size(); i++)
            {
                TickBar* bar = static_cast<TickBar*>((*ticks)[i]);
                excutor.Run(bar->GenTick(inst));
            }

            excutor.Exit();
        }

        void BackTesting2()
        {
            excutor.LoadStrategy();
            excutor.Init();

            Serializer* ser = new TextSerializer("./");

            exchange.TickEvent += Poco::delegate(&excutor, &Executor::SerialRun);
            exchange.GenHistoricalData(ser);

            excutor.Exit();
        }

    private:
        Simulator()
        {
            Poco::DateTime now;
            tick = new CThostFtdcDepthMarketDataField();

            strcpy(tick->InstrumentID, "ru1409");
            tick->LastPrice = 48830.0;
            tick->Volume = 200;

            store["ru1409"] = new SeriesManager("ru1409");
            indicators["ru1409"] = new IndicatorManager("ru1409");

            strategies.push_back(new DoubleMAStrategy());
        }
        ~Simulator() {}

        static Simulator* inst;

    private:
        CThostFtdcDepthMarketDataField* tick;
        std::map<string, SeriesManager*> store;
        std::map<string, IndicatorManager*> indicators;
        vector<Strategy*> strategies;
        Executor excutor;
        Exchange exchange;
        Router router;

        CThostFtdcDepthMarketDataField* GenTick()
        {
            Poco::DateTime now;
            string date = Poco::format("%02d%02d%02d", now.year(), now.month(), now.day());
            string time = Poco::format("%02d:%02d:%02d", now.hour(), now.minute(), now.second());

            strcpy(tick->TradingDay, date.c_str());
            strcpy(tick->UpdateTime, time.c_str());
            tick->UpdateMillisec = now.millisecond();
            tick->LastPrice = RandomWalk(tick->LastPrice);
            tick->Volume = tick->Volume;
            return tick;
        }
    };

    Simulator* Simulator::inst = NULL;

}}


#endif
