cmake_minimum_required(VERSION 3.16)
project(Forge2Engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE Debug)
#adadtar al compilador que uses de c/c++
set(MSYS2_PATH "C:/msys64/mingw64")
set(GLFW_PATH "${MSYS2_PATH}")
set(GLEW_PATH "${MSYS2_PATH}")
set(OPENGL_PATH "${MSYS2_PATH}")

include_directories(${GLFW_PATH}/include)
include_directories(${GLEW_PATH}/include)
include_directories(${OPENGL_PATH}/include)

set(SOURCES
    ForgeEngineMain.cpp
    script.cpp
)

set(HEADERS
    Anvil.h
    script.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

find_package(OpenGL REQUIRED)

target_link_libraries(${PROJECT_NAME} 
    ${OPENGL_LIBRARIES}
    ${GLFW_PATH}/lib/libglfw3.a
    ${GLEW_PATH}/lib/libglew32.a
    opengl32
    glu32
    gdi32
    user32
    kernel32
)

if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE 
        _WIN32_WINNT=0x0601
        WIN32_LEAN_AND_MEAN
        NOMINMAX
    )
    #adadtar a tu conveniencia
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${GLFW_PATH}/bin/glfw3.dll
        ${GLEW_PATH}/bin/glew32.dll
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
endif()

target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Debug>:-g -O0 -Wall -Wextra>
    $<$<CONFIG:Release>:-O3 -DNDEBUG>
)

if(MINGW)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
    target_link_libraries(${PROJECT_NAME} -static-libgcc -static-libstdc++)
    target_compile_definitions(${PROJECT_NAME} PRIVATE GLEW_STATIC)
endif()

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()
