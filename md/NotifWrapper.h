#ifndef NOTIF_WRAPPER_H
#define NOTIF_WRAPPER_H

#include "Poco/Notification.h"
#include "Tick.h"

namespace Pheux { namespace Core {

    template<class T>
    class NotifWrapper : public Poco::Notification
    {
    public:
        NotifWrapper(const T* t) : _data(t) {}
        T* data() const { return _data; }

    private:
        T* _data;
    };

    class TickNotif : public NotifWrapper<Tick>
    {
    };

}}

#endif
