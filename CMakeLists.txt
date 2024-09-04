cmake_minimum_required(VERSION 3.5)

project(CustomEventSystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find OpenCV
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# Find GLFW
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS})
link_directories(${GLFW_LIBRARY_DIRS})

# Include directories for Dear ImGui
include_directories(third_party/imgui)

# Source files for Dear ImGui
file(GLOB IMGUI_SOURCES
    third_party/imgui/imgui.cpp
    third_party/imgui/imgui_draw.cpp
    third_party/imgui/imgui_tables.cpp
    third_party/imgui/imgui_widgets.cpp
    third_party/imgui/backends/imgui_impl_glfw.cpp
    third_party/imgui/backends/imgui_impl_opengl3.cpp
)
# Include directories for Dear ImGui and other libraries
include_directories(
    third_party/imgui
    third_party/imgui/backends
    ${CMAKE_SOURCE_DIR}/src/gui
    ${CMAKE_SOURCE_DIR}/src/commandline
    ${CMAKE_SOURCE_DIR}/src/common
    ${CMAKE_SOURCE_DIR}/src/defog
    ${CMAKE_SOURCE_DIR}/src/detection
    ${CMAKE_SOURCE_DIR}/src/video
    # Add other include directories here
)


add_executable(CustomEventSystem
    main.cpp
    ${IMGUI_SOURCES}
    src/gui/gui_renderer.cpp src/gui/gui_renderer.h
    src/commandline/commandline_args.cpp src/commandline/commandline_args.h
    src/common/event_dispatcher.cpp src/common/event_dispatcher.h
    src/common/iprocessor.cpp src/common/iprocessor.h
    src/defog/defogger.cpp src/defog/defogger.h
    src/detection/inference_engine.cpp src/detection/inference_engine.h
    src/video/video_processor.cpp src/video/video_processor.h )

include(GNUInstallDirs)
install(TARGETS CustomEventSystem
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

target_link_libraries(
    ${PROJECT_NAME}
    ${OpenCV_LIBS}
    ${GLFW_LIBRARIES}
    GL
    dl
    pthread
)

