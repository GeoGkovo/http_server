#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/strand.hpp>
#include <boost/config.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <nlohmann/json.hpp>
#include <fstream>

#include "session.hpp"

namespace beast = boost::beast; // from <boost/beast.hpp>
namespace http = beast::http;   // from <boost/beast/http.hpp>
namespace net = boost::asio;    // from <boost/asio.hpp>
using tcp = net::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

// This class accepts incoming connections and launches the sessions.
template <typename Handler>
class Listener : public std::enable_shared_from_this<Listener<Handler>>
{
    net::io_context &ioc_;
    tcp::acceptor acceptor_;

public:
    Listener(net::io_context &ioc, tcp::endpoint endpoint)
        : ioc_(ioc), acceptor_(net::make_strand(ioc))
    {
        beast::error_code ec;

        // Open the acceptor
        acceptor_.open(endpoint.protocol(), ec);
        if (ec)
        {
            std::cerr << "Open error: " << ec.message() << std::endl;
            return;
        }

        // Allow address reuse
        acceptor_.set_option(net::socket_base::reuse_address(true), ec);
        if (ec)
        {
            std::cerr << "Set option error: " << ec.message() << std::endl;
            return;
        }

        // Bind to the server address
        acceptor_.bind(endpoint, ec);
        if (ec)
        {
            std::cerr << "Bind error: " << ec.message() << std::endl;
            return;
        }

        // Start listening for connections
        acceptor_.listen(net::socket_base::max_listen_connections, ec);
        if (ec)
        {
            std::cerr << "Listen error: " << ec.message() << std::endl;
            return;
        }

        do_accept();
    }

private:
    void do_accept()
    {
        acceptor_.async_accept(net::make_strand(ioc_), [this](beast::error_code ec, tcp::socket socket)
                               {
            if (!ec) {
                std::make_shared<Session<Handler>>(std::move(socket))->run();
            }
            do_accept(); });
    }
};
