#ifndef _NETMT_APP_H_
#define _NETMT_APP_H_

#include <string>
#include <boost/thread.hpp>
#include "connection.h"

namespace netmt
{
class App: private boost::noncopyable
{
public:
    App();

    virtual ~App();

    /// Run the app to listen on the specified TCP address and port.
    void Run(const std::string& address, const std::string& port,
            std::size_t thread_pool_size);

    /// Run the app without listen
    void Run(std::size_t thread_pool_size);
    
    /// handle message
    virtual void HandleMessage(ConnectionPtr conn, const MessagePtr msg) = 0;

    /// check message whether complete
    /// return 0:not complete, <0:error, >0:message length
    virtual int CheckComplete(ConnectionPtr conn, const char* data,
            std::size_t data_len) = 0;

    /// handle loop per second
    virtual void HandleLoop();
    
    /// handle connect event
    virtual void HandleConnect(ConnectionPtr conn);

    /// handle disconnect event
    virtual void HandleDisconnect(ConnectionPtr conn);

    /// handle async_send error
    void HandleSendError(ConnectionPtr conn, const MessagePtr msg, const boost::system::error_code& e);

    /// get io_service
    boost::asio::io_service& io_service()
    {
        return m_io_service;
    }
private:
    /// Initiate an asynchronous accept operation.
    void StartAccept();

    /// Handle completion of an asynchronous accept operation.
    void HandleAccept(const boost::system::error_code& e);

    /// Handle a request to stop the server.
    void HandleStop();

    void HandleTimeOut(const boost::system::error_code& error);
    
    boost::thread_group m_thread_grp;

    /// The io_service used to perform asynchronous operations.
    boost::asio::io_service m_io_service;

    /// The signal_set is used to register for process termination notifications.
    boost::asio::signal_set m_signals;

    /// Acceptor used to listen for incoming connections.
    boost::asio::ip::tcp::acceptor m_acceptor;

    boost::asio::deadline_timer m_timer;
    
    /// The next connection to be accepted.
    ConnectionPtr m_new_connection;
};
}
#endif
