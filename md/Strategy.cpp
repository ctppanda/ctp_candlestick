#include "Strategy.h"
#include "Executor.h"

namespace Pheux { namespace Core {

    BarSeries* Strategy::RegisterCustomSeries(string inst, int period, Bar::BarType type, int length)
    {
        return exector->RegisterCustomSeries(inst, period, type, length);
    }

    Indicator* Strategy::RegisterIndicator(string inst, Indicator* indicator)
    {
        return exector->RegisterIndicator(inst, indicator);
    }

    void Strategy::GetSettlementInfo()
    {
        exector->Trader()->GetSettlementInfo();
    }

    Account* Strategy::GetAccountInfo(bool isCache)
    {
        return exector->Trader()->GetAccountInfo(isCache);
    }

    Position* Strategy::GetPositionInfo(const string& inst, bool isCache)
    {
        return exector->Trader()->GetPositionInfo(inst, isCache);
    }

    Order* Strategy::SendOrder(std::string inst, Order::Direction direc, int volume, double price)
    {
        return exector->Trader()->SendOrder(inst, direc, volume, price);
    }

    double Strategy::LastPrice()
    {
        Tick* tick = exector->CurTick();
        return (tick != NULL) ? tick->LastPrice : 0;
    }

    //////////////////////////////////////////////////////////////////////////

    void DoubleMAStrategy::Init()
    {
        inst = "cu1305";
        bars = RegisterCustomSeries(inst, 1, Bar::SECOND);
        fast_ma = (MA*)RegisterIndicator(inst, new MA(bars, 3));
        slow_ma = (MA*)RegisterIndicator(inst, new MA(bars, 6));
    }

    void DoubleMAStrategy::Run()
    {
        if (fast_ma->Count() <= 1 || slow_ma->Count() <= 1)
            return;

        double fv1 = fast_ma->get(1);
        double fv0 = fast_ma->get(0);
        double sv1 = slow_ma->get(1);
        double sv0 = slow_ma->get(0);

        if (fv1 < sv1 && fv0 > sv0)
        {
            cout << "Up Cross" << endl;
            SendOrder(inst, Order::Buy, 1, LastPrice());
        }

        if (fv1 > sv1 && fv0 < sv0)
        {
            cout << "Down Cross" << endl;
            SendOrder(inst, Order::Sell, 1, LastPrice());
        }
    }

    void DoubleMAStrategy::Exit()
    {
        cout << "Exit!" << endl;
    }
}}
