cmake_minimum_required(VERSION 3.5)

project(RobotSimulation VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Svg OpenGL)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Svg OpenGL)

# Eigen
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/eigen-3.4.0)

# framelesshelper
set(FRAMELESSHELPER_BUILD_STATIC ON)
set(FRAMELESSHELPER_BUILD_QUICK OFF)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/framelesshelper-2.5.0")
add_library(framelesshelper INTERFACE)

# qrc
set(QRC_SOURCE_FILES res.qrc)
qt5_add_resources(QRC_FILES ${QRC_SOURCE_FILES})

# src
file(GLOB_RECURSE PROJECT_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

# froms
file(GLOB_RECURSE PROJECT_FROMS ${CMAKE_CURRENT_SOURCE_DIR}/froms/*.ui)

set(PROJECT_SOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(
        ${PROJECT_SOURCES_DIR}
)


if (${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(RobotSimulation
            MANUAL_FINALIZATION
            ${PROJECT_SOURCES}
            ${PROJECT_FROMS}
            "icon.rc"
            )
    # Define target properties for Android with Qt 6 as:
    #    set_property(TARGET RobotSimulation APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
    #                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
    # For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else ()
    if (ANDROID)
        add_library(RobotSimulation SHARED
                ${PROJECT_SOURCES}
                ${PROJECT_FROMS}
                )
        # Define properties for Android with Qt 5 after find_package() calls as:
        #    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else ()
        add_executable(RobotSimulation
                ${PROJECT_SOURCES}
                ${PROJECT_FROMS}
                ${QRC_FILES}
                "icon.rc"
                )
    endif ()
endif ()

if (CMAKE_SYSTEM_NAME MATCHES "Linux")
    target_link_libraries(RobotSimulation PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Svg Qt${QT_VERSION_MAJOR}::OpenGL OpenGL FramelessHelperCore FramelessHelperWidgets)
elseif (CMAKE_SYSTEM_NAME MATCHES "Windows")
    target_link_libraries(RobotSimulation PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Svg Qt${QT_VERSION_MAJOR}::OpenGL OpenGL32 FramelessHelperCore FramelessHelperWidgets)
endif ()


set_target_properties(RobotSimulation PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
        )

install(TARGETS RobotSimulation
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if (QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(RobotSimulation)
endif ()
