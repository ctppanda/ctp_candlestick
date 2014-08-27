#ifndef LOGIN_SESSION_H
#define LOGIN_SESSION_H

#include "Util.h"

namespace Pheux { namespace Core {

class LoginSession
{
public:
    LoginSession() {}
    LoginSession(int front, int sess, int ref, const char* day, const char* time)
        : front_id(front), session_id(sess), max_order_ref(ref),
        login_time(Converter::ToDateTime(day, time))
    {}

    int front_id;
    int session_id;
    int max_order_ref;
    Poco::DateTime login_time;
};

}}

#endif
