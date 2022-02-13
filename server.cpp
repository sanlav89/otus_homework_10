#include "server.h"

namespace join {

Server::Server(boost::asio::io_context& io_context, short port)
    : m_acceptor(io_context, tcp::endpoint(tcp::v4(), port))
{
    initDb();
    doAccept();
}

void Server::initDb()
{
    m_db = {
        {"A", {
             {0, {"lean"}},
             {1, {"sweater"}},
             {2, {"frank"}},
             {3, {"violation"}},
             {4, {"quality"}},
             {5, {"precision"}}
         }
        },
        {"B", {
             {3, {"proposal"}},
             {4, {"example"}},
             {5, {"lake"}},
             {6, {"flour"}},
             {7, {"wonder"}},
             {8, {"selection"}}
         }
        }
    };
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

