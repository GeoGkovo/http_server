#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>

namespace beast = boost::beast; // from <boost/beast.hpp>
namespace http = beast::http;

class TabHandler
{
public:
    http::response<http::string_body> handle_request(http::request<http::string_body> const &req);

};
