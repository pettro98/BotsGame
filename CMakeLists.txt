cmake_minimum_required(VERSION 2.2)

set(HUNTER_ROOT)

include("gate/cmake/HunterGate.cmake")

HunterGate(
        URL "https://github.com/ruslo/hunter/archive/v0.19.160.tar.gz"
        SHA1 "8f27f3adb70e8b42de98183782c4b89c24ad8a76"
)

project(game)

hunter_add_package(CURL)
find_package(CURL CONFIG REQUIRED)

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

target_link_libraries(game CURL::libcurl)

install(TARGETS game RUNTIME DESTINATION bin)
