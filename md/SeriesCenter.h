#ifndef SERIES_CENTER_H
#define SERIES_CENTER_H

#include "Util.h"
#include "SeriesManager.h"

namespace Pheux { namespace Core {

    class SeriesCenter
    {
    public:
        SeriesCenter() {}
        ~SeriesCenter() {}

        BarSeries* RegisterCustomSeries(string inst, int period, Bar::BarType type, int length)
        {
            SeriesManager* manager = GetSeriesManager(inst);
            if (manager == NULL)
            {
                manager = new SeriesManager(inst);
                series[inst] = manager;
            }
            return manager->RegisterCustomSeries(period, type, length);
        }

        SeriesManager* GetSeriesManager(string inst)
        {
            store_it = series.find(inst);
            return (store_it != series.end()) ? store_it->second : NULL;
        }

    private:
        map<string, SeriesManager*>::iterator store_it;
        map<string, SeriesManager*> series;
    };

}}

#endif
