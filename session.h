#pragma once

#include <memory>
#include <iostream>
#include <boost/asio.hpp>
#include "database.h"

namespace join {

using boost::asio::ip::tcp;
using buffer_t = char;
const int max_length = 1024;

class Session : public std::enable_shared_from_this<Session>
{
public:
    Session(tcp::socket socket, db::DataBase &database);

    void start();

private:

    tcp::socket m_socket;
    buffer_t m_buffer[max_length];
    db::DataBase &m_database;

    void doRead();

    void doWrite(std::size_t length);

    void processQuery(const std::string &query);

};

}
