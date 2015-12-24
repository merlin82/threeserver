#include "san_server_base.h"
#include "config.h"
#include <iostream>
using namespace std;
using namespace netmt;

SanServerBase::SanServerBase()
{
    SanMsgUtil::SetServer(this);
}

SanServerBase::~SanServerBase()
{
}

int SanServerBase::Init(int argc, char**argv)
{
    if (argc != 2)
    {
        cout << "Usage:\n\t" << basename(argv[0]) << " config_file" << endl;
        return -1;
    }
    GFLAGS_NAMESPACE::ParseCommandLineFlags(&argc, &argv, false);
    if(!GFLAGS_NAMESPACE::ReadFromFlagsFile(argv[1], basename(argv[0]), false))
    {
        cout << "load configuration file failed." << endl;
        return -1;
    }
    google::InitGoogleLogging(GFLAGS_NAMESPACE::GetArgv0());
    
    return 0;
}

void SanServerBase::Run()
{
    LOG(INFO) << "start server: " << GFLAGS_NAMESPACE::GetArgv0();
    try
    {
        Server::Run(FLAGS_listen_ip, FLAGS_listen_port, FLAGS_thread_pool_size);
    }
    catch(...)
    {
        LOG(ERROR) << "catch exception";
    }
    LOG(INFO) << "stop server: " << GFLAGS_NAMESPACE::GetArgv0();
}

void SanServerBase::Register(int32_t cmdid, CmdFunc func)
{
    m_cmd_map[cmdid] = func;
}

void SanServerBase::HandleMessage(ConnectionPtr conn, const MessagePtr msg)
{
    try
    {
        SanMessage san_msg;
        int ret = SanMsgUtil::Decode(msg, san_msg);
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
    catch(...)
    {
        LOG(ERROR) << "SanServerBase::HandleMessage, catch exception.";
    }
}

int SanServerBase::CheckComplete(ConnectionPtr conn, const char* data,
        std::size_t data_len)
{
    return SanMsgUtil::CheckComplete(data, data_len);
}

void SanServerBase::HandleConnect(ConnectionPtr conn)
{
    LOG(INFO) << conn->remote_endpoint() << " connect" << endl;
}

void SanServerBase::HandleDisconnect(ConnectionPtr conn)
{
    LOG(INFO) << conn->remote_endpoint() << " disconnect"  << endl;
}

void SanServerBase::HandleSendError(ConnectionPtr conn, const MessagePtr msg, const boost::system::error_code& e)
{
    LOG(INFO) << conn->remote_endpoint() << " send error"  << endl;
}

