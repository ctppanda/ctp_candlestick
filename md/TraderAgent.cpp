#include "TraderAgent.h"
#include "TraderApi.h"
#include "TraderSpi.h"

namespace Pheux { namespace Core {

#ifdef _MOCK_

    using Mock::Exchange;

    TraderAgent::TraderAgent()
    {
        exchange = new Exchange();
    }

    void TraderAgent::Init()
    {}

    void TraderAgent::GetSettlementInfo()
    {}

    Account* TraderAgent::GetAccountInfo(bool isCache)
    {
        Account* account = om.GetAccount();
        if(!isCache)
        {
            exchange->OnRspQryTradingAccount(account);
        }
        return account;
    }

    Position* TraderAgent::GetPositionInfo(const string& inst, bool isCache)
    {
        Position* position = om.GetPosition(inst);
        if (!isCache)
        {
            exchange->OnRspQryInvestorPosition(inst, position);
            if (position == NULL)
            {
                om.RemovePosition(inst);
            }
        }
        return position;
    }

    Order* TraderAgent::SendOrder(std::string inst, Order::Direction direc, int volume, double price)
    {
        Order* order = new Order();

        order->broker = BROKER;
        order->investor = USER;
        order->instrument = inst;
        order->direction = direc;
        order->volume = volume;
        order->limit_price = price;
        order->max_order_ref++;

        logger->information(order->ToString());

        exchange->OnRspOrderInsert(order);
        return order;
    }

#else

    TraderAgent::TraderAgent()
        : api(NULL), spi(NULL)
    {
        account = new Account();
        position = new Position();
    }

    void TraderAgent::Init()
    {
        api = new TraderApi(ADDR_TREDE, BROKER, USER, PASSWORD);
        spi = new TraderSpi(this);

        api->Init(spi);
    }

    void TraderAgent::GetSettlementInfo()
    {
        api->ReqSettlementInfoConfirm();
    }

    Account* TraderAgent::GetAccountInfo(bool isCache)
    {
        if(!isCache)
        {
            api->ReqQryTradingAccount();
            // should wait for TraderSpi::OnRspQryTradingAccount finished
            // it is in another thread. Here we should sync the two threads...
            // TODO: Use event or signal?
        }
        return account;
    }

    Position* TraderAgent::GetPositionInfo(const string& inst, bool isCache)
    {
        if (!isCache)
        {
            api->ReqQryInvestorPosition(inst);
            // TODO: ditto
        }
        // TODO: need through OM instead of directly returning in the cache mode
        return position;
    }

    Order* TraderAgent::SendOrder(std::string inst, Order::Direction direc, int volume, double price)
    {
        Order* order = new Order();

        order->broker = BROKER;
        order->investor = USER;
        order->instrument = inst;
        order->direction = direc;
        order->volume = volume;
        order->limit_price = price;

        api->ReqOrderInsert(order);
        return order;
    }

#endif

}}
