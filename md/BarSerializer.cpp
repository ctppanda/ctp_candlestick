#include <fstream>
#include "BarSerializer.h"
#include "Bar.h"
#include "BarSeries.h"
#include "Poco/Glob.h"
#include "Poco/StringTokenizer.h"

namespace Pheux { namespace Core {

    static string getCombinedType(const string& file)
    {
        int pos = file.find_first_of("_") + 1;
        int pos2 = file.find_first_of(".");
        return file.substr(pos, pos2 - pos);
    }

    static string delim = ";";

    void BarSerializer::Serialize(const string& path, map<string, BarSeries*>& series)
    {
        map<string, BarSeries*>::const_iterator it = series.begin();
        for (; it != series.end(); it++)
        {
            serBarSeries(path, *(it->second));
        }
    }

    void BarSerializer::DeSerialize(const std::string& path, map<string, BarSeries*>& series)
    {
        set<string> files;
        Poco::Glob::glob(path + "*.csv", files);

        for (set<string>::iterator it = files.begin(); it != files.end(); it++)
        {
            string file = *it;
            string line;
            std::ifstream ifs(file);
            if (ifs)
            {
                string ctype = getCombinedType(file);

                BarSeries* bars = NULL;
                if (series.find(ctype) != series.end())
                {
                    bars = series[ctype];
                    bars->clear();
                }

                while (ifs.good())
                {
                    getline(ifs,line);
                    Bar* bar = deserBar(ctype, line);
                    if (bar != NULL)
                    {
                        if (bars == NULL)
                        {
                            bars = new BarSeries(bar);
                            series[ctype] = bars;
                        }
                        bars->add(bar);
                    }
                }
                // add an dummy bar to keep sure to receive the latest real time tick
                //bars->add(createBar(ctype));
            }
            ifs.close();
        }
    }


    std::string BarSerializer::serBar(const Bar& bar)
    {
        std::ostringstream oss;
        oss << Converter::ToString(bar.BeginTime) << delim
            << Converter::ToString(bar.EndTime) << delim
            << bar.Open << delim
            << bar.Close << delim
            << bar.High << delim
            << bar.Low << delim
            << bar.Volume << delim
            << bar.Turnover << endl;
        return oss.str();
    }

    void BarSerializer::serBarSeries(const string& path, const BarSeries& bars)
    {
        if (bars.size() == 0)
            return;

        Bar& bar = *bars[0];

        std::stringstream ss;
        ss  << path
            << bar.BeginTime.year() << bar.BeginTime.month() << bar.BeginTime.day()
            << "_" << bars.CombinedType()
            << ".csv";

        std::ofstream os(ss.str());
        // reverse to dump the data to keep sure the latest one is on [0]
        for (int i = bars.size() - 1; i >= 0; i--)
        {
            // consider to store only non-historical data
            //if (!bars[i]->IsHistorical)
            os << serBar(*bars[i]);
        }
        os << std::flush;
        os.close();
    }

    Bar* BarSerializer::createBar(const std::string& ctype)
    {
        Bar* bar = NULL;
        char type = ctype[0];
        int period = Poco::NumberParser::parse(ctype.substr(1, ctype.size() - 1));

        switch(type)
        {
        case 'T':
            bar = new TickBar();
            break;
        case 'S':
            bar = new SecondBar(period);
            break;
        case 'M':
            bar = new MinuteBar(period);
            break;
        default:
            // TODO: unrecognized type!
            break;
        }

        if (bar != NULL)
        {
            bar->IsComplete = true;
            bar->IsHistorical = true;
        }
        return bar;
    }

    Bar* BarSerializer::deserBar(const std::string& ctype, const std::string& line)
    {
        if (line.empty())
            return NULL;

        Bar* bar = createBar(ctype);
        if (bar != NULL)
        {
            Poco::StringTokenizer token(line, delim);
            if (token.count() == 8)
            {
                Converter::ToDateTime(token[0], bar->BeginTime);
                Converter::ToDateTime(token[1], bar->EndTime);

                bar->Open = Converter::ToDouble(token[2]);
                bar->Close = Converter::ToDouble(token[3]);
                bar->High = Converter::ToDouble(token[4]);
                bar->Low = Converter::ToDouble(token[5]);
                bar->Volume = Converter::ToInt(token[6]);
                bar->Turnover = Converter::ToDouble(token[7]);
            }
        }
        return bar;
    }

}}
