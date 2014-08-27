#ifndef ACCOUNT_MANAGER_H
#define ACCOUNT_MANAGER_H

#include "Util.h"
#include "Account.h"
#include "ThostFtdcTraderApi.h"

namespace Pheux { namespace Core {

class AccountManager
{
public:
    AccountManager() : account(NULL) {}
    ~AccountManager() {}

    void Update(CThostFtdcTradingAccountField* info)
    {
        if (account != NULL)
            account->Update(info);
        else
            account = new Account(info);
    }

    Account* GetAccount() const { return account; }

private:
    Account* account;
};

}}

#endif
