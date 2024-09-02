cmake_minimum_required(VERSION 3.16)

project(FlowchartEditor VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
include_directories(${PROJECT_SOURCE_DIR})

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(FlowchartEditor
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        model/elements/connectorelement.cpp model/elements/connectorelement.h model/elements/dataelement.cpp model/elements/dataelement.h model/elements/decisionelement.cpp model/elements/decisionelement.h model/elements/flowchartelement.cpp model/elements/flowchartelement.h model/elements/label.cpp model/elements/label.h model/elements/line.cpp model/elements/line.h model/elements/processelement.cpp model/elements/processelement.h model/elements/startendelement.cpp model/elements/startendelement.h model/config.h view/canvas.cpp view/canvas.h main.cpp mainwindow.cpp mainwindow.h mainwindow.ui res.qrc
        controller/drawer.h controller/drawer.cpp
        controller/controller.h controller/controller.cpp
        controller/updater.h controller/updater.cpp
        controller/remover.h controller/remover.cpp
        controller/filer.h controller/filer.cpp


    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET FlowchartEditor APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(FlowchartEditor SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(FlowchartEditor
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(FlowchartEditor PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.FlowchartEditor)
endif()
set_target_properties(FlowchartEditor PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS FlowchartEditor
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(FlowchartEditor)
endif()
