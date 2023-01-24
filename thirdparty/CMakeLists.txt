## add libraries
add_library(glad ${CMAKE_CURRENT_SOURCE_DIR}/glad/include/glad/glad.h ${CMAKE_CURRENT_SOURCE_DIR}/glad/src/glad.c)

file(GLOB IMGUI_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/imgui/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/imgui/*.h)
add_library(imgui STATIC ${IMGUI_SOURCES}  ${CMAKE_CURRENT_SOURCE_DIR}/imgui/misc/cpp/imgui_stdlib.cpp  ${CMAKE_CURRENT_SOURCE_DIR}/imgui/misc/cpp/imgui_stdlib.cpp)

# state that anybody linking to us needs to include the current source dir
# to find third party libs, while we don't.

target_include_directories(glad
                   PUBLIC
                    ${CMAKE_CURRENT_SOURCE_DIR}/glad/include
                   )

target_include_directories(imgui
                           PUBLIC
                            ${CMAKE_CURRENT_SOURCE_DIR}/imgui
                           )