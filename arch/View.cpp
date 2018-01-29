#include "View.h"
#include "Hex.h"
#include "curl/curl.h"
#include "nlohmann/json.hpp"
#include <boost/asio.hpp>
#include <vector>
#include <iostream>
#include <string>

using namespace boost::asio;

using nlohmann::json;

namespace game_module {

    using vector2hex = std::vector<std::vector<Hex *>>;

    json convertMap(const Map &map) {
        int x = map.dimension_x();
        int y = map.dimension_y();

        vector2hex vec;

        json data = json::array();

        for (int i = 0; i < x; ++i) {
            json row = json::array();
            for (int j = 0; j < y; ++j) {
                const auto hex = map(i, j);
                const auto owner = hex->color() + 1;
                const auto unit = hex->get_unit();
                const auto type = hex->get_unit_type();
                std::string contents;
                if (type != unit_type::none) {
                    int count = 0;
                    switch (unit->type()) {
                        case unit_type::army: //h, i, j, k
                            contents += 'g' + unit->strength();
                            break;
                        case unit_type::tower: //f, g
                            count += unit->strength();
                        case unit_type::palm: //e
                            count += 1;
                        case unit_type::pine: //d
                            count += 1;
                        case unit_type::grave: //c
                            count += 1;
                        case unit_type::farm: //b
                            count += 1;
                        case unit_type::capital: //a
                            content += 'a' + count;
                    }
                }
                row.push_back(json({{"O",            owner},
                                    {"C", contents.c_str()}}));
            }
            data.push_back(row);
        }
        return data;
    }

    json convertStats(const Result &res) {
        json stats({{"LT",    res.last_turn},
                    {"BA", res.built_armies},
                    {"BF",  res.built_farms},
                    {"BT", res.built_towers},
                    {"M",         res.moves},
                    {"W",    res.winner + 1}});
        return stats;
    }

    View::View(IGetData *getData)
            : MyGetData(getData) {}

    View::~View() = default;

void showCURL(const std::string &requestData){
    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_ALL);

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:8080/data");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, requestData.c_str());
        struct curl_slist *headers = NULL;

        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, "Expect:");

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        curl_slist_free_all(headers);

        res = curl_easy_perform(curl);
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));

        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
}


    void View::show(const Map &newState, const Result &res) {
        auto current_turn = MyGetData->current_turn();
        auto data = convertMap(newState);
        auto stats = convertStats(res);
        json full_Data({{"CT", current_turn},
                        {"F",    data},
                        {"S",    stats}});

        showCURL(full_Data.dump());

//        boost::system::error_code ec;
//        io_service io;
//        ip::tcp::resolver resolver(io);
//        ip::tcp::resolver::query query("localhost", "8080");
//        ip::tcp::resolver::iterator iter = resolver.resolve(query, ec);
//        if(!ec) {
//            auto ep = (*iter).endpoint();
//            ip::tcp::socket sock(io);
//            sock.set_option(ip::tcp::socket::reuse_address(true));
//            sock.open(ip::tcp::v4());
//            sock.connect(ep);
//            sock.write_some(buffer(request));
//            char buff[1024 * 2]; sock.read_some(buffer(buff, 1024 * 2));
//            sock.shutdown(ip::tcp::socket::shutdown_receive);
//            sock.close();
//        } else {
//            std::cout << ec << std::endl;
//        }
    }
}

