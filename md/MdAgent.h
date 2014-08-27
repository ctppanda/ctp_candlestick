#ifndef MD_AGENT_H
#define MD_AGENT_H

#include "Util.h"
#include "Tick.h"
#include "Poco/BasicEvent.h"
#include "Poco/Delegate.h"
namespace Pheux { namespace Core {

    class MdApi;
    class MdSpi;

    class MdAgent
    {
    public:
        friend class MdApi;
        friend class MdSpi;

    public:
        MdAgent()
            : api(NULL), spi(NULL)
        {}
        ~MdAgent() {}

        void Init(const vector<string>& insts);
        void Join();
        void Release();

        Poco::BasicEvent<Tick*> TickEvent;

    private:
        MdApi* api;
        MdSpi* spi;

        vector<string> instruments;
    };

}}

#endif
