
cmake_minimum_required(VERSION 3.19.3)

project(SCHESS)

include(cmake/CompilerWarnings.cmake)
set(WARNINGS_AS_ERRORS OFF)


add_compile_definitions(_DEBUG=1)
#add_compile_definitions(IMGUI_IMPL_OPENGL_LOADER_GLEW=1) # setup imgui with glew
add_compile_definitions(GLEW_STATIC=1)                   # use static

# -mwindows turns off the console and others make it shareble or whatever
#SET(CMAKE_CXX_FLAGS "-static-libgcc -static-libstdc++ -mwindows")

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)

# set(LIBRARY_SOURCE_DIR ../dev)
# #set(IMGUI_SRC_DIR ${LIBRARY_SOURCE_DIR}/ImGui)

# set(GLEW_SRC_FILES ${LIBRARY_SOURCE_DIR}/glew/glew.c)
# set(STB_IMAGE_SRC_FILES ${LIBRARY_SOURCE_DIR}/stb/stb_image.cpp)

# set(
#     IMGUI_SRC_FILES
#     ${IMGUI_SRC_DIR}/imgui.cpp
#     ${IMGUI_SRC_DIR}/imgui_demo.cpp
#     ${IMGUI_SRC_DIR}/imgui_draw.cpp
#     ${IMGUI_SRC_DIR}/imgui_impl_glfw.cpp
#     ${IMGUI_SRC_DIR}/imgui_impl_opengl3.cpp
#     ${IMGUI_SRC_DIR}/imgui_widgets.cpp
# )


set(SCHESS_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SCHESS_SRC_DIR     ${CMAKE_CURRENT_SOURCE_DIR}/src    )

set(
    HEADER_FILES 
    ${SCHESS_INCLUDE_DIR}/utility.h 
    ${SCHESS_INCLUDE_DIR}/buffer_object.h 
    ${SCHESS_INCLUDE_DIR}/shader.h 
    ${SCHESS_INCLUDE_DIR}/schess.h 
    ${SCHESS_INCLUDE_DIR}/uniform.h 
    ${SCHESS_INCLUDE_DIR}/vertex_array_object.h
    ${SCHESS_INCLUDE_DIR}/texture.h
    ${SCHESS_INCLUDE_DIR}/renderer.h
    ${SCHESS_INCLUDE_DIR}/game_logic.h
)

set(
    SRC_FILES
    ${SCHESS_SRC_DIR}/buffer_object.cpp
    ${SCHESS_SRC_DIR}/main.cpp 
    ${SCHESS_SRC_DIR}/schess.cpp 
    ${SCHESS_SRC_DIR}/shader.cpp 
    ${SCHESS_SRC_DIR}/utility.cpp 
    ${SCHESS_SRC_DIR}/vertex_array_object.cpp
    ${SCHESS_SRC_DIR}/texture.cpp
    ${SCHESS_SRC_DIR}/renderer.cpp
    ${SCHESS_SRC_DIR}/game_logic.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/stb/stb_image.cpp
)

set(EXECUTABLE_NAME "e")

add_executable(${EXECUTABLE_NAME} ${SRC_FILES} ${HEADER_FILES} ${GLEW_SRC_FILES} ${IMGUI_SRC_FILES})

set_project_warnings(${EXECUTABLE_NAME})

target_link_libraries(
    ${EXECUTABLE_NAME} 
    ${OPENGL_LIBRARIES}
    glfw
    GLEW::GLEW
)

# #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  # 
# # for precompiled
#
#   add_executable(mandel ${SRC_FILES} ${HEADER_FILES})
#
#   #link glew before opengl
#   target_link_libraries(
#       mandel 
#       ${GLEW_STATIC_LIBRARIES}
#       ${OPENGL_LIBRARIES}
#       ${GLFW_LIBRARY}
#   )
# #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  # 

# target_compile_options(${EXECUTABLE_NAME} PRIVATE "-Wall -Wextra")


target_include_directories(
    ${EXECUTABLE_NAME} PUBLIC
    ${SCHESS_INCLUDE_DIR}
    ${OPENGL_INCLUDE_DIR} 
    ${LIBRARY_SOURCE_DIR}
)

set_target_properties(
    ${EXECUTABLE_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)