#include "message.h"
#include <string.h>
namespace netmt
{
MessagePtr Message::Alloc(uint32_t len)
{
    MessagePtr msg;
    if (len > 0)
    {
        msg.reset(new Message(len));
    }
    return msg;
}

MessagePtr Message::Alloc(const char* data, uint32_t len)
{
    MessagePtr msg;
    if (len > 0)
    {
        msg.reset(new Message(data, len));
    }
    return msg;    
}

Message::Message()
{
    m_data = NULL;
    m_len = 0;
}

Message::Message(uint32_t len)
{
    m_len = len;
    m_data = new char[m_len];
}

Message::Message(const char* data, uint32_t len)
{
    m_len = len;
    m_data = new char[m_len];
    memcpy(m_data, data, m_len);
}

Message::~Message()
{
    if (NULL != m_data)
    {
        delete m_data;
        m_data = NULL;
    }
    m_len = 0;
}

const char* Message::Data() const
{
    return m_data;
}

char* Message::Data()
{
    return m_data;
}

uint32_t Message::Length()
{
    return m_len;
}

}

