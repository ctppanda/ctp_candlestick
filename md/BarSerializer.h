#ifndef BAR_SERIALIZER_H
#define BAR_SERIALIZER_H

#include "Util.h"

namespace Pheux { namespace Core {

    class BarSeries;
    class Bar;

    // a special static serializer which is not derived from Serializer
    //
    class BarSerializer
    {
    public:
        static void Serialize(const std::string& path, std::map<std::string, BarSeries*>& series);
        static void DeSerialize(const std::string& path, std::map<std::string, BarSeries*>& series);

    private:
        static std::string serBar(const Bar& bar);
        static void serBarSeries(const std::string& path, const BarSeries& bars);
        static Bar* deserBar(const std::string& ctype, const std::string& line);
        static Bar* createBar(const std::string& ctype);
    };

}}

#endif
