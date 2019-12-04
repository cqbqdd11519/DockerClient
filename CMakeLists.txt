cmake_minimum_required(VERSION 3.10)

project(DockerClient)

# Add rapidJSON dependency
set(RAPIDJSON_BUILD_TESTS OFF CACHE BOOL "")
set(RAPIDJSON_BUILD_EXAMPLES OFF CACHE BOOL "")
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/rapidjson)
find_package(RapidJSON)

find_package(CURL)

add_library(docker-cpp STATIC "")

set(DOCKER_CLIENT_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE)
message(MESSAGE "${RapidJSON_INCLUDE_DIR}")
target_include_directories(docker-cpp
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
        PUBLIC ${CURL_INCLUDE_DIRS}
        PUBLIC ${RapidJSON_INCLUDE_DIR}
        )

target_link_libraries(docker-cpp PUBLIC ${CURL_LIBRARIES})

target_sources(docker-cpp PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/docker.cpp"
        )
