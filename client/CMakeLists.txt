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
    mainwindow.cpp mainwindow.h
    appContext.h
    #_________________________________________
    #___________Sources_______________________
    sources/client.cpp
    #___________DBHelpers____________
    sources/DBHelpers/dbhelper.cpp
    sources/DBHelpers/dbcrud.cpp
    #__________Controllers____________
    sources/controllers/controller.cpp
    #____________Models______________
    sources/models/tableModel.cpp
    #______________UI________________
    sources/UI/authPage.cpp
    sources/UI/mainPage.cpp
    #_________________________________________
    #___________Headers_______________________
    headers/client.h
    #___________DBHelpers____________
    headers/DBHelpers/dbhelper.h
    headers/DBHelpers/dbcrud.h
    #__________Controllers____________
    headers/controllers/controller.h
    #_____________Utils_______________
    headers/utils/utils.h
    headers/utils/packetQueryStructure.h
    headers/utils/serverQueryStructure.h
    headers/utils/serverResponseStructure.h
    #___________OpCodes______________
    headers/OpCodes/localOpcode.h
    headers/OpCodes/serverOpcode.h
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
    ${CMAKE_CURRENT_SOURCE_DIR}/headers/DBHelpers
    ${CMAKE_CURRENT_SOURCE_DIR}/headers/dispatchers
    ${CMAKE_CURRENT_SOURCE_DIR}/headers/controllers
    ${CMAKE_CURRENT_SOURCE_DIR}/headers/utils
    ${CMAKE_CURRENT_SOURCE_DIR}/headers/OpCodes
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
