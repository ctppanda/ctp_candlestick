#ifndef TRADER_API_H
#define TRADER_API_H

#include "ThostFtdcTraderApi.h"
#include "Util.h"
#include "Order.h"

namespace Pheux { namespace Core {

    // TraderApi is a proxy of CThostFtdcTraderApi, instead of the derived class
    //
    class TraderApi
    {
    public:
        TraderApi() {}
        ~TraderApi() {}

        TraderApi(const string& addr, const string& broker, const string& user, const string& passwd, const string& path = "");

        void Init(CThostFtdcTraderSpi* spi);

        bool ReqUserLogin();

        bool ReqSettlementInfoConfirm();

        bool ReqQryTradingAccount();

        bool ReqQryInvestorPosition(const std::string& inst);

        bool ReqOrderInsert(const Order* order);

    private:
        CThostFtdcTraderApi* proxy;
        int request_id;

        char* address;
        char* broker_id;
        char* user_id;
        char* password;
    };

}}

#endif
