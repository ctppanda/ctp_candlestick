#ifndef DISPATCHER_H
#define DISPATCHER_H

#include "Poco/NotificationCenter.h"
#include "Poco/Notification.h"
#include "Poco/Observer.h"
#include "Poco/NObserver.h"
#include "Poco/AutoPtr.h"

namespace Pheux { namespace Core {

    class Dispatcher
    {
    public:
        Dispatcher()
        {
            // if we want to change tick driven to bar driven, only need to use bar-handler to replace tick-handler
            nc.addObserver(Observer<Target, TickNotif>(target, &Target::handleTick));
            nc.addObserver(Observer<Target, CallBackNotif>(target, &Target::handleCallback));
        }
        ~Dispatcher()
        {
            nc.removeObserver(Observer<Target, TickNotif>(target, &Target::handleTick));
            nc.removeObserver(Observer<Target, CallBackNotif>(target, &Target::handleCallback));
        }

        template <class C, class N>
        void RegisterObserver(target, callback)
        {
            typedef void (C::*Callback)(N*);
            nc.addObserver(Observer<C, Callback>(target, &callback));
        }

        // it is not thread safe.
        // how to keep sure? lock?
        // if there is lock, so still not so effective
        void Dispatch(Notification* notif)
        {
            queue.push(notif);
            Notification* it = queue.front();
            queue.pop();
            // 1, if there is multiple ticks, process them to a new tick
            // 2, if (it is tick and previous tick hasn't finished) skip or let callback first
            //
            nc.postNotification(it);
        }

        // 1, is it thread safe?
        // 2, consider priority_queue, callback is more important
        std::queue<Notification*> queue;
        Poco::NotificationCenter nc;
        Target target;
    };

}}

#endif
