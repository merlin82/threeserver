#include "san_app_base.h"
#include "config.h"
#include <iostream>
using namespace std;
using namespace netmt;

SanAppBase::SanAppBase()
{
    SanMsgUtil::SetApp(this);
}

SanAppBase::~SanAppBase()
{
}

int SanAppBase::Init(int argc, char**argv)
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

void SanAppBase::Run()
{
    LOG(INFO) << "start app: " << GFLAGS_NAMESPACE::GetArgv0();
    try
    {
        if (FLAGS_listen_ip.empty())
        {
            App::Run(FLAGS_thread_pool_size);
        }
        else
        {
            App::Run(FLAGS_listen_ip, FLAGS_listen_port, FLAGS_thread_pool_size);            
        }
    }
    catch(...)
    {
        LOG(ERROR) << "catch exception";
    }
    LOG(INFO) << "stop app: " << GFLAGS_NAMESPACE::GetArgv0();
}

void SanAppBase::Register(int32_t cmdid, CmdFunc func)
{
    m_cmd_map[cmdid] = func;
}

void SanAppBase::HandleMessage(ConnectionPtr conn, const MessagePtr msg)
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
        LOG(ERROR) << "SanAppBase::HandleMessage, catch exception.";
    }
}

int SanAppBase::CheckComplete(ConnectionPtr conn, const char* data,
        std::size_t data_len)
{
    return SanMsgUtil::CheckComplete(data, data_len);
}

void SanAppBase::HandleLoop()
{
    
}

void SanAppBase::HandleConnect(ConnectionPtr conn)
{
    LOG(INFO) << conn->remote_endpoint() << " connect";
}

void SanAppBase::HandleDisconnect(ConnectionPtr conn)
{
    LOG(INFO) << conn->remote_endpoint() << " disconnect";
}

void SanAppBase::HandleSendError(ConnectionPtr conn, const MessagePtr msg, const boost::system::error_code& e)
{
    LOG(INFO) << conn->remote_endpoint() << " send error";
}

