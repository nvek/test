cmake_minimum_required(VERSION 3.5)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(APPNAME testTask)
project(${APPNAME} LANGUAGES C CXX)

#set(CMAKE_VERBOSE_MAKEFILE ON)

if (NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif ()
if (NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
	if (UNIX)
		set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
	else()
		set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
	endif ()
endif()
if (NOT CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
endif ()
mark_as_advanced(
	CMAKE_RUNTIME_OUTPUT_DIRECTORY
	CMAKE_LIBRARY_OUTPUT_DIRECTORY
	CMAKE_ARCHIVE_OUTPUT_DIRECTORY
)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Debug' as none was specified")
  set(CMAKE_BUILD_TYPE Debug CACHE STRING  "Choose the type of build" FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

if(NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 11)
endif()

# compilers flags
if (UNIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unknown-pragmas -Wno-sign-compare")
endif ()


#---------------------Sources---------------------------#

add_executable(${APPNAME}

	src/BMPReader.h
	src/BMPReader.cpp
	src/getCPUTIme.c
	src/immintrin.h
	src/main.cpp
	src/MainManager.h
	src/MainManager.cpp
	src/YUVVideo.h
	src/YUVVideo.cpp
	src/YUVFrame.h
	src/YUVFrame.cpp
	)
if (UNIX)
	target_compile_options(${APPNAME}
		PRIVATE -fvisibility=hidden
		PUBLIC $<$<CONFIG:Debug>:-fstack-protector>
		PUBLIC $<$<CONFIG:Release>:-O3>
	)
endif ()



message(STATUS "CMake ${CMAKE_VERSION} successfully configured ${PROJECT_NAME} using ${CMAKE_GENERATOR} generator")
message(STATUS "CMAKE_CXX_LIBRARY_ARCHITECTURE - ${CMAKE_CXX_LIBRARY_ARCHITECTURE}")
message(STATUS "Installation target path: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "")
message(STATUS "CXX_FLAGS:=${CMAKE_CXX_FLAGS}")
message(STATUS "CMAKE_CXX_FLAGS_DEBUG:=${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "CMAKE_CXX_FLAGS_RELEASE:=${CMAKE_CXX_FLAGS_RELEASE}")
message(STATUS "CMAKE_CXX_FLAGS_MINSIZEREL:=${CMAKE_CXX_FLAGS_MINSIZEREL}")
message(STATUS "CMAKE_CXX_FLAGS_RELWITHDEBINFO:=${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
