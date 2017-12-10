cmake_minimum_required(VERSION 3.8)
project(Game)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        arch/Controller.cpp
        arch/Controller.h
        arch/Game.cpp
        arch/Game.h
        arch/General.h
        arch/Hex.cpp
        arch/Hex.h
        arch/IFullAccess.h
        arch/IGetData.h
        arch/ISetData.h
        arch/Main.cpp
        arch/Map.cpp
        arch/Map.h
        arch/Pair.cpp
        arch/Player.cpp
        arch/Player.h
        arch/Unit.cpp
        arch/Unit.h
        arch/View.h
        arch/View.cpp)


add_executable(Game ${SOURCE_FILES})

target_include_directories(Game
        PUBLIC libevent/include)

target_link_libraries(Game
        PUBLIC libevent/lib/libevent.a)