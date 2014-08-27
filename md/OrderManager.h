#ifndef ORDER_MANAGER_H
#define ORDER_MANAGER_H

#include "Util.h"
#include "Order.h"
#include "Position.h"
#include "Account.h"

namespace Pheux { namespace Core {

    // a simple Order Management System
    //
    class OrderManager
    {
    public:
        enum OrderStatus
        {
            SUCCESS = 0,
            NOT_ENOUGH_MONEY,
            EXCESS_LIMIT,
            UNKNOWN
        };

    public:
        OrderManager() : account(NULL) {}
        ~OrderManager() {}

        Position* GetPosition(const std::string& inst)
        {
            return (positions.find(inst) != positions.end()) ? positions[inst] : NULL;
        }

        void AddPosition(const std::string& inst, Position* position)
        {
            positions[inst] = position;
        }

        void RemovePosition(const std::string& inst)
        {
            pos_it = positions.find(inst);
            if (pos_it != positions.end())
            {
                positions.erase(pos_it);
            }
        }

        Account* GetAccount() const
        {
            return account;
        }

        void SetAccount(Account* account)
        {
            this->account = account;
        }

        OrderStatus InsertOrder(Order* order);

    private:
        Account* account;
        std::map<std::string, Position*> positions;
        std::map<std::string, Position*>::iterator pos_it;
        std::map<Order*, OrderStatus> orders;

        OrderStatus InsertOrder(Order* order, Account* account, Position* position);
    };

}}

#endif
