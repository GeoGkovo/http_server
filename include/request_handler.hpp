#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>

namespace beast = boost::beast; // from <boost/beast.hpp>
namespace http = beast::http;
template <typename HandlerImpl>
class RequestHandler
{
public:
    http::response<http::string_body> handle_request(http::request<http::string_body> const &req)
    {
        return handler_.handle_request(req);
    }

private:
    HandlerImpl handler_;
};

