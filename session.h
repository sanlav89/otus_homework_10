#pragma once

#include <memory>
#include <iostream>
#include <boost/asio.hpp>
#include "db.h"

namespace join {

using boost::asio::ip::tcp;
using buffer_t = char;
const int max_length = 1024;

class Session : public std::enable_shared_from_this<Session>
{
public:
    Session(tcp::socket socket, db::DataBase &db);

    void start();

private:

    tcp::socket m_socket;
    buffer_t m_buffer[max_length];
    db::DataBase &m_db;

    void doRead();

    void doWrite(std::size_t length);

    void processQuery(const std::string &query);

};

}
