#include "session.h"
#include <boost/algorithm/string.hpp>

namespace join {

Session::Session(tcp::socket socket, db::DataBase &database)
    : m_socket(std::move(socket))
    , m_database(database)
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
    std::string answer;

    if (queryArgs.size() == 4 && queryArgs[0] == "INSERT") {
        answer = m_database.insert(queryArgs[1], std::stoi(queryArgs[2]), queryArgs[3]);
    } else if (queryArgs.size() == 2 && queryArgs[0] == "TRUNCATE") {
        answer = m_database.truncate(queryArgs[1]);
    } else if (queryArgs.size() == 1 && queryArgs[0] == "INTERSECTION") {
        std::ostringstream oss;
        answer = m_database.intersection();
    } else if (queryArgs.size() == 1 && queryArgs[0] == "SYMMETRIC_DIFFERENCE") {
        answer = m_database.symdifference();
    } else {
        answer = "ERR supported commands:\n"
                 "    INSERT table id name\n"
                 "    TRUNCATE table\n"
                 "    INTERSECTION\n"
                 "    SYMMETRIC_DIFFERENCE\n"
                 ;
    }

    std::memcpy(m_buffer, answer.c_str(), answer.size());
    doWrite(answer.size());
}

}
