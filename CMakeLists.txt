# Yêu cầu phiên bản CMake tối thiểu
cmake_minimum_required(VERSION 3.16)

# Đặt tên cho dự án và ngôn ngữ sử dụng
project(AutoClicker VERSION 1.0 LANGUAGES CXX)

# Thiết lập tiêu chuẩn C++ là C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- DÒNG NÀY RẤT QUAN TRỌNG CHO VIỆC DEBUG LỖI BUILD ---
# Bật chế độ output chi tiết để xem các lệnh biên dịch đầy đủ
set(CMAKE_VERBOSE_MAKEFILE ON)

# Thêm thư mục con SDL3 vào quá trình build
add_subdirectory(extern/sdl)

# Liệt kê các file mã nguồn của Dear ImGui
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

# Tạo một thư viện tĩnh tên là "imgui"
add_library(imgui STATIC ${IMGUI_SOURCES})

# --- PHẦN SỬA LỖI QUAN TRỌNG NHẤT ---
#
# Explicitly add the include directories to the "imgui" target.
# This includes its own headers, its backend headers, AND the SDL3 headers.
# This is the most direct way to solve the "file not found" error.
target_include_directories(imgui PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui"
    "${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui/backends"
    "${CMAKE_CURRENT_SOURCE_DIR}/extern/sdl/include"  # <-- ĐÂY LÀ DÒNG THÊM VÀO QUAN TRỌNG NHẤT
)
#
# --- KẾT THÚC PHẦN SỬA LỖI ---


# Tạo file thực thi cho ứng dụng
add_executable(AutoClicker src/main.cpp)

# Liên kết ứng dụng chính với các thư viện
# Vì imgui đã được cấu hình với tất cả include paths cần thiết (dưới dạng PUBLIC),
# AutoClicker sẽ tự động kế thừa chúng.
target_link_libraries(AutoClicker PRIVATE
    imgui # Thư viện ImGui của chúng ta
    SDL3::SDL3 # Thư viện SDL3 đã được build
    "-framework OpenGL"
    "-framework CoreFoundation"
)