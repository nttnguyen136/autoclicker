cmake_minimum_required(VERSION 3.16)


project(AutoClicker VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_VERBOSE_MAKEFILE ON)

add_subdirectory(extern/sdl)

set(IMGUI_SOURCES
    extern/imgui/imconfig.h
    extern/imgui/imgui.h
    extern/imgui/imgui.cpp
    extern/imgui/imgui_draw.cpp
    extern/imgui/imgui_tables.cpp
    extern/imgui/imgui_widgets.cpp
    extern/imgui/imgui_demo.cpp
    extern/imgui/backends/imgui_impl_sdl3.h
    extern/imgui/backends/imgui_impl_sdl3.cpp
    extern/imgui/backends/imgui_impl_opengl3.h
    extern/imgui/backends/imgui_impl_opengl3.cpp
)

add_library(imgui STATIC ${IMGUI_SOURCES})

# Explicitly add the include directories to the "imgui" target.
# This includes its own headers, its backend headers, AND the SDL3 headers.
# This is the most direct way to solve the "file not found" error.
target_include_directories(imgui PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui"
    "${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui/backends"
    "${CMAKE_CURRENT_SOURCE_DIR}/extern/sdl/include"
)



add_executable(AutoClicker 
    src/main.cpp
    src/core/Application.cpp
    src/ui/UI.cpp
)

target_include_directories(AutoClicker PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")


# --- PLATFORM-SPECIFIC BUILD LOGIC ---
if(APPLE)
    # Add macOS-specific source files to the executable
    target_sources(AutoClicker PRIVATE
        src/platform/macos/MacOSPlatform.cpp
    )

    # Link against required macOS frameworks
    target_link_libraries(AutoClicker PRIVATE
        "-framework ApplicationServices"
    )
endif()

target_link_libraries(AutoClicker PRIVATE
    imgui
    SDL3::SDL3
    "-framework OpenGL"
    "-framework CoreFoundation"
)