cmake_minimum_required(VERSION 3.1.0)

project ( cliente )

# declaring files in your project

set ( SOURCES
 main.cpp
 mainwindow.cpp
)

set ( MOC_HEADERS
 mainwindow.h
 cliente.h
)

set ( UIS
 mainwindow.ui
)

set ( SERVER
 server.c
)


# Next lines needed for building all Qt projects
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Qt4 REQUIRED QtCore QtGui)
include_directories(${QT_INCLUDES})
add_definitions(${QT_DEFINITIONS})





# Next, using precompiler, compiler and linker

# using Qt meta-system (precompiler)
QT4_WRAP_UI( UI_HEADERS ${UIS} )
QT4_WRAP_CPP( MOC_SRCS ${MOC_HEADERS} )

# compile
add_executable( cliente ${SOURCES} ${MOC_HEADERS} ${UIS} )
add_executable( server ${SERVER}) 


# build it (link libraries)
target_link_libraries( cliente Qt4::QtGui)
