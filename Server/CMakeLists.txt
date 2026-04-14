cmake_minimum_required(VERSION 3.16)
project(server VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Network Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Network Sql)

set(PROJECT_SOURCES
    main.cpp
    #_________________________________________
    #___________Sources_______________________
    sources/server.cpp
    sources/clientConnection.cpp
    sources/dispatcher.cpp
    #___________DBHelpers____________
    sources/DBHelpers/dbhelper.cpp
    #___________ICommands____________
    sources/Icommand/registry.cpp
    sources/Icommand/login.cpp
    sources/Icommand/sync.cpp
    sources/Icommand/rollback.cpp
    sources/Icommand/fulldump.cpp
    #_________________________________________
    #___________Headers_______________________
    headers/server.h
    headers/clientConnection.h
    headers/dispatcher.h
    #_____________Utils_______________
    headers/Utils/utils.h
    headers/Utils/keys.h
    headers/Utils/protocol.h
    headers/Utils/userInfo.h
    #___________OpCodes______________
    headers/Opcodes/localOpcode.h
    headers/Opcodes/serverOpcode.h
    #___________DBHelpers____________
    headers/DBHelpers/dbhelper.h
    #___________ICommands____________
    headers/Icommand/icommand.h
    headers/Icommand/registry.h
    headers/Icommand/login.h
    headers/Icommand/sync.h
    headers/Icommand/rollback.h
    headers/Icommand/fulldump.h
    #_________________________________________
    #_________________________________________
)

if(QT_VERSION_MAJOR GREATER_EQUAL 6)
    qt_add_executable(Server
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    add_executable(server
        ${PROJECT_SOURCES}
    )
endif()

target_include_directories(Server PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/headers
    ${CMAKE_CURRENT_SOURCE_DIR}/headers/Icommand
    ${CMAKE_CURRENT_SOURCE_DIR}/headers/DBHelpers
    ${CMAKE_CURRENT_SOURCE_DIR}/headers/Utils
    ${CMAKE_CURRENT_SOURCE_DIR}/headers/Opcodes
)

target_link_libraries(Server PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Sql
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Server)
endif()
