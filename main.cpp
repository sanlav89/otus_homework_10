#include <iostream>
#include "server.h"

int main(int argc, char* argv[])
{
    std::cout << "Hello from join_server!" << std::endl;
    try {
        if (argc != 2) {
            std::cerr << "Usage: async_tcp_echo_server <port>\n";
            return 1;
        }

        boost::asio::io_context io_context;
        join::Server server(io_context, std::atoi(argv[1]));
        io_context.run();

    } catch (const std::exception& ex) {
      std::cerr << "Exception: " << ex.what() << "\n";
    }

    return 0;
}
