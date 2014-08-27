#include "Order.h"

namespace Pheux { namespace Core {

    int Order::max_order_ref = 0;

    string Order::ToString()
    {
        std::ostringstream oss;
        oss << "(Order)id=" << max_order_ref
            << ",broker=" << broker
            << ",exchange=" << exchange
            << ",investor=" << investor
            << ",instrument=" << instrument
            << ",insert_time=" << Converter::ToString(insert_time)
            << ",limit_price=" << limit_price
            << ",stop_price=" << stop_price
            << ",volume=" << volume
            << ",direction=" << direction
            << ",condition=" << condition
            << ",open_close=" << open_close;
        return oss.str();
    }

}}
