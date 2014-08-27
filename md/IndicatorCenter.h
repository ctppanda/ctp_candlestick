#ifndef INDICATOR_CENTER_H
#define INDICATOR_CENTER_H

#include "Util.h"
#include "IndicatorManager.h"

namespace Pheux { namespace Core {

    class IndicatorCenter
    {
    public:
        IndicatorCenter() {}
        ~IndicatorCenter() {}

        Indicator* RegisterIndicator(string inst, Indicator* indicator)
        {
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

    private:
        map<string, IndicatorManager*>::iterator ind_it;
        map<string, IndicatorManager*> indicators;
    };

}}

#endif
