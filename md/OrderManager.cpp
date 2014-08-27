#include "OrderManager.h"

namespace Pheux { namespace Core {

    OrderManager::OrderStatus OrderManager::InsertOrder(Order* order)
    {
        string inst = order->instrument;
        Position* position = GetPosition(inst);

        if (position == NULL)
        {
            position = new Position(inst);
            positions[inst] = position;
        }

        OrderStatus status = InsertOrder(order, this->account, position);
        orders[order] = status;

        logger->information(Poco::format("OrderManager::InsertOrder: order_id=%d, status=%d", order->max_order_ref, status));
        logger->information(account->ToString());
        logger->information(position->ToString());

        return status;
    }

    OrderManager::OrderStatus OrderManager::InsertOrder(Order* order, Account* account, Position* position)
    {
        if (order->direction == Order::Buy)
        {
            if (position->direction == Position::Long || position->direction == Position::Net)
            {
                // TODO: need to judge the margin
                double residual = account->Available - order->limit_price * order->volume;
                if (residual <= 0)
                {
                    return OrderManager::NOT_ENOUGH_MONEY;
                }

                account->Available = residual;
                position->volume += order->volume;
                position->direction = Position::Long;
            }
            else // position->direction == Position::Short
            {
                if (position->volume >= order->volume)
                {
                    account->Available += order->volume * order->limit_price;
                    position->volume -= order->volume;

                    if (position->volume == 0)
                        position->direction = Position::Net;
                }
                else
                {
                    // TODO: allow partial deal?
                    // 1, close the position
                    account->Available += position->volume * order->limit_price;
                    int revert_volume = order->volume - position->volume;

                    double residual = account->Available - order->limit_price * revert_volume;
                    if (residual <= 0)
                    {
                        return OrderManager::NOT_ENOUGH_MONEY;
                    }

                    // 2, open the reverse position
                    account->Available = residual;
                    position->volume = revert_volume;
                    position->direction = Position::Short;
                }
            }
        }
        else // order->direction == Order::Sell
        {
            if (position->direction == Position::Short || position->direction == Position::Net)
            {
                double residual = account->Available - order->limit_price * order->volume;
                if (residual <= 0)
                {
                    return OrderManager::NOT_ENOUGH_MONEY;
                }

                account->Available = residual;
                position->volume += order->volume;
                position->direction = Position::Short;
            }
            else // position->direction == Position::Long
            {
                if (position->volume >= order->volume)
                {
                    account->Available += order->volume * order->limit_price;
                    position->volume -= order->volume;

                    if (position->volume == 0)
                        position->direction = Position::Net;
                }
                else
                {
                    // TODO: allow partial deal?
                    // 1, close the position
                    account->Available += position->volume * order->limit_price;
                    int revert_volume = order->volume - position->volume;

                    double residual = account->Available - order->limit_price * revert_volume;
                    if (residual <= 0)
                    {
                        return OrderManager::NOT_ENOUGH_MONEY;
                    }

                    // 2, open the reverse position
                    account->Available = residual;
                    position->volume = revert_volume;
                    position->direction = Position::Long;
                }
            }
        }
        return OrderManager::SUCCESS;
    }
}}
