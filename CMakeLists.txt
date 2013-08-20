project(PLOTAPP CXX)
cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_FLAGS "-Wall -std=c++11 ${CMAKE_CXX_FLAGS}")

find_package(Qt4 REQUIRED QtCore QtGui)
include(${QT_USE_FILE})

include_directories(${PLOTAPP_SOURCE_DIR}/include
                    ${PLOTAPP_SOURCE_DIR}/qcustomplot
                    ${QT_INCLUDE_DIR})

add_subdirectory(${PLOTAPP_SOURCE_DIR}/source)

