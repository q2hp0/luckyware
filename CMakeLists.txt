cmake_minimum_required(VERSION 3.16)
project(anti-luckyware VERSION 0.9 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/W4 /wd4100 /wd4189)
    add_definitions(-DWIN32_LEAN_AND_MEAN -D_CRT_SECURE_NO_WARNINGS)
else()
    add_compile_options(-Wall -Wextra -Wno-unused-parameter)
endif()

include_directories(
    include
    common
    .
    include/engine
    include/canary
    include/fsmon
    include/procguard
    include/handlemon
    include/debugmon
    include/vssmon
    include/allowlist
    include/responder
    include/selfprotect
    include/etw
    include/service
)

file(GLOB_RECURSE SRC src/*.cpp)
file(GLOB_RECURSE COMMON common/*.cpp)

add_executable(anti-luckyware
    ${SRC}
    ${COMMON}
)

if(WIN32)
    target_link_libraries(anti-luckyware
        advapi32
        shell32
        ole32
        oleaut32
        wbemuuid
        tdh
        wevtapi
        crypt32
        wintrust
        psapi
        ntdll
        userenv
        version
        shlwapi
        fltlib
        gdi32
        user32
    )
endif()
