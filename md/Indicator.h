#ifndef INDICATOR_H
#define INDICATOR_H

#include "BarSeries.h"

namespace Pheux { namespace Core {

    class IndicatorEnum
    {
    public:
        enum MAMode
        {
            SMA = 0,
            EMA,
            SMMA,
            LWMA
        };

        enum AppliedPrice
        {
            CLOSE = 0,
            OPEN,
            HIGH,
            LOW,
            MEDIAN,     // (high+low)/2
            TYPICAL,    // (high+low+close)/3
            WEIGHTED    // (high+low+close+close)/4
        };
    };

    inline double getAppliedPrice(const Bar& bar, int applied)
    {
        switch (applied)
        {
        case IndicatorEnum::CLOSE:
            return bar.Close;
        case IndicatorEnum::OPEN:
            return bar.Open;
        case IndicatorEnum::HIGH:
            return bar.High;
        case IndicatorEnum::LOW:
            return bar.Low;
        case IndicatorEnum::MEDIAN:
            return (bar.High + bar.Low) / 2;
        case IndicatorEnum::TYPICAL:
            return (bar.High + bar.Low + bar.Close) / 3;
        case IndicatorEnum::WEIGHTED:
            return (bar.High + bar.Low + bar.Close + bar.Close) / 4;
        default:
            return 0;
        }
    }

    class Indicator 
    {
    public:
        virtual bool IsValid() const { return valid; }
        virtual void Calculate() = 0;
        virtual int Count() const =  0;
        virtual string HashKey() const = 0;

        void SetVisitId(int id) { visit = series->mapId(id); }
        int GetVisitId() const { return (visit != -1) ? series->mapId(visit) : series->size(); }

    protected:
        Indicator() {}
        Indicator(BarSeries* series) : series(series), valid(false), visit(-1) {}
        virtual ~Indicator() {}

        BarSeries* series;
        bool valid;
        int visit; // indicator visiting indexer
    };

    class MA : public Indicator
    {
    public:
        MA() {}
        MA(BarSeries* series, int period, IndicatorEnum::AppliedPrice applied = IndicatorEnum::CLOSE)
            : Indicator(series), period(period), applied(applied)
        {
            // keep sure the indicator is pre-calc before to calc the real time data.
            // that is it should be defined after the (historical) bar data is ready
            // init the historical indicator
            Calculate();
        }
        virtual ~MA() {}

        virtual void Calculate()
        {
            // only take care of the bars with the given length, not the whole bars
            if (series == NULL || series->Length() < period)
            {
                valid = false;
                return;
            }

            // ATTENTION:
            // we could not directly use the TSeries.add() here, as we travel the bar series from the latest to oldest,
            // if we add the result using add(), it will make the latest data in the bottom
            // so use a reverse vector first, and then call add() from back forward
            double sum = 0;
            reverse.clear();
            int visit = GetVisitId();

            // calculate the first one
            for (int i = 0; i < period; i++)
            {
                Bar& bar = *(*series)[i];
                sum += getAppliedPrice(bar, applied);
            }

            if (visit == 0)
            {    // update the latest value instead of adding a new one
                result.set(0, sum / period);
                return;
            }
            else
            {
                reverse.push_back(sum / period);
                visit--;
            }

            for (int i = period; i < series->Length() && visit > 0; i++, visit--)
            {
                Bar& bar_new = *(*series)[i];
                Bar& bar_old = *(*series)[i - period];

                sum += (getAppliedPrice(bar_new, applied) - getAppliedPrice(bar_old, applied));
                reverse.push_back(sum / period);
            }

            for (int i = reverse.size() - 1; i >= 0 ; i--)
            {
                result.add(reverse[i]);
            }
            SetVisitId(0);
            valid = true;
        }

        virtual int Count() const { return result.size(); }
        virtual string HashKey() const
        {
            return "MA_" + Converter::ToString(period);
        }

        double operator[](int id) const { return result[id]; }
        double get(int id) const { return result[id]; }

    private:
        int period;
        IndicatorEnum::AppliedPrice applied;
        TSeries<double> result;
        vector<double> reverse;
    };

}}


#endif
