cmake_minimum_required(VERSION 3.5)
project(pencil_mob)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# QTDIR must point to c:/Qt/5.7/mingw53_32/ or similar
message("qtdir is set to '$ENV{QTDIR}' (must be 'c:/Qt/5.7/mingw53_32/' or similar)")
set(CMAKE_PREFIX_PATH "$ENV{QTDIR}")

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

# Find the QtWidgets library
find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network REQUIRED)

include_directories(src)

set(CLIENT_NAME "pencil_mob")

set(CLIENT_SOURCES
        src/main_client.cpp
        src/pencil_mob.ui
        src/ClientMainWindow.cpp
        src/ClientMainWindow.h)

set(SERVER_NAME "pencil_mob_server")

set(SERVER_SOURCES
        src/main_server.cpp
        src/pencil_mob_server.ui
        src/ServerMainWindow.cpp
        src/ServerMainWindow.h)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

add_executable(${CLIENT_NAME} ${CLIENT_SOURCES})

add_executable(${SERVER_NAME} ${SERVER_SOURCES})

target_link_libraries(${CLIENT_NAME} Qt5::Widgets Qt5::Core Qt5::Network)

target_link_libraries(${SERVER_NAME} Qt5::Widgets Qt5::Core Qt5::Network)
