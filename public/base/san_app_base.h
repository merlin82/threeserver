#ifndef _SAN_APP_BASE_
#define _SAN_APP_BASE_

#include <boost/function.hpp>
#include <boost/bind.hpp>
#include "public/netmt/app.h"
#include "public/netmt/connection.h"
#include "san_msg.h"
typedef boost::function<void (netmt::ConnectionPtr conn, SanMessage& msg)> CmdFunc;

class SanAppBase : public netmt::App
{
public:
    SanAppBase();
    virtual ~SanAppBase();
public:
    virtual int Init(int argc, char**argv);

    void Run();
    
    void Register(int32_t cmdid, CmdFunc func);
public:    
    /// handle message
    virtual void HandleMessage(netmt::ConnectionPtr conn, const netmt::MessagePtr msg);

    /// check message whether complete
    /// return 0:not complete, <0:error, >0:message length
    virtual int CheckComplete(netmt::ConnectionPtr conn, const char* data,
            std::size_t data_len);

    virtual void HandleLoop();
    
    /// handle connect event
    virtual void HandleConnect(netmt::ConnectionPtr conn);

    /// handle disconnect event
    virtual void HandleDisconnect(netmt::ConnectionPtr conn);

    /// handle async_send error
    void HandleSendError(netmt::ConnectionPtr conn, const netmt::MessagePtr msg, const boost::system::error_code& e);
private:
    std::map<int32_t, CmdFunc> m_cmd_map;
};
#endif

