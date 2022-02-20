#pragma once

#include <memory>
#include "session.h"

namespace join {

class Server
{
public:
    Server(boost::asio::io_context& io_context, short port);

private:
    void doAccept();

    tcp::acceptor m_acceptor;
    db::DataBase m_db;
};

}
