#include "server.h"

namespace join {

Server::Server(boost::asio::io_context& io_context, short port)
    : m_acceptor(io_context, tcp::endpoint(tcp::v4(), port))
{
    doAccept();
}

void Server::doAccept()
{
    m_acceptor.async_accept(
        [this](boost::system::error_code error, tcp::socket socket)
        {
            if (!error) {
                std::make_shared<Session>(std::move(socket), m_db)->start();
            }
            doAccept();
        }
    );
}

}

