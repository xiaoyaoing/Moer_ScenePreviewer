cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 17)
project(Moer_ScenePreviewer VERSION 1.1 LANGUAGES CXX)

if (MSVC)
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
endif (MSVC)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

configure_file(src/include/CMakeConfig.h.in src/include/CMakeConfig.h)
set(TARGET_NAME Moer_ScenePreviewer)
set(CMAKE_CXX_USE_RESPONSE_FILE_FOR_INCLUDES 0)
file(GLOB_RECURSE ALL_INCLUDE CONFIGURE_DEPENDS "src/*.h")
file(GLOB_RECURSE ALL_SOURCES CONFIGURE_DEPENDS "src/*.cpp")

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${ALL_INCLUDE} ${ALL_SOURCES})
add_executable(${TARGET_NAME} ${ALL_INCLUDE} ${ALL_SOURCES})
target_include_directories(${TARGET_NAME} PRIVATE ${PROJECT_BINARY_DIR}/src/include)

include(FetchContent)

FetchContent_Declare(
   ImGuiFileDialog
   GIT_REPOSITORY https://github.com/aiekick/ImGuiFileDialog.git
   GIT_TAG v0.6.6.1
)

FetchContent_MakeAvailable(ImGuiFileDialog)
set_target_properties(ImGuiFileDialog PROPERTIES FOLDER ext)


# cmake_policy(SET CMP0135 NEW)
add_subdirectory(ext/glfw)
target_link_libraries(${TARGET_NAME} PRIVATE glfw)

#add_subdirectory(ext/glad)
#target_link_libraries(${TARGET_NAME} PRIVATE glad::glad)

include(${CMAKE_CURRENT_SOURCE_DIR}/ext/imgui.cmake)
target_link_libraries(${TARGET_NAME} PRIVATE imgui)
set(IMGUI_INCLUDE_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/ext/imgui)

target_link_libraries(${TARGET_NAME} PRIVATE glad)

message(STATUS "IMGUI_INCLUDE_DIR: ${IMGUI_INCLUDE_DIR}")
target_include_directories(ImGuiFileDialog PRIVATE ${IMGUI_INCLUDE_DIR})

#find_package(nlohmann_json REQUIRED)
#target_link_libraries(${TARGET_NAME} PRIVATE nlohmann_json::nlohmann_json)

#find_package(Eigen3 REQUIRED)
#target_link_libraries(${TARGET_NAME} PRIVATE Eigen3::Eigen)

#find_package(tinyobjloader REQUIRED)
#target_link_libraries(${TARGET_NAME} PRIVATE tinyobjloader::tinyobjloader)

target_link_libraries(Moer_ScenePreviewer PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(Moer_ScenePreviewer PRIVATE Eigen3::Eigen)
target_link_libraries(Moer_ScenePreviewer PRIVATE tinyobjloader)

target_include_directories(${TARGET_NAME} PRIVATE ${ImGuiFileDialog_SOURCE_DIR})
target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib)
target_link_libraries(
   ${TARGET_NAME} 
   PRIVATE 
   ImGuiFileDialog
)

target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/include)
