#ifndef TICK_H
#define TICK_H

#include <iostream>
#include "Util.h"
#include "Quote.h"
#include "ThostFtdcMdApi.h"

namespace Pheux { namespace Core {

    class Tick
    {
    public:
        Tick() {}
        Tick(CThostFtdcDepthMarketDataField* d)
            : InstrumentID(d->InstrumentID), LastPrice(d->LastPrice), Volume(d->Volume),
            quote(d->AskPrice1, d->BidPrice1, d->AskVolume1, d->BidVolume1)
        {
            DateTime = Converter::ToDateTime(d->TradingDay, d->UpdateTime, d->UpdateMillisec);
        }

        ~Tick() {}

        std::string ToString() const
        {
            std::ostringstream oss;
            oss << "[Tick][" << Converter::ToString(DateTime) << "]"
                << InstrumentID << ": " << LastPrice << endl;
            return oss.str();
        }

    public:
        double Change;
        Poco::DateTime DateTime;
        double DropLimit;
        string ExchangeID;
        string InstrumentID;
        double HighPrice;
        double LastPrice;
        double LowPrice;
        double OpenInterest;
        double OpenPrice;
        double PreClosePrice;
        double PreOpenInterest;
        double PreSettlementPrice;
        Quote quote;
        double Turnover;
        double UpLimit;
        int Volume;
    };

}}


#endif
