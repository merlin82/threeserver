#ifndef _NETMT_MSG_
#define _NETMT_MSG_
#include <stdint.h>
#include <boost/shared_ptr.hpp>

namespace netmt
{
class Message;
typedef boost::shared_ptr<Message> MessagePtr;

class Message
{
public:
    static MessagePtr Alloc(uint32_t len);
    static MessagePtr Alloc(const char* data, uint32_t len);
    ~Message();
public:
    const char* Data() const;
    char* Data();
    uint32_t Length();    
private:
    Message();
    Message(uint32_t len);
    Message(const char* data, uint32_t len);
    

private:
    char* m_data;
    uint32_t m_len;
};

}
#endif
