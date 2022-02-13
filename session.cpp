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
    std::string answer;

    if (queryArgs.size() == 4 && queryArgs[0] == "INSERT") {
        if (m_db.find(queryArgs[1]) == m_db.end()) {
            answer = "ERR database doesn't contain table " + queryArgs[1];
        } else {
            auto ok = db::query::insert(m_db[queryArgs[1]],
                    std::stoi(queryArgs[2]), {queryArgs[3]});
            if (ok) {
                answer = "OK";
            } else {
                answer = "ERR duplicate " + queryArgs[2];
            }
        }
    } else if (queryArgs.size() == 2 && queryArgs[0] == "TRUNCATE") {
        if (m_db.find(queryArgs[1]) == m_db.end()) {
            answer = "ERR database doesn't contain table " + queryArgs[1];
        } else {
            db::query::truncate(m_db[queryArgs[1]]);
            answer = "OK";
        }
    } else if (queryArgs.size() == 1 && queryArgs[0] == "INTERSECTION") {
        std::ostringstream oss;
        db::printTable(db::query::intersection(m_db["A"], m_db["B"]), oss);
        answer = oss.str() + "OK";
    } else if (queryArgs.size() == 1 && queryArgs[0] == "SYMMETRIC_DIFFERENCE") {
        std::ostringstream oss;
        db::printTable(db::query::symdifference(m_db["A"], m_db["B"]), oss);
        answer = oss.str() + "OK";
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
