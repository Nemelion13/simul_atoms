cmake_minimum_required(VERSION 3.20)

project(simul_atoms LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(raylib CONFIG REQUIRED)
find_path(RAYGUI_INCLUDE_DIR raygui.h HINTS "$ENV{MINGW_PREFIX}/include" REQUIRED)

add_executable(simul_atoms main.cpp atoms.cpp)
target_include_directories(simul_atoms PRIVATE "${RAYGUI_INCLUDE_DIR}")
target_link_libraries(simul_atoms PRIVATE raylib)

if(WIN32 AND MINGW)
    set(MINGW_RUNTIME_DLLS
        "$ENV{MINGW_PREFIX}/bin/glfw3.dll"
        "$ENV{MINGW_PREFIX}/bin/libgcc_s_seh-1.dll"
        "$ENV{MINGW_PREFIX}/bin/libraylib.dll"
        "$ENV{MINGW_PREFIX}/bin/libstdc++-6.dll"
        "$ENV{MINGW_PREFIX}/bin/libwinpthread-1.dll"
    )
    foreach(RUNTIME_DLL IN LISTS MINGW_RUNTIME_DLLS)
        if(NOT EXISTS "${RUNTIME_DLL}")
            message(FATAL_ERROR "Runtime DLL not found: ${RUNTIME_DLL}")
        endif()
        add_custom_command(TARGET simul_atoms POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${RUNTIME_DLL}"
                "$<TARGET_FILE_DIR:simul_atoms>"
            VERBATIM
        )
    endforeach()
endif()

file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Tools")
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/Tools/logo.png"
    "${CMAKE_CURRENT_BINARY_DIR}/Tools/logo.png"
    COPYONLY
)

file(GLOB AUDIO_FILES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/Tools/*.mp3"
)
foreach(AUDIO_FILE IN LISTS AUDIO_FILES)
    get_filename_component(AUDIO_FILE_NAME "${AUDIO_FILE}" NAME)
    configure_file(
        "${AUDIO_FILE}"
        "${CMAKE_CURRENT_BINARY_DIR}/Tools/${AUDIO_FILE_NAME}"
        COPYONLY
    )
endforeach()
