#include "san_user_server.h"
#include "proto/user.pb.h"

SanUserServer::SanUserServer()
{
    
}

SanUserServer::~SanUserServer()
{
    
}

int SanUserServer::Init(int argc, char**argv)
{
    int ret = SanServerBase::Init(argc, argv);
    if (ret != 0)
    {
        return -1;
    }
    Register(GET_RANDOM_NAME_REQ.number(), boost::bind(&SanUserServer::HandleGetRandomName, this, _1, _2));
    return 0;
}

void SanUserServer::HandleGetRandomName(netmt::ConnectionPtr conn, SanMessage& msg)
{
    GetRandomNameReq* req = msg.MutableExtension(GET_RANDOM_NAME_REQ);
    req->number();
}

int main(int argc, char** argv)
{
    //daemon(1, 1);
    SanUserServer s;
    if (0 != s.Init(argc, argv))
    {
        return -1;
    }
    s.Run();
    return 0;
}

