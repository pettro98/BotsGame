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
                    if (type == unit_type::army) {
                        switch (unit->strength()) {
                            case 1: {
                                contents = "a1";
                                break;
                            }
                            case 2: {
                                contents = "a2";
                                break;
                            }
                            case 3: {
                                contents = "a3";
                                break;
                            }
                            case 4: {
                                contents = "a4";
                                break;
                            }
                        }
                    } else
                        switch (unit->type()) {
                            case unit_type::capital: {
                                contents = "ca";
                                break;
                            }
                            case unit_type::farm: {
                                contents = "fa";
                                break;
                            }
                            case unit_type::grave: {
                                contents = "gr";
                                break;
                            }
                            case unit_type::pine: {
                                contents = "pi";
                                break;
                            }
                            case unit_type::palm: {
                                contents = "pa";
                                break;
                            }
                            case unit_type::tower: {
                                if (unit->strength() == 1) {
                                    contents = "t1";
                                } else {
                                    contents = "t2";
                                }
                                break;
                            }
                        }
                }
                row.push_back(json({{"owner",    owner},
                                    {"contents", contents.c_str()}}));
            }
            data.push_back(row);
        }
        return data;
    }

    json convertStats(const Result &res) {
        json stats({{"last_turn",    res.last_turn},
                    {"built_armies", res.built_armies},
                    {"built_farms",  res.built_farms},
                    {"built_towers", res.built_towers},
                    {"moves",        res.moves},
                    {"winner",       res.winner + 1}});
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
        struct curl_slist *chunk = NULL;

        chunk = curl_slist_append(chunk, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);

        res = curl_easy_perform(curl);
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));

        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
}


    void View::show(const Map &newState, const Result &res) {
        auto turn = MyGetData->current_turn();
        auto data = convertMap(newState);
        auto stats = convertStats(res);
        json full_Data({{"lastTurn", turn},
                        {"field",    data},
                        {"stats",    stats}});

        showCURL(full_Data.dump());

//        showCURL("[[[\"lol\"],[\"kek\"]]]");

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

