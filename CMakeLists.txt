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
    #________________________________
    sources/mainwindow.cpp
    sources/server.cpp
    #__________________________
    sources/clientConnection.cpp
    sources/dispatcher.cpp
    #________________________________
    sources/icommand/registry.cpp
    sources/icommand/login.cpp
    sources/icommand/get.cpp
    sources/icommand/edit.cpp
    sources/icommand/update.cpp
    sources/icommand/pull.cpp
    #________________________________
    headers/icommand/icommand.h
    headers/icommand/registry.h
    headers/icommand/login.h
    headers/icommand/get.h
    headers/icommand/edit.h
    headers/icommand/update.h
    headers/icommand/pull.h
    #________________________________
    headers/mainwindow.h
    headers/server.h
    #__________________________
    headers/clientConnection.h
    headers/dispatcher.h
    headers/utils.h
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
