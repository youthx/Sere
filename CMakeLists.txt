cmake_minimum_required(VERSION 3.16)
project(Sere VERSION 0.1 LANGUAGES CXX)

# ─────────────────────────────────────────────────────────────
# 🧰 Compiler & Tooling Config
# ─────────────────────────────────────────────────────────────
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# clang-tidy (optional)
find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
if(CLANG_TIDY_EXE)
    set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
endif()

# ─────────────────────────────────────────────────────────────
# 📁 Source Files
# ─────────────────────────────────────────────────────────────
file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE PROJECT_HEADERS CONFIGURE_DEPENDS include/*.hpp)

add_executable(sere ${PROJECT_SOURCES} ${PROJECT_HEADERS})

target_include_directories(sere PRIVATE include)

# ─────────────────────────────────────────────────────────────
# 🧠 LLVM Integration
# ─────────────────────────────────────────────────────────────
find_package(zstd REQUIRED)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "LLVM_INCLUDE_DIRS = ${LLVM_INCLUDE_DIRS}")
message(STATUS "LLVM_DEFINITIONS = ${LLVM_DEFINITIONS}")

set(LLVM_COMPONENTS
    native
    core
    support
)

llvm_map_components_to_libnames(LLVM_LIBS ${LLVM_COMPONENTS})

target_include_directories(sere PRIVATE ${LLVM_INCLUDE_DIRS})
target_compile_definitions(sere PRIVATE ${LLVM_DEFINITIONS})

# Link LLVM libs and zstd properly, once only
target_link_libraries(sere PRIVATE ${LLVM_LIBS} zstd::libzstd_shared)

# ─────────────────────────────────────────────────────────────
# 🧼 IDE Grouping (Optional)
# ─────────────────────────────────────────────────────────────
source_group(TREE ${CMAKE_SOURCE_DIR}/src PREFIX "Source Files" FILES ${PROJECT_SOURCES})
source_group(TREE ${CMAKE_SOURCE_DIR}/include PREFIX "Header Files" FILES ${PROJECT_HEADERS})
