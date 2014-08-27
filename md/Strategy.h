#ifndef STRATEGY_H
#define STRATEGY_H

#include "Bar.h"
#include "BarSeries.h"
#include "Indicator.h"
#include "Account.h"
#include "Position.h"
#include "Order.h"

namespace Pheux { namespace Core {

    class Executor;

    class Strategy
    {
    public:
        friend class Executor;

        virtual void Init() = 0;
        virtual void Run() = 0;
        virtual void Exit() = 0;

    protected:
        Executor* exector;

        Strategy() {}
        virtual ~Strategy() {}

        BarSeries* RegisterCustomSeries(string inst, int period, Bar::BarType type, int length = 0);
        Indicator* RegisterIndicator(string inst, Indicator* indicator);

        void GetSettlementInfo();
        Account* GetAccountInfo(bool isCache = true);
        Position* GetPositionInfo(const string& inst, bool isCache = true);
        Order* SendOrder(std::string inst, Order::Direction direc, int volume, double price);

        double LastPrice();
    };

    class DoubleMAStrategy : public Strategy
    {
    public:
        DoubleMAStrategy() {}
        virtual ~DoubleMAStrategy() {}

        virtual void Init();
        virtual void Run();
        virtual void Exit();

    private:
        BarSeries* bars;
        MA* fast_ma;
        MA* slow_ma;
        string inst;
    };

}}


#endif
