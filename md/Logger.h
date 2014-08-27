#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include "Poco/Logger.h"

namespace Pheux { namespace Core {

    extern Poco::Logger* logger;
    extern Poco::Logger* console;

    Poco::Logger* CreateFileLogger(const std::string& file, const std::string& name);
    Poco::Logger* CreateConsoleLogger(const std::string& name);
}}

#endif
