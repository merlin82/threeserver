#ifndef _SAN_USER_SERVER_
#define _SAN_USER_SERVER_

#include "public/base/san_app_base.h"

class SanUserServer : public SanAppBase
{
public:
    SanUserServer();
    virtual ~SanUserServer();
public:
    virtual int Init(int argc, char**argv);
    void HandleGetRandomName(netmt::ConnectionPtr conn, SanMessage& msg);
};
#endif

