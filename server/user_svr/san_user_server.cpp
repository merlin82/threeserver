#include "san_user_server.h"
#include "proto/user.pb.h"

SanUserServer::SanUserServer()
{
    
}

SanUserServer::~SanUserServer()
{
    
}

int SanUserServer::Init()
{
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
    daemon(0, 0);
    SanUserServer s;
    s.Run("127.0.0.1", "30001", 10);
    return 0;
}

