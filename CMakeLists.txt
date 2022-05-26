cmake_minimum_required(VERSION 3.0)
project(SDL2Game)

set(CMAKE_CXX_STANDARD 17)

set(SDL2_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/SDL2/include)
set(SDL2_LIBRARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/SDL2/lib)

set(GAME_SRCS
        main.cpp
        Player.cpp
        Object.cpp
        Controller.cpp
        GameCommon.h
        SDLUtils.h
        )

add_executable(SDL2Game ${GAME_SRCS})
target_include_directories(SDL2Game PUBLIC ${SDL2_INCLUDE_DIR})
target_link_directories(SDL2Game PUBLIC ${SDL2_LIBRARY_DIR})
target_link_libraries(SDL2Game PUBLIC mingw32 SDL2main SDL2 SDL2_image SDL2_ttf)