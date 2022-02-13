#include <iostream>
#include <thread>
#include <boost/asio.hpp>

namespace ba = boost::asio;

int main()
{
    std::locale::global(std::locale(""));

    try {
        auto ip = ba::ip::address::from_string("127.0.0.1");
        char buffer[1024];
        ba::io_context io_context;
        ba::ip::tcp::endpoint ep(ip, 9000);
        ba::ip::tcp::socket sock(io_context);
        sock.connect(ep);

        std::string query;
        while (std::getline(std::cin, query)) {
            if (!query.empty()) {
                ba::write(sock, ba::buffer(query.c_str(), query.size()));
                size_t len = sock.read_some(ba::buffer(buffer));
                std::cout << std::string{buffer, len} << std::endl;
            }
        }

        sock.close();

    } catch(const boost::system::system_error& ex) {
        std::cout << "boost exception! " << ex.what() << std::endl;
    } catch(const std::exception& ex) {
        std::cout << "std::exception! " << ex.what() << std::endl;
    }

    return 0;
}
