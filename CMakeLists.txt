cmake_minimum_required(VERSION 3.0)
project(SDL2Game)

set(CMAKE_CXX_STANDARD 17)

set(SDL2_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/SDL2/include)
set(SDL2_LIBRARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/SDL2/lib)

set(GNF_ROOT "C://Data//CPP//GameNetworkFramework")
set(GNF_INCLUDE_DIR ${GNF_ROOT} ${GNF_ROOT}/Third/asio ${GNF_ROOT}/Third/fmt)

set(GNF_LIBRARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../GameNetworkFramework/cmake-build-debug-mingw)


set(GAME_SRCS
        Game/Player.cpp
        Game/Object.cpp
        Game/SDLUtils.h
        )

add_executable(SDL2Game main.cpp ${GAME_SRCS})
target_include_directories(SDL2Game PUBLIC ${SDL2_INCLUDE_DIR} ${GNF_INCLUDE_DIR})
target_link_directories(SDL2Game PUBLIC ${SDL2_LIBRARY_DIR} ${GNF_LIBRARY_DIR})
target_link_libraries(SDL2Game PRIVATE mingw32 SDL2main SDL2 SDL2_image SDL2_ttf
        client_pack wsock32 ws2_32 )