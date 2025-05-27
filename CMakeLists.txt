cmake_minimum_required(VERSION 3.10)

project (Sere)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(fmt CONFIG REQUIRED) 
find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "LLVM_INCLUDE_DIRS = ${LLVM_INCLUDE_DIRS}")
message(STATUS "LLVM_DEFINITIONS = ${LLVM_DEFINITIONS}")

# Add LLVM includes and definitions
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# Collect all .cpp and .h files in the current directory and /Parser
file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/Parser/*.cpp")
file(GLOB HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/Parser/*.h")

add_executable(Sere ${SOURCES} ${HEADERS})

# Include the /Parser directory for header files
target_include_directories(Sere PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Parser")
llvm_map_components_to_libnames(llvm_libs core support) # minimal libs

target_link_libraries(Sere PRIVATE fmt::fmt)
target_link_libraries(Sere PRIVATE ${llvm_libs})
