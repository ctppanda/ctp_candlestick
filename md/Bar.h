#ifndef BAR_H
#define BAR_H

#include <ctime>
#include "Util.h"
#include "Tick.h"
#include "ConnectionPool.h"
namespace Pheux { namespace Core {

    class Bar
    {
    public:
        enum BarType
        {
            TICK,
            SECOND,
            MINUTE,
            HOUR,
            DAY,
            WEEK,
            YEAR
        };

        friend class BarFactory;

    public:
        Poco::DateTime BeginTime;
        Poco::DateTime EndTime;
        double Open;
        double Close;
        double High;
        double Low;
        double Turnover;
        int Volume;
        double PreClose;
        bool IsComplete;
        int Period;
        bool IsHistorical;
		ConnPool *mysqlpool;

        virtual Bar* Calculate(Tick* tick) = 0;
        virtual BarType Type() const = 0;

        virtual string ToString() const
        {
            std::ostringstream oss;
            oss << "[" << Converter::ToString(BeginTime)
                << " - " << Converter::ToString(EndTime) << "]"
                << "Period: " << Period
                << ", Open: " << Open << ", Close: " << Close << ", High: " << High << ", Low: " << Low
                << ", Volume: " << Volume << endl;
            return oss.str();
        }
		virtual void Save()
		{
		}

        virtual void Init(Tick* tick)
        {
            BeginTime = EndTime = tick->DateTime;
            Open = Close = High = Low = tick->LastPrice;
            Volume = tick->Volume;
        }

    public:
        //Bar() : IsComplete(false), IsHistorical(false) {}
        Bar(int p) 
            : Period(p), IsComplete(false), IsHistorical(false) {}
        Bar(Tick* t, int p)
            : BeginTime(t->DateTime), EndTime(t->DateTime), Period(p), IsComplete(false), IsHistorical(false),
            Open(t->LastPrice), Close(t->LastPrice), High(t->LastPrice), Low(t->LastPrice), Volume(t->Volume)
        {}
		Bar(int p, ConnPool* conpl) 
            : Period(p), IsComplete(false), IsHistorical(false), mysqlpool(conpl) {}
        Bar(Tick* t, int p, ConnPool* conpl)
            : BeginTime(t->DateTime), EndTime(t->DateTime), Period(p), IsComplete(false), IsHistorical(false),
			mysqlpool(conpl),
            Open(t->LastPrice), Close(t->LastPrice), High(t->LastPrice), Low(t->LastPrice), Volume(t->Volume)
        {}
        virtual ~Bar() {}

    protected:
        void update(Tick* tick)
        {
            High = tick->LastPrice > High ? tick->LastPrice : High;
            Low = tick->LastPrice < Low ? tick->LastPrice : Low;
            Close = tick->LastPrice;
            Volume += tick->Volume;
            EndTime = tick->DateTime;
        }

        virtual bool isComplete(const Poco::DateTime& ttime) const
        {
            return IsComplete && IsHistorical;
        }
    };

    class TickBar : public Bar
    {
    public:
        TickBar() : Bar(1) {}

        TickBar(Tick* t) : Bar(t, 1)
        {
            EndTime = t->DateTime;
            IsComplete = true;
        }
        virtual ~TickBar() {}

        virtual BarType Type() const { return Bar::TICK; }

        virtual Bar* Calculate(Tick* tick)
        {
            return new TickBar(tick);
        }

        virtual string ToString() const
        {
            return "[TickBar]" + Bar::ToString();
        }

        Tick* GenTick(string inst)
        {
            Tick* tick = new Tick();
            tick->DateTime = BeginTime;
            tick->LastPrice = Open;
            tick->Volume = Volume;
            tick->InstrumentID = inst;
            return tick;
        }
		virtual void Save()
		{Connection*con
			mysqlpool = ConnPool::GetInstance();
			mysqlpool->GetConnection();
		}

    private:
        virtual bool isComplete(Poco::DateTime* ttime) const { return true; }
    };

    class SecondBar : public Bar
    {
    public:
        SecondBar(int p = 1) : Bar(p) {}
        SecondBar(Tick* t, int p = 1) : Bar(t, p) {}
        virtual ~SecondBar() {}
        virtual BarType Type() const { return Bar::SECOND; }

        virtual Bar* Calculate(Tick* tick)
        {
            IsComplete = isComplete(tick->DateTime);

            if (IsComplete)
            {
                cout << this->ToString();
                return new SecondBar(tick, Period);
            }
            else
            {
                update(tick);
                return NULL;
            }
        }

        virtual string ToString() const
        {
            return "[SecondBar]" + Bar::ToString();
        }

    private:
        virtual bool isComplete(const Poco::DateTime& ttime) const
        {
            bool _compl =
                (EndTime.second() != ttime.second()) ? true :
                (EndTime.minute() != ttime.minute()) ? true :
                (EndTime.hour() != ttime.hour()) ? true : false;
            return
                (_compl && (ttime.second() % Period == 0)) ||
                Bar::isComplete(ttime);
        }
    };

    class MinuteBar : public Bar
    {
    public:
        MinuteBar(int p = 1) : Bar(p) {}
        MinuteBar(Tick* t, int p = 1) : Bar(t, p) {}

        virtual ~MinuteBar() {}
        virtual BarType Type() const { return Bar::MINUTE; }

        virtual Bar* Calculate(Tick* tick)
        {
            IsComplete = isComplete(tick->DateTime);

            if (IsComplete)
            {
                cout << this->ToString();
                return new MinuteBar(tick, Period);
            }
            else
            {
                update(tick);
                return NULL;
            }
        }

        virtual string ToString() const
        {
            return "[MinuteBar]" + Bar::ToString();
        }

    private:
        virtual bool isComplete(const Poco::DateTime& ttime) const
        {
            bool _compl =
                (EndTime.minute() != ttime.minute()) ? true :
                (EndTime.hour() != ttime.hour()) ? true : false;
            return
                (_compl && (ttime.minute() % Period == 0)) ||
                Bar::isComplete(ttime);
        }
    };

    class BarFactory
    {
    public:
        static Bar* CreateBar(Bar::BarType type, int period)
        {
            switch (type)
            {
            case Bar::TICK:
                return new TickBar();
            case Bar::SECOND:
                return new SecondBar(period);
            case Bar::MINUTE:
                return new MinuteBar(period);
            default:
                // TODO: error
                return NULL;
            }
        }

        static Bar* CloneBar(const Bar* proto)
        {
            if (proto == NULL)
                return NULL;

            // the DateTime type is the same! not really deep clone
            // TODO: consider to replace the Begin/EndTime from pointer to instance
            Bar* bar = CreateBar(proto->Type(), proto->Period);
            return bar;
        }
    };

}}


#endif
