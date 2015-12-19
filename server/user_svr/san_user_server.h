#ifndef _SAN_USER_SERVER_
#define _SAN_USER_SERVER_

#include "public/base/san_server_base.h"

class SanUserServer : public SanServerBase
{
public:
    SanUserServer();
    virtual ~SanUserServer();
public:
    virtual int Init();
    void HandleGetRandomName(netmt::ConnectionPtr conn, SanMessage& msg);
};
#endif
