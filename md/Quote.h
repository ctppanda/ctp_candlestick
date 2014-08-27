#ifndef QUOTE_H
#define QUOTE_H

#include <iostream>
#include <sstream>

namespace Pheux { namespace Core {

    class Quote
    {
    public:
        Quote() {}
        Quote(double ap1, double bp1, int av1, int bv1)
        {
            AskPrice[0] = ap1;
            BidPrice[0] = bp1;
            AskVolume[0] = av1;
            BidVolume[0] = bv1;
        }

        ~Quote() {}

        double AskPrice[5];
        double BidPrice[5];
        int AskVolume[5];
        int BidVolume[5];

        friend std::ostream& operator<< (std::ostream& os, Quote& e)
        {
            return os << e.ToString();
        }

        std::string ToString() const
        {
            std::ostringstream oss;
            oss << "AskPrice1: " << AskPrice[0] << " BidPrice1: " << BidPrice[0] << " AskVolume1: " << AskVolume[0] << " BidVolume1: " << BidVolume[0];
            return oss.str();
        }
    };

}}


#endif
