 find_package(Qt4 REQUIRED)
 
 find_path(QWT_INCLUDE_DIR
     NAMES qwt_plot.h
     HINTS ${QT_INCLUDE_DIR}
     PATH_SUFFIXES qwt qwt-qt3 qwt-qt4 qwt-qt5
)

find_library(QWT_LIBRARIES
	NAMES qwt qwt-qt4
)

QT4_ADD_RESOURCES(VIZ_RESC ressources.qrc)

INCLUDE_DIRECTORIES(${QWT_INCLUDE_DIR})

rock_executable(viz 
    SOURCES
        main.cpp
        replay_gui.cpp
        ${VIZ_RESC}
    HEADERS
        replay_gui.h
    MOC
    	replay_gui.h
    	ressources.qrc
    DEPS_PKGCONFIG
        QtCore
        QtGui
    DEPS
    	rock_replay
    UI
        main.ui
)



TARGET_LINK_LIBRARIES(viz ${QWT_LIBRARIES})