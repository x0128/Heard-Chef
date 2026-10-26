cmake_minimum_required(VERSION 3.28)

project(game_template LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(GAME_HOT_RELOAD "Build the game as a DLL loaded by a hot reload host" OFF)

if(GAME_HOT_RELOAD)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
else()
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

include(FetchContent)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS ${GAME_HOT_RELOAD} CACHE BOOL "" FORCE)
FetchContent_Declare(
    raylib
    URL https://github.com/raysan5/raylib/archive/refs/tags/6.2.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
)
FetchContent_MakeAvailable(raylib)

file(GLOB CORE_SOURCES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/core/*.cpp)
list(FILTER CORE_SOURCES EXCLUDE REGEX ".*/main_[a-z_]+\\.cpp$")
file(GLOB GAME_SOURCES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/game/*.cpp)

add_custom_target(
    copy_assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different ${CMAKE_SOURCE_DIR}/assets ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets
)

function(game_target_options Target)
    target_include_directories(${Target} PRIVATE ${CMAKE_SOURCE_DIR}/include)
    target_link_libraries(${Target} PRIVATE raylib)
    if(MSVC)
        target_compile_options(${Target} PRIVATE /W4 /permissive- /utf-8)
    endif()
    add_dependencies(${Target} copy_assets)
endfunction()

if(GAME_HOT_RELOAD)
    add_library(game SHARED ${CORE_SOURCES} ${GAME_SOURCES})
    target_compile_definitions(game PRIVATE GAME_DLL USE_LIBTYPE_SHARED)
    game_target_options(game)

    add_executable(game_hot_reload src/core/main_hot_reload.cpp)
    target_include_directories(game_hot_reload PRIVATE ${CMAKE_SOURCE_DIR}/include)
    if(MSVC)
        target_compile_options(game_hot_reload PRIVATE /W4 /permissive- /utf-8)
    endif()
    add_dependencies(game_hot_reload game)
else()
    add_executable(game src/core/main_release.cpp ${CORE_SOURCES} ${GAME_SOURCES})
    game_target_options(game)
    if(MSVC)
        set_target_properties(game PROPERTIES WIN32_EXECUTABLE $<CONFIG:Release>)
        target_link_options(game PRIVATE $<$<CONFIG:Release>:/ENTRY:mainCRTStartup>)
    endif()
endif()
