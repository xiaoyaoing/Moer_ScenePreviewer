set(IMGUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ext/imgui")
set(IMGUI_FILES
        "${IMGUI_DIR}/imgui.cpp"
        "${IMGUI_DIR}/imgui_demo.cpp"
        "${IMGUI_DIR}/imgui_draw.cpp"
        "${IMGUI_DIR}/imgui_widgets.cpp"
        "${IMGUI_DIR}/imgui_tables.cpp"
        "${IMGUI_DIR}/imconfig.h"
        "${IMGUI_DIR}/imgui.h"
        "${IMGUI_DIR}/imgui_internal.h"
        "${IMGUI_DIR}/imstb_rectpack.h"
        "${IMGUI_DIR}/imstb_textedit.h"
        "${IMGUI_DIR}/imstb_truetype.h"
        "${IMGUI_DIR}/backends/imgui_impl_glfw.h"
        "${IMGUI_DIR}/backends/imgui_impl_glfw.cpp"
        "${IMGUI_DIR}/backends/imgui_impl_opengl3.h"
        "${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp"
)



add_library(imgui STATIC ${IMGUI_FILES})

target_link_libraries(imgui PUBLIC glfw)
target_include_directories(imgui PUBLIC ${IMGUI_DIR})

set_target_properties(imgui PROPERTIES FOLDER "EXT" POSITION_INDEPENDENT_CODE ON)

set(GLAD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ext/glad")
set(GLAD_FILES
        "${GLAD_DIR}/glad.c"
        "${GLAD_DIR}/glad.h"
)
add_library(glad STATIC ${GLAD_FILES})
target_include_directories(glad PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/ext")
set_target_properties(glad PROPERTIES FOLDER "EXT" POSITION_INDEPENDENT_CODE ON)



