#include "async_client.h"
#include "server.h"

using namespace std;
namespace netmt
{

AsyncConnection::AsyncConnection(Server* server, const std::string& ip, uint16_t port) : Connection(server), m_ip(ip), m_port(port)
{
    
}

AsyncConnection::~AsyncConnection()
{
    
}

void AsyncConnection::Send(const MessagePtr msg)
{
    if (is_open())
    {
        async_send(boost::asio::buffer(msg->Data(), msg->Length()),
                m_strand.wrap(boost::bind(&AsyncConnection::HandleSend, this,
                        msg, boost::asio::placeholders::error, true)));        
    }
    else
    {
        boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(m_ip), m_port);
        async_connect(endpoint, m_strand.wrap(boost::bind(&AsyncConnection::HandleConnect, this,
                msg, boost::asio::placeholders::error)));
    }
}

void AsyncConnection::HandleConnect(const MessagePtr msg, const boost::system::error_code& e)
{
    if (!e)
    {
        m_server->HandleConnect(Connection::shared_from_this());
        Start();
        async_send(boost::asio::buffer(msg->Data(), msg->Length()),
                m_strand.wrap(boost::bind(&AsyncConnection::HandleSend, this,
                        msg, boost::asio::placeholders::error, false))); 
    }
}

void AsyncConnection::HandleSend(const MessagePtr msg, const boost::system::error_code& e, bool resend)
{
    if (e)
    {
        if (resend)
        {
            boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(m_ip), m_port);
            async_connect(endpoint, m_strand.wrap(boost::bind(&AsyncConnection::HandleConnect, this,
                msg, boost::asio::placeholders::error)));             
        }
        else
        {
            m_server->HandleSendError(Connection::shared_from_this(), msg, e);
        }
    }
}

ASyncClient ASyncClient::s_client;
ASyncClient* ASyncClient::Instance()
{
    return &s_client;
}
ASyncClient::ASyncClient()
{
    
}
ASyncClient::~ASyncClient()
{
    
}

int ASyncClient::Send(Server* server, const std::string& ip, uint16_t port, const MessagePtr msg)
{
    AsyncConnectionPtr conn = GetConnection(server, ip, port);
    conn->Send(msg);
    return 0;    
}

AsyncConnectionPtr ASyncClient::GetConnection(Server* server, const std::string& ip, uint16_t port)
{
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(ip), port);
    AsyncConnectionPtr conn;
    boost::mutex::scoped_lock lock(m_mutex); 
    map<boost::asio::ip::tcp::endpoint, AsyncConnectionPtr>::iterator it = m_conn_map.find(endpoint);
    if (it == m_conn_map.end())
    {
        conn.reset(new AsyncConnection(server, ip, port));
        m_conn_map[endpoint] = conn;       
    }
    else
    {
        conn = it->second;
    }
    
    return conn;
}

}
