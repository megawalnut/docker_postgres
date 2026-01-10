cmake_minimum_required(VERSION 3.16)
project(pet_proj_1 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_AUTOUIC_SEARCH_PATHS
    ${CMAKE_CURRENT_SOURCE_DIR}/ui
)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Core Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Core Network)

set(PROJECT_SOURCES
    main.cpp
    ui/mainwindow.ui
    sources/mainwindow.cpp
    sources/server.cpp
    sources/clientConnection.cpp
    sources/dispatcher.cpp
    sources/packet.cpp
    headers/mainwindow.h
    headers/server.h
    headers/clientConnection.h
    headers/dispatcher.h
    headers/icommand/icommand.h
    headers/packet.h
)

if(QT_VERSION_MAJOR GREATER_EQUAL 6)
    qt_add_executable(pet_proj_1
        ${PROJECT_SOURCES}
    )
else()
    add_executable(pet_proj_1
        ${PROJECT_SOURCES}
    )
endif()

target_include_directories(pet_proj_1 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/headers
    ${CMAKE_CURRENT_SOURCE_DIR}/headers/icommand
)

target_link_libraries(pet_proj_1 PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Network
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(pet_proj_1)
endif()
