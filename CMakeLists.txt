cmake_minimum_required(VERSION 3.10)

project (Sere)

find_package(fmt CONFIG REQUIRED) 

# Collect all .cpp and .h files in the current directory and /Parser
file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/Parser/*.cpp")
file(GLOB HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/Parser/*.h")

add_executable(Sere ${SOURCES} ${HEADERS})

# Include the /Parser directory for header files
target_include_directories(Sere PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Parser")

target_link_libraries(Sere PRIVATE fmt::fmt)
