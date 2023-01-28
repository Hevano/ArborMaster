## add libraries
add_library(glad ${CMAKE_CURRENT_SOURCE_DIR}/glad/include/glad/glad.h ${CMAKE_CURRENT_SOURCE_DIR}/glad/src/glad.c)

file(GLOB IMGUI_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/imgui/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/imgui/*.h)
add_library(imgui STATIC ${IMGUI_SOURCES}  ${CMAKE_CURRENT_SOURCE_DIR}/imgui/misc/cpp/imgui_stdlib.cpp  ${CMAKE_CURRENT_SOURCE_DIR}/imgui/misc/cpp/imgui_stdlib.cpp)

file(GLOB IMNODE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/imnodes/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/imnodes/*.h)
add_library(imnodes STATIC ${IMNODE_SOURCES})


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

target_include_directories(imnodes PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/imnodes) 
target_link_libraries(imnodes PUBLIC imgui)