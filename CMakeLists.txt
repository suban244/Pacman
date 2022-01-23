cmake_minimum_required (VERSION 3.5)
set(PROJECT_NAME Pacman)
project(Pacman)

#add_subdirectory(SDL2)

set (CMAKE_BUID_TYPE Debug)

set (source_dir "${PROJECT_SOURCE_DIR}/src/")
file (GLOB source_files "${source_dir}/*.cpp")

add_executable(${PROJECT_NAME} ${source_files})

#target_link_libraries(${PROJECT_NAME} PUBLIC SDL SDL_image SDL_mixer SDL_ttf)
# target_include_directories(${PROJECT_NAME} PUBLIC "${source_dir}/SDL/include" "${source_dir}/SDL_image/include" "${source_dir}/SDL_mixer/include" "${source_dir}/SDL_ttf/include")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -g -std=c++14 -lSDL2 -lGL -lSDL2_image -lSDL2_mixer -lSDL2_ttf")



