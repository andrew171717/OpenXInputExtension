cmake_policy(SET CMP0091 NEW)
project(OpenXinput)
cmake_minimum_required(VERSION 3.15)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

option(OPENXINPUT_STATIC_GUIDS     "Static GUIDs instead. (Uses less dependencies)" ON)
option(OPENXINPUT_DISABLE_COM      "Disable COM calls." ON)

set(OPENXINPUT_XUSER_MAX_COUNT 8 CACHE STRING "Max controller count supported by OpenXinput. (Can't be less than 4 because of Xinput compatibility)")

option(OPENXINPUT_BUILD_EXTENSION    "Build OpenXInputExtension." ON)

option(OPENXINPUT_BUILD_EXAMPLE    "Build OpenXinput example." ON)

option(BUILD_SHARED_LIBS "Build OpenXinput as a Xinput replacement. If set to no, all exported library functions will be prepended with 'Open', like 'OpenXinputGetState'." ON)

##################
## DEVOBJ
add_library(devobj SHARED
  DEVOBJ/DEVOBJ.c
  DEVOBJ/DEVOBJ.h
  DEVOBJ/DEVOBJ.def
)

set_target_properties(devobj
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/dustbin/bin"
)

##################
## OpenXinput
set(LIBRARY_NAME Xinput1_4)

set(OPENXINPUT_SOURCES
  src/OpenXinput.cpp
)

set(OPENXINPUT_HEADERS
  src/OpenXinput.h
  src/OpenXinputInternal.h
)

add_library(${LIBRARY_NAME}
  $<$<BOOL:${BUILD_SHARED_LIBS}>:src/OpenXinput.def>
  ${OPENXINPUT_SOURCES}
  ${OPENXINPUT_HEADERS}
)

set_target_properties(${LIBRARY_NAME} PROPERTIES
  MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
  POSITION_INDEPENDENT_CODE ON
  C_VISIBILITY_PRESET hidden
  CXX_VISIBILITY_PRESET hidden
  VISIBILITY_INLINES_HIDDEN ON
)

target_include_directories(${LIBRARY_NAME}
  PRIVATE
  DEVOBJ
)
 
target_link_libraries(${LIBRARY_NAME}
  PUBLIC
  devobj
  
  mincore
  WindowsApp
)

target_compile_definitions(${LIBRARY_NAME}
  PRIVATE
  $<$<BOOL:${OPENXINPUT_STATIC_GUIDS}>:OPENXINPUT_STATIC_GUIDS>
  $<$<BOOL:${OPENXINPUT_DISABLE_COM}>:OPENXINPUT_DISABLE_COM>
  $<$<BOOL:${OPENXINPUT_XUSER_MAX_COUNT}>:OPENXINPUT_XUSER_MAX_COUNT=${OPENXINPUT_XUSER_MAX_COUNT}>
  
  PUBLIC
  $<$<BOOL:${BUILD_SHARED_LIBS}>:OPENXINPUT_BUILD_SHARED>
)

set_target_properties(${LIBRARY_NAME}
  PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

##################
## GameMaker Extension
if(${OPENXINPUT_BUILD_EXTENSION})

add_library(OpenXInputExtension
  GMOpenXInputExtension/main.cpp
)

# Set the build type to Release
set(CMAKE_BUILD_TYPE "Release")

# Add compiler and linker flags to control the runtime library
if(MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /NODEFAULTLIB:msvcp140d")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /NODEFAULTLIB:vcruntime140d")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /NODEFAULTLIB:vcruntime140_1d")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /NODEFAULTLIB:ucrtbased")
endif()

target_link_libraries(OpenXInputExtension
  PRIVATE
  ${LIBRARY_NAME}
)

target_include_directories(OpenXInputExtension
  PRIVATE
  src/
)

set_target_properties(OpenXInputExtension
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

endif()

##################
## Examples
if(${OPENXINPUT_BUILD_EXAMPLE})

add_executable(openxinput_example
  example/main.cpp
)

target_link_libraries(openxinput_example
  PRIVATE
  ${LIBRARY_NAME}
)

target_include_directories(openxinput_example
  PRIVATE
  src/
)

set_target_properties(openxinput_example
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

endif()

##################
## Install rules
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR})

install(
  TARGETS ${LIBRARY_NAME}
  RUNTIME DESTINATION "output/bin"
  LIBRARY DESTINATION "output/lib"
)

install(FILES
  $<$<AND:$<BOOL:${BUILD_SHARED_LIBS}>,$<BOOL:${MSVC}>>:$<TARGET_PDB_FILE:${LIBRARY_NAME}>> DESTINATION "output/pdb" OPTIONAL
)