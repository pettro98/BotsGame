cmake_minimum_required(VERSION 3.0)

set(HUNTER_ROOT)

include("gate/cmake/HunterGate.cmake")

HunterGate(
		URL "https://github.com/ruslo/hunter/archive/v0.19.240.tar.gz"
		SHA1 "aa3cd9c45391d8bd14441971b00a43c05d40347c"
)

project(game)
hunter_add_package(CURL)
find_package(CURL CONFIG REQUIRED)
hunter_add_package(nlohmann_json)
find_package(nlohmann_json CONFIG REQUIRED)


set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
		arch/Bot.cpp
		arch/Bot.h
		arch/dynamic.h
        arch/Controller.cpp
        arch/Controller.h
        arch/Game.cpp
        arch/Game.h
		arch/General.cpp
        arch/General.h
        arch/Hex.cpp
        arch/Hex.h
        arch/IGetData.h
        arch/ISetData.h
        arch/IFullAccess.h
        arch/Main.cpp
        arch/Map.cpp
        arch/Map.h
        arch/Player.cpp
        arch/Player.h
        arch/Unit.cpp
        arch/Unit.h
        arch/View.h
        arch/View.cpp
		$ENV{BOT_SOURCES})


add_executable(game ${SOURCE_FILES})

target_link_libraries(game PUBLIC nlohmann_json CURL::libcurl)

target_include_directories(game PUBLIC arch PUBLIC bot_sources)

install(TARGETS game RUNTIME DESTINATION bin)
