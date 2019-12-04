cmake_minimum_required(VERSION 3.10)

project(DockerClient)

find_package(CURL)

# Add rapidJSON dependency
set(RAPIDJSON_BUILD_TESTS OFF)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/rapidjson)


add_library(docker-cpp STATIC "")

target_include_directories(docker-cpp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(docker-cpp PUBLIC ${CURL_INCLUDE_DIRS})
target_include_directories(docker-cpp PUBLIC ${RapidJSON_INCLUDE_DIR})

target_link_libraries(docker-cpp PUBLIC ${CURL_LIBRARIES})

target_sources(docker-cpp PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/docker.cpp"
        )
