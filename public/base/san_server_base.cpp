#include "san_server_base.h"
using namespace netmt;

SanServerBase::SanServerBase()
{
    SanMsgHelper::SetServer(this);
}

SanServerBase::~SanServerBase()
{
}

int SanServerBase::Init()
{
    return 0;
}

void SanServerBase::Register(int32_t cmdid, CmdFunc func)
{
    m_cmd_map[cmdid] = func;
}

void SanServerBase::HandleMessage(ConnectionPtr conn, const MessagePtr msg)
{
    SanMessage san_msg;
    int ret = SanMsgHelper::Decode(msg, san_msg);
    if (ret != 0)
    {
        return;
    }
    std::map<int32_t, CmdFunc>::iterator it = m_cmd_map.find(san_msg.cmdid());
    if (it == m_cmd_map.end())
    {
        return;
    }
    (it->second)(conn, san_msg);
}

int SanServerBase::CheckComplete(ConnectionPtr conn, const char* data,
        std::size_t data_len)
{
    return SanMsgHelper::CheckComplete(data, data_len);
}

void SanServerBase::HandleConnect(ConnectionPtr conn)
{
    
}

void SanServerBase::HandleDisconnect(ConnectionPtr conn)
{
    
}

void SanServerBase::HandleSendError(ConnectionPtr conn, const MessagePtr msg, const boost::system::error_code& e)
{
    
}
