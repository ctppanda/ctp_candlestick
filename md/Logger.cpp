#include "Logger.h"
#include "Poco/PatternFormatter.h"
#include "Poco/FormattingChannel.h"
#include "Poco/ConsoleChannel.h"
#include "Poco/FileChannel.h"
#include "Poco/Message.h"

namespace Pheux { namespace Core {

    using std::string;
    using Poco::Logger;
    using Poco::FormattingChannel;
    using Poco::FormattingChannel;
    using Poco::PatternFormatter;
    using Poco::FileChannel;
    using Poco::ConsoleChannel;

    Poco::Logger* logger;
    Poco::Logger* console;

    Poco::Logger* CreateFileLogger(const string& file, const string& name)
    {
        FormattingChannel* pFCFile = new FormattingChannel(new PatternFormatter("%Y-%m-%d %H:%M:%S.%c [%l][%p] %t"));
        pFCFile->setChannel(new FileChannel(file));
        pFCFile->open();

        Logger::root().setChannel(pFCFile);
        return &(Logger::get(name));
    }

    Poco::Logger* CreateConsoleLogger(const string& name)
    {
        FormattingChannel* pFCConsole = new FormattingChannel(new PatternFormatter("%s: %p: %t"));
        pFCConsole->setChannel(new ConsoleChannel);
        pFCConsole->open();

        Logger::root().setChannel(pFCConsole);
        return &(Logger::get(name));
    }
}}
