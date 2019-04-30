cmake_minimum_required (VERSION 2.8.7)
project (http_server)

###
# variables
###
set(PROJECT_ROOT_DIR ${PROJECT_SOURCE_DIR}/../../)
set(SOURCES_DIRECTORY ../../example)
set(INCLUDES_DIRECTORY ../../includes)

set(MUDUO_LOGGER_INCLUDES ${INCLUDES_DIRECTORY}/muduo_logger)
set(MUDUO_NETWORK_INCLUDES ${INCLUDES_DIRECTORY}/muduo_network)

###
# includes
###
include_directories(.
         ./http
         ${INCLUDES_DIRECTORY}
         ${MUDUO_LOGGER_INCLUDES}
         ${MUDUO_NETWORK_INCLUDES})

###
# libraries
###
link_directories("${PROJECT_ROOT_DIR}/library")

###
# outputs
###
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
#set(CMAKE_PKGCONFIG_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/pkgconfig)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(LIBRARY_OUTPUT_PATH ${PROJECT_ROOT_DIR}/library)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_ROOT_DIR}/bin)

###
# compilation options
###
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -W -Wall -Wextra -O3")

# print compile info
#set(CMAKE_VERBOSE_MAKEFILE ON)

###
# sources file
###
set(http_server_sources
    ${SOURCES_DIRECTORY}/http_server/http/HttpContext.cpp
    ${SOURCES_DIRECTORY}/http_server/http/HttpResponse.cpp
    ${SOURCES_DIRECTORY}/http_server/main.cpp
)

###
#library
###

###
# executable
###
add_executable(http_server ${http_server_sources})

###
# link librarys
###
target_link_libraries(http_server    async_logging
								     muduo_server
                                     pthread)