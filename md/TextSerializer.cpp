#include <fstream>
#include "Serializer.h"
#include "BarSeries.h"
#include "Constant.h"
#include "Tick.h"
#include "Bar.h"
#include "Poco/Glob.h"
#include "Poco/StringTokenizer.h"

namespace Pheux { namespace Core {

    TextSerializer::TextSerializer(const string& path, string delim, int chunk)
        : path(path), delim(delim), chunk(chunk)
    {}

    TextSerializer::~TextSerializer()
    {
        dump();
    }

    void TextSerializer::Serialize(Tick* tick)
    {
        buffer.push_back(tick);

        if (buffer.size() == chunk)
        {
            dump();
            buffer.clear();
        }
    }

    void TextSerializer::Deserialize(vector<Tick*>& vec)
    {
        //vec.clear();
        //set<string> files;
        //Poco::Glob::glob(path, files);

        //for (set<string>::iterator it = files.begin(); it != files.end(); it++)
        //{
        //    vector<Tick*> tmp;
        //    Deserialize(*it, tmp);

        //    for (vector<Tick*>::iterator it2 = tmp.begin(); it2 != tmp.end(); it2++)
        //        vec.push_back(*it2);
        //}
        vec.clear();
        set<string> files;
        Poco::Glob::glob(Poco::format("%s/*.csv", path), files);

        for (set<string>::iterator it = files.begin(); it != files.end(); it++)
        {
            string file = *it;
            string line;
            std::ifstream ifs(file);
            if (ifs)
            {
                while (ifs.good())
                {
                    getline(ifs,line);
                    Tick* tick = deserTick(line);
                    if (tick != NULL)
                        vec.push_back(tick);
                }
            }
            ifs.close();
        }
    }

    void TextSerializer::Deserialize(const std::string& inst, std::vector<Tick*>& vec)
    {
        vec.clear();
        set<string> files;
        Poco::Glob::glob(Poco::format("%s/%s/*.csv", path, inst), files);

        for (set<string>::iterator it = files.begin(); it != files.end(); it++)
        {
            string file = *it;
            string line;
            std::ifstream ifs(file);
            if (ifs)
            {
                while (ifs.good())
                {
                    getline(ifs,line);
                    Tick* tick = deserTick(line);
                    if (tick != NULL)
                        vec.push_back(tick);
                }
            }
            ifs.close();
        }
    }

    void TextSerializer::dump()
    {
        if (buffer.size() == 0)
            return;

        Poco::DateTime date = buffer[0]->DateTime;
        string file = Poco::format("%s/%04d%02d%02d.csv", path, date.year(), date.month(), date.day());

        std::ofstream os;
        os.open (file, std::ofstream::out | std::ofstream::app);

        for (int i = 0; i < buffer.size(); i++)
            os << serTick(buffer[i]);

        os << std::flush;
        os.close();
    }

    string TextSerializer::serTick(const Tick* tick) const
    {
        std::ostringstream oss;
        oss << Converter::ToString(tick->DateTime) << delim
            << tick->LastPrice << delim
            << tick->Volume << endl;
        return oss.str();
    }

    Tick* TextSerializer::deserTick(const string& line)
    {
        Tick* tick = NULL;
        Poco::StringTokenizer token(line, delim);

        if (token.count() == 3)
        {
            tick = new Tick();
            Converter::ToDateTime(token[0], tick->DateTime);
            tick->LastPrice = Converter::ToDouble(token[1]);
            tick->Volume = Converter::ToInt(token[2]);
            tick->InstrumentID = INSTRUMENT;
        }
        return tick;
    }

}}
