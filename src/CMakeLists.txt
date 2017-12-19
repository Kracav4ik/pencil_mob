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

set(COMMON_SOURCES
        transport.cpp
        transport.h
        enums.h
        messages.h
        MessageHandler.h
        MessageHandler.cpp
        Painting.cpp
        Painting.h
        Stroke.cpp
        Stroke.h
        Layer.cpp
        Layer.h
        Serializable.h
        enums.cpp
        LayerId.h
    )

add_library(pencil_mob_common ${COMMON_SOURCES})
target_link_libraries(pencil_mob_common Qt5::Core Qt5::Widgets)

set(CLIENT_SOURCES
        main_client.cpp
        pencil_mob.ui
        ClientMainWindow.cpp
        ClientMainWindow.h
        widgets/CanvasWidget.cpp
        widgets/CanvasWidget.h
        widgets/color_chooser.ui
        widgets/ColorChooserWidget.cpp
        widgets/ColorChooserWidget.h
        widgets/CustomPaintWidget.cpp
        widgets/CustomPaintWidget.h
        TextProgress.h
        widgets/tool_selector.ui
        widgets/ToolSelectorWidget.cpp
        widgets/ToolSelectorWidget.h
        tools/Tool.cpp
        tools/Tool.h
        tools/PenTool.cpp
        tools/PenTool.h
        widgets/ToolSelectorButton.cpp
        widgets/ToolSelectorButton.h
        tools/EraserTool.cpp
        tools/EraserTool.h
        widgets/layers.ui
        widgets/LayersWidget.cpp
        widgets/LayersWidget.h
        widgets/layer_button.ui
        widgets/LayerButtonWidget.cpp
        widgets/LayerButtonWidget.h
        Camera.cpp
        Camera.h
        widgets/MessagesWidget.cpp
        widgets/MessagesWidget.h
        widgets/messages.ui
        widgets/ListOfVisibleUsersWidget.cpp
        widgets/ListOfVisibleUsersWidget.h
        widgets/users.ui
        widgets/UserWidget.cpp
        widgets/UserWidget.h
        widgets/user.ui
        )

set(SERVER_SOURCES
        main_server.cpp
        pencil_mob_server.ui
        ServerMainWindow.cpp
        ServerMainWindow.h)

add_executable(${CLIENT_NAME} ${CLIENT_SOURCES})
target_link_libraries(${CLIENT_NAME} pencil_mob_common Qt5::Widgets Qt5::Core Qt5::Network)

add_executable(${SERVER_NAME} ${SERVER_SOURCES})
target_link_libraries(${SERVER_NAME} pencil_mob_common Qt5::Widgets Qt5::Core Qt5::Network)
