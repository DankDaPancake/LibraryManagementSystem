cmake_minimum_required(VERSION 3.10)
project(LibraryManagementSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(IMGUI_DIR "${CMAKE_SOURCE_DIR}/ui")
set(UI_DIR  "${CMAKE_SOURCE_DIR}/MainUI")

file(GLOB_RECURSE PROJECT_SOURCES
    "${SRC_DIR}/*.cpp"
    "${UI_DIR}/*.cpp"
    "${CMAKE_SOURCE_DIR}/main.cpp"
)

set(IMGUI_SOURCES
    "${IMGUI_DIR}/imgui.cpp"
    "${IMGUI_DIR}/imgui_draw.cpp"
    "${IMGUI_DIR}/imgui_tables.cpp"
    "${IMGUI_DIR}/imgui_widgets.cpp"
    "${IMGUI_DIR}/imgui_impl_win32.cpp"
    "${IMGUI_DIR}/imgui_impl_dx9.cpp"
    "${IMGUI_DIR}/imgui_demo.cpp"   # <- thêm nếu có gọi ShowDemoWindow
)

add_executable(LMS
    ${PROJECT_SOURCES}
    ${IMGUI_SOURCES}
)

target_include_directories(LMS PRIVATE
    "${CMAKE_SOURCE_DIR}/include"
    "${CMAKE_SOURCE_DIR}/MainUI"
    "${IMGUI_DIR}"
)

# Link DX9 + Win32 + DWM
target_link_libraries(LMS PRIVATE
    d3d9
    d3dcompiler
    imm32
    dwmapi
)

# set_target_properties(LMS PROPERTIES WIN32_EXECUTABLE YES) # nếu muốn ẩn console
