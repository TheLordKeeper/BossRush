cmake_minimum_required(VERSION 3.16)
project(Game.cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Curses REQUIRED)

add_executable(Game
  main.cpp
  src/game/Game.cpp
  src/characters/Character.cpp
)

target_link_libraries(Game PRIVATE ${CURSES_LIBRARIES})
target_include_directories(Game PRIVATE ${CURSES_INCLUDE_DIRS})
