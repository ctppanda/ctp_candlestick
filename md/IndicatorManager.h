#ifndef INDICATOR_MANAGER_H
#define INDICATOR_MANAGER_H

#include "Util.h"
#include "Indicator.h"

namespace Pheux { namespace Core {

    class IndicatorManager
    {
    public:
        IndicatorManager(string inst) : InstrumentID(inst) {}
        ~IndicatorManager() {}

        string InstrumentID;

        void Calculate(Tick* tick)
        {
            map<string, Indicator*>::iterator it;
            for (it = indicators.begin(); it != indicators.end(); it++)
            {
                it->second->Calculate();
            }
        }

        Indicator* RegisterIndicator(Indicator* indicator)
        {
            indicators[indicator->HashKey()] = indicator;
            return indicator;
        }

    private:
        map<string, Indicator*> indicators;
    };

}}

#endif
