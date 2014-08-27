#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <queue>
#include "Poco/Thread.h"
#include "Poco/ThreadPool.h"
#include "Poco/Runnable.h"
#include "Poco/Mutex.h"
#include "Util.h"
#include "TraderAgent.h"
#include "SeriesManager.h"
#include "IndicatorManager.h"


namespace Pheux { namespace Core {

    class Executor : public Poco::Runnable
    {
    public:
        enum QueueSchedule
        {
            AVERAGE,
            FIFO,
            LIFO,
        };

    public:
        Executor()
            : queue_schedule(AVERAGE), tick(NULL), finish(true)
        {
            trader.Init();
        }

        void LoadStrategy()
        {
            strategies.push_back(new DoubleMAStrategy());
        }

        void Init()
        {
            for (int i = 0; i < strategies.size(); i++)
            {
                strategies[i]->exector = this;
                strategies[i]->Init();
            }
        }

        void Run(Tick* tick)
        {
			cout << "here" << endl;
            if (tick == NULL || instruments.find(tick->InstrumentID) == instruments.end())
                return;

            if (!finish)
            {
				cout << "fuck!" << endl;
                queue.push(tick);
				cout << queue.size() << "now!" << endl;
                return;
            }
			cout << "here2" << endl;
            this->tick = schedule(tick);
			cout << queue.size() << endl;
            Poco::ThreadPool::defaultPool().start(*this);
			cout << "here3" << endl;
            //Poco::ThreadPool::defaultPool().joinAll();
        }

        void Run(const void* sender, Tick*& arg)
        {
            Tick* tick = arg;
            Run(tick);
        }

        void SerialRun(Tick* tick)
        {
            this->tick = tick;
            run();
        }

        void SerialRun(const void* sender, Tick*& arg)
        {
            Tick* tick = arg;
            SerialRun(tick);
        }

        void Exit()
        {
            for (int i = 0; i < strategies.size(); i++)
            {
                strategies[i]->Exit();
            }
        }

        BarSeries* RegisterCustomSeries(string inst, int period, Bar::BarType type, int length)
        {
            instruments.insert(inst);

            SeriesManager* manager = GetSeriesManager(inst);
            if (manager == NULL)
            {
                manager = new SeriesManager(inst);
                series[inst] = manager;
            }
            return manager->RegisterCustomSeries(period, type, length);
        }

        SeriesManager* GetSeriesManager(string inst)
        {
            series_it = series.find(inst);
            return (series_it != series.end()) ? series_it->second : NULL;
        }

        Indicator* RegisterIndicator(string inst, Indicator* indicator)
        {
            if (instruments.find(inst) == instruments.end())
            {
                string msg = inst + ": is not found in the bar series.";
                //throw new std::exception(msg.c_str());
                throw msg;
            }

            IndicatorManager* manager = GetIndicatorManager(inst);
            if (manager == NULL)
            {
                manager = new IndicatorManager(inst);
                indicators[inst] = manager;
            }
            return manager->RegisterIndicator(indicator);
        }

        IndicatorManager* GetIndicatorManager(string inst)
        {
            ind_it = indicators.find(inst);
            return (ind_it != indicators.end()) ? ind_it->second : NULL;
        }

        TraderAgent* Trader() { return &trader; }

        Tick* CurTick() { return tick; }

        bool IsFinish() const { return finish; }

        set<string> Instruments() const { return instruments; }

    private:
        vector<Strategy*> strategies;
        bool finish;
        Tick* tick;
        QueueSchedule queue_schedule;
        set<string> instruments;

        TraderAgent trader;

        map<string, SeriesManager*>::iterator series_it;
        map<string, SeriesManager*> series;

        map<string, IndicatorManager*>::iterator ind_it;
        map<string, IndicatorManager*> indicators;

        // TODO:: actually it will be Bar instead of Tick
        std::queue<Tick*> queue;
        Poco::Mutex _mutex;

        Tick* dequeue()
        {
            Tick* res = queue.front();
            queue.pop();
            return res;
        }

        virtual void run()
        {
			cout << "you are here" << endl;
            changeStatus(false);

            SeriesManager* sm = GetSeriesManager(tick->InstrumentID);
            if (sm != NULL)
                sm->Calculate(tick);

            IndicatorManager* im = GetIndicatorManager(tick->InstrumentID);
            if (im != NULL)
                im->Calculate(tick);

            for (int i = 0; i < strategies.size(); i++)
            {
                strategies[i]->Run();
            }

            //Poco::Thread::sleep(2000);
            changeStatus(true);
			cout << "you are here now!!!!!" << endl;
        }

        Tick* schedule(Tick* tick)
        {
            if (queue.empty())
                return tick;

            Tick* res = tick;
            switch (queue_schedule)
            {
            case AVERAGE:
                {
                    double sum = tick->LastPrice;
                    int size = queue.size();
                    for (int i = 0; i < size; i++)
                    {
                        sum += dequeue()->LastPrice;
                    }
                    res->LastPrice = sum / (size + 1);
                    break;
                }
            case FIFO:
                res = dequeue();
                break;
            case LIFO:
                //res = tick;
                break;
            }
            return res;
        }

        void changeStatus(bool status)
        {
            _mutex.lock();
			cout << "status: "<< status << endl;
            finish = status;
            _mutex.unlock();
        }
    };

}}
#endif
