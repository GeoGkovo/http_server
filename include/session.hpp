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
#include <fstream>


namespace beast = boost::beast; // from <boost/beast.hpp>
namespace http = beast::http;   // from <boost/beast/http.hpp>
namespace net = boost::asio;    // from <boost/asio.hpp>
using tcp = net::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

// This class handles an HTTP server connection.
template <typename Handler>
class Session : public std::enable_shared_from_this<Session<Handler>>
{
    tcp::socket socket_;
    beast::flat_buffer buffer_;
    http::request<http::string_body> req_;

public:
    explicit Session(tcp::socket socket) : socket_(std::move(socket)) {}

    void run()
    {
        do_read();
    }

private:
    void do_read()
    {
        auto self(this->shared_from_this());
        http::async_read(socket_, buffer_, req_, [this, self](beast::error_code ec, std::size_t)
                         {
            if (!ec) {
                do_write(handler_.handle_request(req_));
            } });
    }

    void do_write(http::response<http::string_body> res)
    {
        auto self(this->shared_from_this());
        auto sp = std::make_shared<http::response<http::string_body>>(std::move(res));
        http::async_write(socket_, *sp, [this, self, sp](beast::error_code ec, std::size_t)
                          { socket_.shutdown(tcp::socket::shutdown_send, ec); });
    }

    RequestHandler<Handler> handler_;
};
