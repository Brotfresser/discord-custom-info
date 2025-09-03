cmake_minimum_required(VERSION 4.0)
set(PROJECT_NAME "discord_parser")
project(${PROJECT_NAME} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(DISCORD_INCLUDE_DIR "${INCLUDE_DIR}/discord")
set(DISCORD_SDK_SO ${CMAKE_SOURCE_DIR}/lib/x86_64/discord_game_sdk.so)
file(GLOB SOURCES "${DISCORD_INCLUDE_DIR}/*.cpp")

add_library(discord_sdk SHARED IMPORTED)
set_target_properties(discord_sdk PROPERTIES
    IMPORTED_LOCATION "${DISCORD_SDK_SO}"
    INTERFACE_INCLUDE_DIRECTORIES "${DISCORD_INCLUDE_DIR}"
)
find_package(spdlog REQUIRED)

add_executable(${PROJECT_NAME} main.cpp ${SOURCES})


#target_include_directories(${PROJECT_NAME} PRIVATE ${DISCORD_INCLUDE_DIR})
# не работает с модулями
include_directories(${DISCORD_INCLUDE_DIR})
include_directories("${INCLUDE_DIR}/json")
#target_include_directories(${PROJECT_NAME} PRIVATE "${INCLUDE_DIR}/json")
# не работает с модулями

target_link_libraries(${PROJECT_NAME} PRIVATE 
    discord_sdk
    pthread
    spdlog::spdlog
)



file(GLOB ALL_SOURCES "*.cppm")
#list(FILTER ALL_SOURCES EXCLUDE REGEX "main.cpp")
add_library(AllModules)
target_sources(AllModules
    PUBLIC
        FILE_SET modules TYPE CXX_MODULES
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
        FILES ${ALL_SOURCES}
)
target_link_libraries(${PROJECT_NAME} PRIVATE AllModules)
#target_precompile_headers(test PRIVATE <print>)