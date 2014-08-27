#ifndef ROUTER_H
#define ROUTER_H

#include "Util.h"
#include "Executor.h"
#include "MdAgent.h"
#include "Exchange.h"
#include "Poco/BasicEvent.h"
#include "Poco/Delegate.h"
#include "Poco/Thread.h"

namespace Pheux { namespace Core {

    class Router
    {
    public:
        Router()
        {
            instruments.push_back("cu1305");
            instruments.push_back("if9999");
        }

        void RegisterExecutor(Executor* executor)
        {
            if (executor == NULL)
                return;

            executors.insert(executor);
            executor->LoadStrategy();
            executor->Init();

            agent.TickEvent += Poco::delegate(&(*executor), &Executor::Run);
            exchange.TickEvent += Poco::delegate(&(*executor), &Executor::Run);
        }

        void DeregisterExecutor(Executor* executor)
        {
            if (executors.find(executor) != executors.end())
            {
                executors.erase(executor);
                executor->Exit();

                agent.TickEvent -= Poco::delegate(&(*executor), &Executor::Run);
                exchange.TickEvent -= Poco::delegate(&(*executor), &Executor::Run);
            }
        }

        void RealTimeRun()
        {
            agent.Init(instruments);

            std::unique_ptr<Serializer> ser = std::unique_ptr<Serializer>(new TextSerializer("./"));
            agent.TickEvent += Poco::delegate(&(*ser), &Serializer::Serialize);

            agent.Join();
            agent.Release();
        }

        void RealTimeMock()
        {
            std::unique_ptr<Serializer> ser = std::unique_ptr<Serializer>(new TextSerializer("./"));
            // TODO: need to let exchange to populate different instruments
            // in the different threads
            exchange.TickEvent += Poco::delegate(&(*ser), &Serializer::Serialize);
            exchange.Run();
        }

    private:
        set<Executor*> executors;
        vector<string> instruments;

        MdAgent agent;
        Mock::Exchange exchange;
    };

}}

#endif
