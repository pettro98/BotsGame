#include "View.h"
#include "Hex.h"
#include "curl/curl.h"
#include "nlohmann/json.hpp"
#include <vector>
#include <iostream>

using nlohmann::json;
namespace game_module
{
using vector2hex = std::vector<std::vector<Hex *>>;

json convertMap(const Map &map)
{
    int x = map.dimension_x();
    int y = map.dimension_y();

    vector2hex vec;

    json data = json::array();

    for (int i = 0; i < x; ++i)
    {
        json row = json::array();
        for (int j = 0; j < y; ++j)
        {
            auto hex = *map(i, j);
            auto owner = hex.color() + 1;
            auto unit = hex.get_unit();
            auto type = hex.get_unit_type();
            std::string contents;
            if (type == unit_type::army)
            {
                switch (unit->strength())
                {
                case 1:
                {
                    contents = "a1";
                    break;
                }
                case 2:
                {
                    contents = "a2";
                    break;
                }
                case 3:
                {
                    contents = "a3";
                    break;
                }
                case 4:
                {
                    contents = "a4";
                    break;
                }
                }
            }
            else
                switch (unit->type())
                {
                case unit_type::capital:
                {
                    contents = "ca";
                    break;
                }
                case unit_type::farm:
                {
                    contents = "fa";
                    break;
                }
                case unit_type::grave:
                {
                    contents = "gr";
                    break;
                }
                case unit_type::pine:
                {
                    contents = "pi";
                    break;
                }
                case unit_type::palm:
                {
                    contents = "pa";
                    break;
                }
                case unit_type::tower:
                {
                    if (unit->strength() == 1)
                    {
                        contents = "t1";
                    }
                    else
                    {
                        contents = "t2";
                    }
                    break;
                }
                }
            row.push_back(json({{"owner", owner}, {"contents", contents.c_str()}}));
        }
        data.push_back(row);
    }
    return data;
}

json convertStats(const Result &res)
{
    json stats({{"last_turn", res.last_turn},
                {"built_armies", res.built_armies},
                {"built_farms", res.built_farms},
                {"built_towers", res.built_towers},
                {"moves", res.moves},
                {"winner", res.winner + 1}});
}

View::View(IGetData *getData)
    : MyGetData(getData)
{}

View::~View() = default;

void View::show(const Map &newState, const Result &res)
{
    auto turn = MyGetData->current_turn();
    auto data = convertMap(newState);
    auto stats = convertStats(res);
    json full_Data({{"lastTurn", turn},
                    {"field", data},
                    {"stats", stats}});

    CURL *curl;
    CURLcode resp;

    curl_global_init(CURL_GLOBAL_ALL);

    curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost/data");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, full_Data.dump());

        resp = curl_easy_perform(curl);

        if (resp != CURLE_OK)
            fprintf(stderr, "failed to upload data \n");

        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
}

}