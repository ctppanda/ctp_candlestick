#ifndef CONVERTER_H
#define CONVERTER_H

#include <stdlib.h>
#include <cstring>
#include "Util.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/NumberParser.h"
#include "Poco/DateTimeParser.h"
#include "Poco/Timespan.h"
#include "Poco/NumberParser.h"
#include "Poco/NumberFormatter.h"

namespace Pheux { namespace Core {

    class Converter
    {
    public:
        template<typename IN>
        static std::string ToString(IN in)
        {
            return Poco::NumberFormatter::format(in);
        }

        static std::string ToString(const Poco::DateTime& datetime)
        {
            return Poco::DateTimeFormatter::format(datetime, Poco::DateTimeFormat::ISO8601_FRAC_FORMAT);
        }

        static double ToDouble(const std::string& str)
        {
            return Poco::NumberParser::parseFloat(str);
        }

        static int ToInt(const std::string& str)
        {
            return Poco::NumberParser::parse(str);
        }

        static Poco::DateTime ToDateTime(const std::string& str)
        {
            int utc = Poco::DateTimeFormatter::UTC;
            return Poco::DateTimeParser::parse(Poco::DateTimeFormat::ISO8601_FRAC_FORMAT, str, utc);
        }

        static void ToDateTime(const std::string& str, Poco::DateTime& datetime)
        {
            int utc = Poco::DateTimeFormatter::UTC;
            Poco::DateTimeParser::parse(Poco::DateTimeFormat::ISO8601_FRAC_FORMAT, str, datetime, utc);
        }

        static int ToInt(const char* str, int off, int len)
        {
            char* buf = new char[len + 1];
            int i = 0, j = off;
            while (len-- > 0)
                buf[i++] = str[j++];
            buf[i] = '\0';
            return atoi(buf);
        }

        static Poco::DateTime ToDateTime(const char* date, const char* time, int millionsecond = 0)
        {
            // Date: "20130429", Time: "17:18:05"
            int year = ToInt(date, 0, 4);
            int month = ToInt(date, 4, 2);
            int day = ToInt(date, 6, 2);
            int hour = ToInt(time, 0, 2);
            int minute = ToInt(time, 3, 2);
            int second = ToInt(time, 6, 2);
            return Poco::DateTime(year, month, day, hour, minute, second, millionsecond);
        }

        static char* StringToCharArray(const std::string& str)
        {
            char *cstr = new char[str.length() + 1];
            strcpy(cstr, str.c_str());
            return cstr;
        }
    };

}}


#endif
