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

set(CLIENT_NAME "pencil_mob")
set(SERVER_NAME "pencil_mob_server")

set(COMMON_SOURCES transport.cpp transport.h)
add_library(pencil_mob_common ${COMMON_SOURCES})
target_link_libraries(pencil_mob_common Qt5::Core)

set(CLIENT_SOURCES
        main_client.cpp
        pencil_mob.ui
        ClientMainWindow.cpp
        ClientMainWindow.h
        CanvasWidget.cpp
        CanvasWidget.h)

set(SERVER_SOURCES
        main_server.cpp
        pencil_mob_server.ui
        ServerMainWindow.cpp
        ServerMainWindow.h)

add_executable(${CLIENT_NAME} ${CLIENT_SOURCES})
target_link_libraries(${CLIENT_NAME} pencil_mob_common Qt5::Widgets Qt5::Core Qt5::Network)

add_executable(${SERVER_NAME} ${SERVER_SOURCES})
target_link_libraries(${SERVER_NAME} pencil_mob_common Qt5::Widgets Qt5::Core Qt5::Network)
