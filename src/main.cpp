#include <boost/asio/ip/tcp.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <nlohmann/json.hpp>

#include "request_handler.hpp"
#include "tab_handler.hpp"
#include "listener.hpp"

namespace net = boost::asio;    // from <boost/asio.hpp>
using tcp = net::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

using json = nlohmann::json;

json load_json_config(const std::string& filename)
{
    std::ifstream file(filename);
    json config;
    file >> config;
    return config;
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        throw std::runtime_error("Not enough arguments. Config filepath required.");
    }

    auto config = load_json_config(argv[1]);


    try
    {
        auto const address = net::ip::make_address(config["address"]);
        unsigned short port = config["port"];

        net::io_context ioc{1};

        auto listener = std::make_shared<Listener<TabHandler>>(ioc, tcp::endpoint{address, port});

        std::cout <<"Starting server..." <<std::endl;
        ioc.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}