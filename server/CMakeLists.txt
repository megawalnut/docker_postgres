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

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Network Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Network Sql)

set(PROJECT_SOURCES
    main.cpp
    #_________________________________________
    #_________________________________________
    sources/server.cpp
    sources/clientConnection.cpp
    sources/dispatcher.cpp
    sources/dbhelper.cpp
    #________________________________
    sources/icommand/registry.cpp
    sources/icommand/login.cpp
    sources/icommand/get.cpp
    sources/icommand/update.cpp
    #_________________________________________
    #_________________________________________
    headers/server.h
    headers/clientConnection.h
    headers/dispatcher.h
    headers/utils.h
    headers/dbhelper.h
    #________________________________
    headers/icommand/icommand.h
    headers/icommand/registry.h
    headers/icommand/login.h
    headers/icommand/get.h
    headers/icommand/update.h
    #_________________________________________
    #_________________________________________
)

if(QT_VERSION_MAJOR GREATER_EQUAL 6)
    qt_add_executable(pet_proj_1
        MANUAL_FINALIZATION
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
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Sql
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(pet_proj_1)
endif()
