#ifndef ORDER_H
#define ORDER_H

#include "Util.h"

namespace Pheux { namespace Core {

    class Order
    {
    public:
        enum Direction
        {
            Buy = 48,
            Sell = 49
        };

        enum ContingentCondition
        {
            Immediately = 49,
            Touch = 50,
            TouchProfit = 51,
            ParkedOrder = 52,
            LastPriceGreaterThanStopPrice = 53,
            LastPriceGreaterEqualStopPrice = 54,
            LastPriceLesserThanStopPrice = 55,
            LastPriceLesserEqualStopPrice = 56,
            AskPriceGreaterThanStopPrice = 57,
            AskPriceGreaterEqualStopPrice = 65,
            AskPriceLesserThanStopPrice = 66,
            AskPriceLesserEqualStopPrice = 67,
            BidPriceGreaterThanStopPrice = 68,
            BidPriceGreaterEqualStopPrice = 69,
            BidPriceLesserThanStopPrice = 70,
            BidPriceLesserEqualStopPrice = 72
        };

        enum OffsetFlag
        {
            Open = 48,
            Close,
            ForceClose,
            CloseToday,
            CloseYesterday
        };

    public:
        Order()
            : condition(Immediately), open_close(Open), stop_price(0)
        {}
        ~Order() {}

        std::string ToString();

        std::string broker;
        std::string exchange;
        std::string investor;
        std::string instrument;
        Poco::DateTime insert_time;
        double limit_price;
        double stop_price;
        int volume;
        Direction direction;
        ContingentCondition condition;
        OffsetFlag open_close;

        static int max_order_ref;
    };

}}

#endif
