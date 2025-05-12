#include "tab_handler.hpp"
#include <nlohmann/json.hpp>
#include "server_side_utility.hpp"

http::response<http::string_body> TabHandler::handle_request(http::request<http::string_body> const &req)
{
    if (req.method() == http::verb::options)
    {
        http::response<http::string_body> res{http::status::ok, req.version()};
        res.set(http::field::access_control_allow_origin, "*");
        res.set(http::field::access_control_allow_methods, "POST, OPTIONS");
        res.set(http::field::access_control_allow_headers, "Content-Type");
        res.set(http::field::access_control_max_age, "3600"); // Optional: Cache preflight response for 1 hour
        res.prepare_payload();
        return res;
    }
    if (req.method() == http::verb::post && req.target() == "/api/v1/convert")
    {
        // std::cout << "Conversion request received..." << std::endl;
        std::cout << req.body() << std::endl;
        auto data = parse_input_json(req.body());
        std::string output_tab;
        nlohmann::json json_response;
        /*
        if (data)
        {
            std::cout << "Input data parsed" << std::endl;
            auto error = create_transformed_tab(data.value(), output_tab);
            if (!error)
            {
                json_response = {
                    {"outputTab", output_tab},
                    {"message", "Operation successful"}};
            }
            else
            {
                json_response = {
                    {"outputTab", "|-------------|"},
                    {"message", error.value()}};
            }
        }
            */

        http::response<http::string_body> res{http::status::ok, req.version()};
        res.set(http::field::server, "Beast");
        res.set(http::field::content_type, "application/json");
        // res.keep_alive(req.keep_alive());
        res.set(http::field::access_control_allow_origin, "*"); // Allow all origins
        res.body() = json_response.dump();
        res.prepare_payload();
        return res;
    }

    // Default response for unsupported methods
    std::cout << "Defaulting..." << " Request method was " << req.method_string() << " Request target was " << req.target() << std::endl;
    return http::response<http::string_body>{http::status::bad_request, req.version()};
}
