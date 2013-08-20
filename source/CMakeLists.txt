set(PLOTAPP_QT_HEADERS ${PLOTAPP_SOURCE_DIR}/include/mainwindow.h
                       ${PLOTAPP_SOURCE_DIR}/qcustomplot/qcustomplot.h)
qt4_wrap_cpp(PLOTAPP_QT_HEADERS_MOC ${PLOTAPP_QT_HEADERS})

set(PLOTAPP_SOURCES main.cc
                    mainwindow.cc
                    ${PLOTAPP_SOURCE_DIR}/qcustomplot/qcustomplot.cpp)

add_executable(plotapp ${PLOTAPP_SOURCES}
                       ${PLOTAPP_QT_HEADERS_MOC}
                       ${PLOTAPP_SOURCE_DIR}/qcustomplot/qcustomplot.cpp)
target_link_libraries(plotapp ${QT_LIBRARIES})

