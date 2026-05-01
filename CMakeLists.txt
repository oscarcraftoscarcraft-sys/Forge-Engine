cmake_minimum_required(VERSION 3.20)

project(ForgeEngine LANGUAGES CXX)

# -----------------------------
# Configuración global
# -----------------------------
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

# Evita recompilar dependencias cada vez
set(FETCHCONTENT_UPDATES_DISCONNECTED ON)

# -----------------------------
# OpenGL
# -----------------------------
find_package(OpenGL REQUIRED)

# -----------------------------
# GLFW
# -----------------------------
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.8
)

# Opciones para GLFW (evita cosas innecesarias)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(glfw)

# -----------------------------
# GLEW (instalación local)
# -----------------------------
find_path(GLEW_INCLUDE_DIR GL/glew.h
    PATHS "C:/Lib/glew-2.3.1/include"
    NO_DEFAULT_PATH
)

find_library(GLEW_LIBRARY
    NAMES glew32 glew32s
    PATHS "C:/Lib/glew-2.3.1/lib"
    PATH_SUFFIXES Release/x64 x64 Release
    NO_DEFAULT_PATH
)

if(NOT GLEW_INCLUDE_DIR OR NOT GLEW_LIBRARY)
    message(FATAL_ERROR "GLEW no encontrado. Verifica la ruta C:/Lib/glew-2.3.1")
endif()

add_library(GLEW::GLEW UNKNOWN IMPORTED)
set_target_properties(GLEW::GLEW PROPERTIES
    IMPORTED_LOCATION "${GLEW_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${GLEW_INCLUDE_DIR}"
)

# -----------------------------
# Ejecutable
# -----------------------------
add_executable(Forge2Engine
    ForgeEngineMain.cpp
    version.rc
)

# -----------------------------
# Includes
# -----------------------------
target_include_directories(Forge2Engine PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# -----------------------------
# Link
# -----------------------------
target_link_libraries(Forge2Engine PRIVATE
    OpenGL::GL
    glfw
    GLEW::GLEW
)

# -----------------------------
# Fixes por plataforma
# -----------------------------
if(WIN32)
    # Necesario para que M_PI esté disponible en MSVC
    target_compile_definitions(Forge2Engine PRIVATE _USE_MATH_DEFINES)

    # Copiar DLL automáticamente (ruta conocida)
    find_file(GLEW_DLL
        NAMES glew32.dll
        PATHS "C:/Lib/glew-2.3.1/bin" "C:/Lib/glew-2.3.1/bin/Release/x64"
        NO_DEFAULT_PATH
    )
    if(GLEW_DLL)
        add_custom_command(TARGET Forge2Engine POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${GLEW_DLL}"
                $<TARGET_FILE_DIR:Forge2Engine>
        )
    endif()


    # Copiar shaders al directorio del ejecutable
    add_custom_command(TARGET Forge2Engine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/SHADERS
            $<TARGET_FILE_DIR:Forge2Engine>/SHADERS
    )


    # Copiar ejecutable como .bin
    add_custom_command(TARGET Forge2Engine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            $<TARGET_FILE:Forge2Engine>
            $<TARGET_FILE_DIR:Forge2Engine>/Forge2Engine.bin
    )
endif()

if(UNIX AND NOT APPLE)
    target_link_libraries(Forge2Engine PRIVATE dl pthread)
endif()

