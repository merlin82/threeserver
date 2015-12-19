#include "san_msg.h"
#include <arpa/inet.h>
#include "public/netmt/sync_client.h"
#include "public/netmt/async_client.h"
using namespace netmt;

netmt::Server *SanMsgHelper::s_server = NULL;
int SanMsgHelper::Decode(const MessagePtr raw_msg, SanMessage& san_msg)
{
    if(!san_msg.ParseFromArray(raw_msg->Data() + sizeof(SanMsgHead), 
        raw_msg->Length() - sizeof(SanMsgHead)))
    {
        return -1;
    }
    return 0;
}

int SanMsgHelper::Encode(const SanMessage& san_msg, MessagePtr raw_msg)
{
    raw_msg = Message::Alloc(san_msg.ByteSize() + sizeof(SanMsgHead));
    if(!san_msg.SerializeToArray(raw_msg->Data() + sizeof(SanMsgHead), 
            raw_msg->Length() - sizeof(SanMsgHead)))
    {
        return -1;
    }
    SanMsgHead* head = (SanMsgHead*)raw_msg->Data();
    head->magic = SAN_MAGIC;
    head->reserved[0] = 0;
    head->reserved[1] = 0;
    head->len = htonl(raw_msg->Length());
    return 0;
}

int SanMsgHelper::CheckComplete(const char* data, std::size_t data_len)
{
    if (data[0] != SAN_MAGIC)
    {
        return -1;
    }
    if (data_len < sizeof(SanMsgHead))
    {
        return 0;
    }
    SanMsgHead* head = (SanMsgHead*)data;
    uint32_t len = htonl(head->len);
    if(data_len >= len)
    {
        return len;
    }
    return 0;
}

void SanMsgHelper::AsyncSend(const std::string& ip, uint16_t port, const SanMessage& san_msg)
{
    if (NULL == s_server)
    {
        return;
    }
    MessagePtr raw_msg;
    int ret = Encode(san_msg, raw_msg);
    if (ret != 0)
    {
        return;
    }    
    ASyncClient::Instance()->Send(s_server, ip, port, raw_msg);
}

int SanMsgHelper::SendAndRecv(const std::string& ip, uint16_t port, const SanMessage& req, SanMessage& rsp)
{
    MessagePtr raw_msg;
    int ret = Encode(req, raw_msg);
    if (ret != 0)
    {
        return ret;
    }
    char * rsp_data = NULL;
    uint32_t rsp_data_len = 0;
    ret = SyncClient::Instance()->SendAndRecv(ip, port, raw_msg->Data(), raw_msg->Length(), 
        rsp_data, rsp_data_len, 10000, SanMsgHelper::CheckComplete);
    if (ret != 0)
    {
        return ret;
    }

    if (!rsp.ParseFromArray(rsp_data + sizeof(SanMsgHead), rsp_data_len - sizeof(SanMsgHead)))
    {
        return -1;
    }
    return 0;
}
 