#include "session.h"
#include <boost/algorithm/string.hpp>

namespace join {

Session::Session(tcp::socket socket, db::db_t &db)
    : m_socket(std::move(socket))
    , m_db(db)
{
}

void Session::start()
{
    doRead();
}

void Session::doRead()
{
    auto self(shared_from_this());
    m_socket.async_read_some(
        boost::asio::buffer(m_buffer, max_length),
        [this, self](boost::system::error_code error, std::size_t length)
        {
            if (!error) {
                std::cout << "receive " << length
                          << "=" << std::string{m_buffer, length} << std::endl;
                processQuery(std::string{m_buffer, length});
            }
        }
    );
}

void Session::doWrite(std::size_t length)
{
    auto self(shared_from_this());
    boost::asio::async_write(m_socket, boost::asio::buffer(m_buffer, length),
        [this, self](boost::system::error_code error, std::size_t /*length*/)
        {
            if (!error) {
                doRead();
            }
        }
    );
}

void Session::processQuery(const std::string &query)
{
    std::vector<std::string> queryArgs;
    boost::split(queryArgs, query, boost::is_any_of(" "));
//    std::size_t length = 0;
    if (queryArgs.size() == 4 && queryArgs[0] == "INSERT") {

        if (queryArgs[1] != "A" && queryArgs[1] != "B") {

        }

//        if (queryArgs[1] == "A"
//                && db::query::insert(m_tableA, std::stoi(queryArgs[2]), {queryArgs[3]})) {

//        }
    } else if (queryArgs.size() == 2 && queryArgs[0] == "TRUNCATE") {

    } else if (queryArgs.size() == 1 && queryArgs[0] == "INTERSECTION") {

    } else if (queryArgs.size() == 1 && queryArgs[0] == "SYMMETRIC_DIFFERENCE") {

    }

//    doWrite(length);
}

}
