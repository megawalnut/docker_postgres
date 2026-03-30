cmake_minimum_required(VERSION 3.16)
project(client VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_AUTOUIC_SEARCH_PATHS
    ${CMAKE_CURRENT_SOURCE_DIR}/ui
)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Network Sql Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Network Sql Widgets)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    #_________________________________________
    #___________Sources_______________________
    sources/client.cpp
    sources/dispatcher.cpp
    sources/dbhelper.cpp
    sources/controller.cpp
    #___________ICommand_____________
    sources/icommand/registry.cpp
    sources/icommand/login.cpp
    sources/icommand/get.cpp
    sources/icommand/update.cpp
    #____________Models______________
    sources/models/tableModel.cpp
    #______________UI________________
    sources/UI/authPage.cpp
    sources/UI/mainPage.cpp
    #_________________________________________
    #___________Headers_______________________
    headers/client.h
    headers/dispatcher.h
    headers/utils.h
    headers/dbhelper.h
    headers/controller.h
    headers/localOpcode.h
    headers/serverOpcode.h
    #___________ICommand_____________
    headers/icommand/icommand.h
    headers/icommand/registry.h
    headers/icommand/login.h
    headers/icommand/get.h
    headers/icommand/update.h
    #____________Models______________
    headers/models/tableModel.h
    #______________UI________________
    headers/UI/authPage.h
    headers/UI/mainPage.h
    #_________________________________________
    #_________________________________________
)

if(QT_VERSION_MAJOR GREATER_EQUAL 6)
    qt_add_executable(client
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    add_executable(client
        ${PROJECT_SOURCES}
    )
endif()

target_include_directories(client PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/headers
    ${CMAKE_CURRENT_SOURCE_DIR}/headers/icommand
)

target_link_libraries(client PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Sql
    Qt${QT_VERSION_MAJOR}::Widgets
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(client)
endif()
