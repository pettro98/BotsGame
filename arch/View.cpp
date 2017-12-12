#include "View.h"
/*#include "curl/curl.h"
#include "nlohmann/json.hpp"
#include <vector>

using namespace game_module;
using vector2hex = std::vector<std::vector<Hex*>>;
using nlohmann::json;

json convertMap(const Map& map){
    int x = map.dimension_x();
    int y = map.dimension_y();

    vector2hex vec;

    json data = json::array();

    for(int i = 0; i < x; ++i) {
        json row = json::array();
        for (int j = 0; j < y; ++j) {
            auto hex = *map(i, j);
            auto owner = hex.color() + 1;
            auto unit = hex.get_unit();
            auto type = hex.get_unit_type();
            char* contents = "";
            if(type == unit_type::army){
                switch(unit->strength()){
                    case 1: {
                        contents = "1";
                        break;
                    }
                    case 2: {
                        contents = "2";
                        break;
                    }
                    case 3: {
                        contents = "3";
                        break;
                    }
                    case 4: {
                        contents = "4";
                        break;
                    }
                }
            } else switch(unit->type()){
                    case unit_type::capital:{
                        contents = "c";
                        break;
                    }
                    case unit_type::farm:{
                        contents = "f";
                        break;
                    }
                    case unit_type::grave:{
                        contents = "g";
                        break;
                    }
                    case unit_type::pine:{
                        contents = "t1";
                        break;
                    }
                    case unit_type::palm:{
                        contents = "t2";
                        break;
                    }
                    case unit_type::tower:{
                        contents = "t";
                        break;
                    }
                }
            row.push_back(json({{"owner", owner}, {"contents", contents}}));
        }
        data.push_back(row);
    }
    return data;
}

json convertStats(const Result& res){
    json stats({{"last_turn", res.last_turn},
               {"built_armies", res.built_armies},
                {"built_farms", res.built_farms},
                {"built_towers", res.built_towers},
                {"moves", res.moves},
                {"winner", res.winner + 1}});
}

View::View(IGetData* getData)
        : MyGetData(getData)
{}

View::~View() = default;

void View::show(const Map & newState){
    auto turn = MyGetData->current_turn();
    auto data = convertMap(newState);
    auto stats = convertStats(MyGetData->results());
    json full_Data({{"turn", turn},
                    {"data", data},
                    {"stats", stats}});

    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_ALL);

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost/game/data");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, full_Data.dump());

        res = curl_easy_perform(curl);

        if(res != CURLE_OK)
            fprintf(stderr, "failed to upload data \n");

        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
}*/