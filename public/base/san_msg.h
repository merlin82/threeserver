#ifndef _SAN_MSG_
#define _SAN_MSG_
#include <stdint.h>
#include "public/netmt/message.h"
#include "public/netmt/app.h"
#include "proto/common.pb.h"

const char SAN_MAGIC = 0xFE;

struct SanMsgHead
{
    char magic;
    char type;
    char reserved[2];
    uint32_t len;
};

class SanMsgUtil
{
public:
    static void SetApp(netmt::App* app) {s_app = app;}
    static int Decode(const netmt::MessagePtr& raw_msg, SanMessage& san_msg);
    static int Encode(const SanMessage& san_msg, netmt::MessagePtr& raw_msg);
    static int CheckComplete(const char* data, std::size_t data_len);
    static void AsyncSend(const std::string& ip, uint16_t port, const SanMessage& san_msg);
    static int SendAndRecv(const std::string& ip, uint16_t port, const SanMessage& req, SanMessage& rsp);
    static int Response(netmt::ConnectionPtr conn, const SanMessage& san_msg);
private:
    static netmt::App *s_app;
};
#endif

