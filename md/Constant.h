#ifndef CONSTANT_H
#define CONSTANT_H

#include "Util.h"
#include "Bar.h"

namespace Pheux { namespace Core {

    static map<Bar::BarType, string> create_map_BarShortName()
    {
        map<Bar::BarType, string> m;
        m[Bar::TICK] = "T";
        m[Bar::SECOND] = "S";
        m[Bar::MINUTE] = "M";
        m[Bar::HOUR] = "H";
        m[Bar::DAY] = "D";
        m[Bar::WEEK] = "W";
        m[Bar::YEAR] = "Y";
        return m;
    }
    static map<Bar::BarType, string> BarShortName = create_map_BarShortName();

}}

#endif
