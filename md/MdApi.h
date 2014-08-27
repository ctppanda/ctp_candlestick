#ifndef MD_API_H
#define MD_API_H

#include "Util.h"
#include "ThostFtdcMdApi.h"

namespace Pheux { namespace Core {

    class MdApi
    {
    public:
        MdApi() {}
        ~MdApi() {}

        MdApi(const string& addr, const string& broker, const string& user, const string& passwd, const string& path = "");

        void Init(CThostFtdcMdSpi* spi);
        void Join();
        void Release();

        bool ReqUserLogin();

        bool SubscribeMarketData(const vector<string>& insts);

    private:
        CThostFtdcMdApi* proxy;
        int request_id;

        char* address;
        char* broker_id;
        char* user_id;
        char* password;

        vector<string> instruments;
    };

}}

#endif
