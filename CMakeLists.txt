#This makefile is in charge of creating the executables and the library
cmake_minimum_required(VERSION 2.8)

# set the project name
project(TCPcannon)

# Add the needed flags for compiler
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -D_DEFAULT_SOURCE")

#Adding executables
include_directories(include src modules)

#This is to let cmake know that the library used later is not under /usr/bin, but rather in our custom dir
SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

add_executable(TCPcannon main.c
    include/SYNflooding.h modules/SYNflooding.c
    include/iputils.h     src/iputils.c
    include/randomCWMC.h  src/randomCWMC.c)

#We link the target with the library
include_directories(lib)

target_link_libraries(TCPcannon ${CMAKE_CURRENT_SOURCE_DIR}/lib/libRawTCP_Lib.a)

#We create the executables in the destination folder bin
install(TARGETS TCPcannon RUNTIME DESTINATION bin)
