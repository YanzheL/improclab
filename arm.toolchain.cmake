if (COMMAND TOOLCHAIN_FINISH)
    return()
endif ()
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_VERSION 1)
set(GNU_MACHINE arm-linux-musleabi)
if (NOT DEFINED CMAKE_SYSTEM_PROCESSOR)
    set(CMAKE_SYSTEM_PROCESSOR arm)
endif ()
message("CMAKE_SYSTEM_PROCESSOR=${CMAKE_SYSTEM_PROCESSOR}")

if (NOT CMAKE_FIND_ROOT_PATH_MODE_LIBRARY)
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
endif ()

if (NOT CMAKE_FIND_ROOT_PATH_MODE_INCLUDE)
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
endif ()

if (NOT CMAKE_FIND_ROOT_PATH_MODE_PACKAGE)
    set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
endif ()

if (NOT CMAKE_FIND_ROOT_PATH_MODE_PROGRAM)
    set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
endif ()

if (CMAKE_SYSTEM_PROCESSOR STREQUAL arm AND NOT ARM_IGNORE_FP)
    set(FLOAT_ABI_SUFFIX "")
    if (NOT SOFTFP)
        set(FLOAT_ABI_SUFFIX "hf")
    endif ()
endif ()

set(FULL_ARCH ${GNU_MACHINE}${FLOAT_ABI_SUFFIX})
message("FULL_ARCH=${FULL_ARCH}")

if (NOT DEFINED CMAKE_C_COMPILER)
    find_program(CMAKE_C_COMPILER NAMES ${FULL_ARCH}-gcc)
else ()
    message(WARNING "CMAKE_C_COMPILER=${CMAKE_C_COMPILER} is defined")
endif ()
message("CMAKE_C_COMPILER=${CMAKE_C_COMPILER}")

if (NOT DEFINED CMAKE_CXX_COMPILER)
    find_program(CMAKE_CXX_COMPILER NAMES ${FULL_ARCH}-g++)
else ()
    message(WARNING "CMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER} is defined")
endif ()
message("CMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}")

if (NOT DEFINED CMAKE_LINKER)
    find_program(CMAKE_LINKER NAMES ${FULL_ARCH}-ld ${FULL_ARCH}-ld)
else ()
    message(WARNING "CMAKE_LINKER=${CMAKE_LINKER} is defined")
endif ()
message("CMAKE_LINKER=${CMAKE_LINKER}")

if (NOT DEFINED CMAKE_AR)
    find_program(CMAKE_AR NAMES ${FULL_ARCH}-ar ${FULL_ARCH}-ar)
else ()
    message(WARNING "CMAKE_AR=${CMAKE_AR} is defined")
endif ()
message("CMAKE_AR=${CMAKE_AR}")

if (NOT DEFINED CMAKE_CXX_FLAGS)
    set(CMAKE_CXX_FLAGS "" CACHE INTERNAL "")
    set(CMAKE_C_FLAGS "" CACHE INTERNAL "")
    set(CMAKE_SHARED_LINKER_FLAGS "" CACHE INTERNAL "")
    set(CMAKE_MODULE_LINKER_FLAGS "" CACHE INTERNAL "")
    set(CMAKE_EXE_LINKER_FLAGS "" CACHE INTERNAL "")

    #    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdata-sections -Wa,--noexecstack -fsigned-char -Wno-psabi")
    #    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fdata-sections -Wa,--noexecstack -fsigned-char -Wno-psabi")
    set(CMAKE_EXE_LINKER_FLAGS "-static")

    if (CMAKE_SYSTEM_PROCESSOR STREQUAL arm)
        set(CMAKE_CXX_FLAGS "-mthumb ${CMAKE_CXX_FLAGS}")
        set(CMAKE_C_FLAGS "-mthumb ${CMAKE_C_FLAGS}")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-z,nocopyreloc")
        #        set(ARM_LINKER_FLAGS "-Wl,--fix-cortex-a8 -Wl,--no-undefined -Wl,--gc-sections -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now")
        #    elseif (CMAKE_SYSTEM_PROCESSOR STREQUAL aarch64)
        #        set(ARM_LINKER_FLAGS "-Wl,--no-undefined -Wl,--gc-sections -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now")
    endif ()
    set(CMAKE_SHARED_LINKER_FLAGS "${ARM_LINKER_FLAGS} ${CMAKE_SHARED_LINKER_FLAGS}")
    set(CMAKE_MODULE_LINKER_FLAGS "${ARM_LINKER_FLAGS} ${CMAKE_MODULE_LINKER_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${ARM_LINKER_FLAGS} ${CMAKE_EXE_LINKER_FLAGS}")
else ()
    message(WARNING "CMAKE_CXX_FLAGS='${CMAKE_CXX_FLAGS}' is defined")
endif ()

message("CMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}")
message("CMAKE_C_FLAGS=${CMAKE_C_FLAGS}")
message("CMAKE_SHARED_LINKER_FLAGS=${CMAKE_SHARED_LINKER_FLAGS}")
message("CMAKE_MODULE_LINKER_FLAGS=${CMAKE_MODULE_LINKER_FLAGS}")
message("CMAKE_EXE_LINKER_FLAGS=${CMAKE_EXE_LINKER_FLAGS}")
macro(TOOLCHAIN_FINISH)
    message("Toolchain Config Finished")
endmacro()
TOOLCHAIN_FINISH()