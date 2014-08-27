#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "Util.h"
#include "ThostFtdcTraderApi.h"

namespace Pheux { namespace Core {

    class Account
    {
    public:
        Account() {}
        ~Account() {}

        Account(CThostFtdcTradingAccountField* info);

        void Update(CThostFtdcTradingAccountField* info);

        string HashKey() const;

        string ToString();

    public:
        string AccountID;
        string BrokerID;
		string MarketAddress;
		string TraderAddress;
		string password;
        double Balance;
        double PreBalance;
        double Available;
        double CurrMargin;
        double PreMargin;
        double FrozenMargin;
        double PositionProfit;
        double Commission;
        double FrozenCommission;
        double CloseProfit;
    };

}}

#endif
