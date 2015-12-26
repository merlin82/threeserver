#include "app.h"
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>

namespace netmt
{

App::App() :m_signals(m_io_service), m_acceptor(m_io_service), m_timer(m_io_service)
{
    // Register to handle the signals that indicate when the App should exit.
    // It is safe to register for the same signal multiple times in a program,
    // provided all registration for the specified signal is made through Asio.
    m_signals.add(SIGINT);
    m_signals.add(SIGTERM);
#if defined(SIGQUIT)
    m_signals.add(SIGQUIT);
#endif // defined(SIGQUIT)
    m_signals.async_wait(boost::bind(&App::HandleStop, this));

    m_timer.expires_from_now(boost::posix_time::milliseconds(1000));
    m_timer.async_wait(
            boost::bind(&App::HandleTimeOut, this,
                    boost::asio::placeholders::error));
}

App::~App()
{

}

void App::Run(const std::string& address, const std::string& port,
            std::size_t thread_pool_size)
{
    // Open the acceptor with the option to reuse the address (i.e. SO_REUSEADDR).
    boost::asio::ip::tcp::resolver resolver(m_io_service);
    boost::asio::ip::tcp::resolver::query query(address, port);
    boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);
    m_acceptor.open(endpoint.protocol());
    m_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    m_acceptor.set_option(boost::asio::ip::tcp::acceptor::linger(true, 0));  
    m_acceptor.set_option(boost::asio::socket_base::keep_alive(true));
    m_acceptor.bind(endpoint);
    m_acceptor.listen();

    StartAccept();
    
    for (std::size_t i = 0; i < thread_pool_size; ++i)
    {
        m_thread_grp.create_thread(
                boost::bind(&boost::asio::io_service::run, &m_io_service));
    }

    m_thread_grp.join_all();
}



void App::Run(std::size_t thread_pool_size)
{
    for (std::size_t i = 0; i < thread_pool_size; ++i)
    {
        m_thread_grp.create_thread(
                boost::bind(&boost::asio::io_service::run, &m_io_service));
    }

    m_thread_grp.join_all();    
}


void App::HandleLoop()
{
    
}

void App::StartAccept()
{
    m_new_connection.reset(new Connection(this));
    m_acceptor.async_accept(*m_new_connection,
            boost::bind(&App::HandleAccept, this,
                    boost::asio::placeholders::error));
}

void App::HandleAccept(const boost::system::error_code& e)
{
    if (!e)
    {
        HandleConnect(m_new_connection);
        m_new_connection->Start();
    }

    StartAccept();
}

void App::HandleStop()
{
    m_io_service.stop();
}

void App::HandleTimeOut(const boost::system::error_code& error)
{
    HandleLoop();
    m_timer.expires_from_now(boost::posix_time::milliseconds(1000));
    m_timer.async_wait(
            boost::bind(&App::HandleTimeOut, this,
                    boost::asio::placeholders::error));    
}

void App::HandleConnect(ConnectionPtr conn)
{

}

void App::HandleDisconnect(ConnectionPtr conn)
{

}

void App::HandleSendError(ConnectionPtr conn, const MessagePtr msg,
        const boost::system::error_code& e)
{

}

}
