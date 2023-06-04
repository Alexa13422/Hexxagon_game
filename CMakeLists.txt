cmake_minimum_required(VERSION 3.25)
project(Hex)

set(CMAKE_CXX_STANDARD 23)

set(BUILD_SHARED_LIBS FALSE)
include(FetchContent)
FETCHCONTENT_DECLARE(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
)

FETCHCONTENT_MAKEAVAILABLE(SFML)

add_executable(Hex main.cpp StartWindow.h GraphicalBoard.h Board.h Variables.h Game.h AI.h ScoreBoard.h)

target_link_libraries(Hex
        sfml-system
        sfml-window
        sfml-graphics
        )
