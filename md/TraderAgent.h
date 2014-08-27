#ifndef TRADER_AGENT_H
#define TRADER_AGENT_H

#include "Util.h"
#include "Account.h"
#include "Order.h"
#include "LoginSession.h"
#include "Position.h"
#include "Exchange.h"
#include "OrderManager.h"

namespace Pheux { namespace Core {

    class TraderApi;
    class TraderSpi;

    class TraderAgent
    {
    public:
        friend class TraderApi;
        friend class TraderSpi;

    public:
        TraderAgent();

        void Init();

        void GetSettlementInfo();

        Account* GetAccountInfo(bool isCache = true);

        Position* GetPositionInfo(const std::string& inst, bool isCache = true);

        Order* SendOrder(std::string inst, Order::Direction direc, int volume, double price);

    private:
        TraderApi* api;
        TraderSpi* spi;

        LoginSession* session;
        OrderManager om;

        //Account* account;
        //std::map<std::string, Position*> positions;

#ifdef _MOCK_
        Mock::Exchange* exchange;
#endif
    };

}}
#endif
