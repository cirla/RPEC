cmake_minimum_required (VERSION 3.0)

project (rpec CXX)
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${rpec_SOURCE_DIR}/cmake")

set (rpec_VERSION_MAJOR 0)
set (rpec_VERSION_MINOR 1)
set (rpec_VERSION_PATCH 0)
set (rpec_VERSION_PRERELEASE "dev")

find_package (Git)
if (GIT_FOUND)
    execute_process (
        COMMAND ${GIT_EXECUTABLE} rev-parse --verify --abbrev-ref HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE rpec_GIT_BRANCH
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    execute_process (
        COMMAND ${GIT_EXECUTABLE} rev-parse --verify --short HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE rpec_GIT_REVISION
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    set (rpec_VERSION_METADATA "${rpec_GIT_BRANCH}.${rpec_GIT_REVISION}")
endif (GIT_FOUND)

set (rpec_VERSION "${rpec_VERSION_MAJOR}.${rpec_VERSION_MINOR}.${rpec_VERSION_PATCH}")

if (rpec_VERSION_PRERELEASE)
    set (rpec_VERSION "${rpec_VERSION}-${rpec_VERSION_PRERELEASE}")
    if (rpec_VERSION_PRERELEASE STREQUAL "dev")
        set (rpec_VERSION "${rpec_VERSION}+${rpec_VERSION_METADATA}")
    endif ()
endif (rpec_VERSION_PRERELEASE)

set (rpec_PACKAGE_NAME "rpec")
set (rpec_DESCRIPTION_SUMMARY "RPEC game example")

set (CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

include (CheckCXXCompilerFlag)

CHECK_CXX_COMPILER_FLAG ("-std=c++14" COMPILER_SUPPORTS_CXX14)
CHECK_CXX_COMPILER_FLAG ("-std=c++1y" COMPILER_SUPPORTS_CXX1Y)
CHECK_CXX_COMPILER_FLAG ("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG ("-std=c++0x" COMPILER_SUPPORTS_CXX0X)

if (COMPILER_SUPPORTS_CXX14)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
elseif (COMPILER_SUPPORTS_CXX1Y)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")
elseif (COMPILER_SUPPORTS_CXX11)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif (COMPILER_SUPPORTS_CXX0X)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else ()
    message (STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif ()

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR
        ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang" OR
        ${CMAKE_CXX_COMPILER_ID} STREQUAL "AppleClang" )
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -Wno-unused-parameter")
    set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g")
    set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O2")
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
    if (CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else ()
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif ()
endif ()

find_package (Boost REQUIRED)
find_package (OpenGL REQUIRED)
find_package (GLEW REQUIRED)
find_package (SDL2 REQUIRED)
find_package (SDL2_image REQUIRED)
include_directories (
    ${BOOST_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIRS}
    ${SDL2_INCLUDE_DIR}
    ${SDL2_IMAGE_INCLUDE_DIR}
    "deps/RxCpp/Rx/v2/src"
)

add_executable (rpec
    "src/main.cpp"
    "src/events/sdlevents.cpp"
)

target_link_libraries(rpec
    ${BOOST_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${SDL2_LIBRARY}
    ${SDL2_IMAGE_LIBRARY}
)

install (TARGETS rpec
    RUNTIME DESTINATION bin
)

if (APPLE)
    if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
        configure_file ( src/MacOS/Info.plist.cmake MacOS/Info.plist )
        set_target_properties (rpec PROPERTIES
            MACOSX_BUNDLE TRUE
            MACOSX_BUNDLE_INFO_PLIST ${rpec_BINARY_DIR}/MacOS/Info.plist
    )
    endif ()
endif (APPLE)

set (CPACK_PACKAGE_NAME ${rpec_PACKAGE_NAME})
set (CPACK_PACKAGE_DESCRIPTION_SUMMARY ${rpec_DESCRIPTION_SUMMARY})
set (CPACK_PACKAGE_VERSION ${rpec_VERSION})
set (CPACK_PACKAGE_VERSION_MAJOR ${rpec_VERSION_MAJOR})
set (CPACK_PACKAGE_VERSION_MINOR ${rpec_VERSION_MINOR})
set (CPACK_PACKAGE_VERSION_PATCH ${rpec_VERSION_PATCH})

set (CPACK_GENERATOR "TGZ")
if (APPLE)
    if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
        set (CPACK_GENERATOR ${CPACK_GENERATOR} "DragNDrop")
    endif ()
endif (APPLE)

include (CPack)

