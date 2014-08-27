#ifndef PERIOD_SERIES_H
#define PERIOD_SERIES_H

#include <string>
#include "Util.h"
#include "Tick.h"
#include "BarSeries.h"
#include "BarSerializer.h"

namespace Pheux { namespace Core {

    class SeriesManager
    {
    public:
        SeriesManager(string inst) : InstrumentID(inst), _init(false)
        {
            Init();
        }
        ~SeriesManager() {}

        string InstrumentID;

        BarSeries* TickBars;
        BarSeries* SecondBars;
        BarSeries* MinuteBars;
        //BarSeries* HourBars;
        //BarSeries* DayBars;

        map<string, BarSeries*> RegistedBars() const { return _registedBars; }

        void Init()
        {
            // create a series of standard bars with period = 1
            TickBars = RegisterCustomSeries(1, Bar::TICK);
            SecondBars = RegisterCustomSeries(1, Bar::SECOND);
            MinuteBars = RegisterCustomSeries(1, Bar::MINUTE);
        }

        void InitFromHistory()
        {
            BarSerializer::DeSerialize("./", _registedBars);
        }

        void Calculate(Tick* tick)
        {
            if (tick->InstrumentID != InstrumentID)
                return;

            map<string, BarSeries*>::iterator it;
            for (it = _registedBars.begin(); it != _registedBars.end(); it++)
            {
                (it->second)->Calculate(tick);
            }
        }

        // TODO: please make sure the period is a factor of 60/24, if not, it will be truncated at xx:00
        // TODO: now every registered bar will update every tick, one improvement is the conditional trigger
        // e.g: M2 bar is triggered after M1 bar is finished.
        BarSeries* RegisterCustomSeries(int period, Bar::BarType type, int length = 0)
        {
            string key = BarShortName[type] + Converter::ToString(period);
            BarSeries* bars;

            if (_registedBars.find(key) != _registedBars.end())
            {
                bars = _registedBars[key];
            }
            else
            {
                bars = new BarSeries(BarFactory::CreateBar(type, period), length);
                _registedBars[key] = bars;
            }

            return bars;
        }

    private:
        bool _init;
        map<string, BarSeries*> _registedBars;
    };

}}


#endif
