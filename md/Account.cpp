#include "Account.h"

namespace Pheux { namespace Core {

    Account::Account(CThostFtdcTradingAccountField* info)
    {
        Update(info);
    }

    void Account::Update(CThostFtdcTradingAccountField* info)
    {
        if (info == NULL)
            return;

        AccountID = info->AccountID;
        BrokerID = info->BrokerID;
        Balance = info->Balance;
        PreBalance = info->PreBalance;
        Available = info->Available;
        CurrMargin = info->CurrMargin;
        PreMargin = info->PreMargin;
        FrozenMargin = info->FrozenMargin;
        PositionProfit = info->PositionProfit;
        Commission = info->Commission;
        FrozenCommission = info->FrozenCommission;
        CloseProfit = info->CloseProfit;
    }

    string Account::HashKey() const
    {
        return BrokerID + ":" + AccountID;
    }

    string Account::ToString()
    {
        std::ostringstream oss;
        oss << "(Account)AccountID=" << AccountID
            << ",BrokerID=" << BrokerID
            << ",Balance=" << Balance
            << ",PreBalance=" << PreBalance
            << ",Available=" << Available
            << ",CurrMargin=" << CurrMargin
            << ",PreMargin=" << PreMargin
            << ",FrozenMargin=" << FrozenMargin
            << ",PositionProfit=" << PositionProfit
            << ",Commission=" << Commission
            << ",FrozenCommission=" << FrozenCommission
            << ",CloseProfit=" << CloseProfit;
        return oss.str();
    }
}}
