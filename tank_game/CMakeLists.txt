cmake_minimum_required(VERSION 3.5)

project(tank_game LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(tank_game main.cpp)

install(TARGETS tank_game
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
set(SFML_DIR "C:/Users/manga/OneDrive/Pictures/SFML-2.5.1/lib/cmake/SFML")
find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

target_link_libraries(tank_game sfml-graphics sfml-audio)
target_include_directories(tank_game PRIVATE "C:/Users/manga/OneDrive/Pictures/SFML-2.5.1/include")
