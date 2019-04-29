cmake_minimum_required (VERSION 2.8.7)
project (ws_tcp_server)

###
# variables
###
set(SOURCES_DIRECTORY ${PROJECT_SOURCE_DIR}/sources)
set(INCLUDES_DIRECTORY ${PROJECT_SOURCE_DIR}/includes)

set(MUDUO_LOGGER_INCLUDES ${INCLUDES_DIRECTORY}/muduo_logger)
set(MUDUO_NETWORK_INCLUDES ${INCLUDES_DIRECTORY}/muduo_network)

###
# includes
###
include_directories(${INCLUDES_DIRECTORY}
         ${MUDUO_LOGGER_INCLUDES}
         ${MUDUO_NETWORK_INCLUDES})

###
# libraries
###
link_directories("${PROJECT_SOURCE_DIR}/library")

###
# outputs
###
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
#set(CMAKE_PKGCONFIG_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/pkgconfig)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/library)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

###
# compilation options
###
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -W -Wall -Wextra -O3")

# print compile info
#set(CMAKE_VERBOSE_MAKEFILE ON)

###
# sources file
###
set(MUDUO_NETWORK_SOURCES_DIRECTORY ${SOURCES_DIRECTORY}/muduo_network)
set(MUDUO_LOGGER_SOURCES_DIRECTORY ${SOURCES_DIRECTORY}/muduo_logger)

foreach(dir ${MUDUO_LOGGER_SOURCES_DIRECTORY})
  # get directory sources and headers
  file(GLOB s_${dir} "${dir}/*.cpp")
  file(GLOB h_${dir} "${dir}/*.hpp")

  # set muiduo_logger sources
  set(muduo_logger_sources ${s_${dir}} ${h_${dir}})
endforeach()

foreach(dir ${MUDUO_NETWORK_SOURCES_DIRECTORY})
  # get directory sources and headers
  file(GLOB s_${dir} "${dir}/*.cpp")
  file(GLOB h_${dir} "${dir}/*.hpp")

  # set muiduo_network sources
  set(muduo_network_sources ${s_${dir}} ${h_${dir}})
endforeach()

#message("muduo_network_sources: ${muduo_network_sources}")

set(example_server_sources
    ${SOURCES_DIRECTORY}/example/example_server.cpp
)

###
#library
###
add_library(async_logging ${muduo_logger_sources})

add_library(muduo_server ${muduo_network_sources})

###
# executable
###
add_executable(example_server ${example_server_sources})

###
# link librarys
###
target_link_libraries(async_logging pthread)

target_link_libraries(muduo_server async_logging)
target_link_libraries(muduo_server pthread)


target_link_libraries(example_server async_logging
								     muduo_server
                                     pthread)

