####################################################################################################
 #
 #
 #   Hextontiler
 #   Copyright (C) 2020 Ivo Filot <ivo@ivofilot.nl>
 #
 #
####################################################################################################

HEADERS       = src/data/map.h \
                src/data/map_io.h \
                src/data/tile.h \
                src/data/tile_manager.h \
                src/gui/anaglyph_widget.h \
                src/gui/bom_widget.h \
                src/gui/interface_window.h \
                src/gui/mainwindow.h \
                src/gui/shader_program_types.h \
                src/gui/shader_program.h \
                src/gui/shader_program_manager.h \
                src/gui/map_renderer.h \
                src/gui/scene.h \
                src/gui/tile_selector.h \
                src/gui/user_action.h \
                src/config.h

SOURCES       = src/main.cpp \
                src/data/map.cpp \
                src/data/map_io.cpp \
                src/data/tile.cpp \
                src/data/tile_manager.cpp \
                src/gui/anaglyph_widget.cpp \
                src/gui/bom_widget.cpp \
                src/gui/interface_window.cpp \
                src/gui/mainwindow.cpp \
                src/gui/shader_program.cpp \
                src/gui/shader_program_manager.cpp \
                src/gui/map_renderer.cpp \
                src/gui/scene.cpp \
                src/gui/tile_selector.cpp \
                src/gui/user_action.cpp

QT           += core gui widgets
CONFIG       += force_debug_info
CONFIG       += c++17

linux {
    QMAKE_CXXFLAGS+= -fopenmp
    QMAKE_LFLAGS +=  -fopenmp
    LIBS += -lboost_regex -lboost_iostreams -lboost_filesystem -lboost_system
}

win32 {
    INCLUDEPATH += ../../Libraries/boost-1.70.0-win-x64/include
    INCLUDEPATH += ../../Libraries/glm-0.9.8.4-win-x64
    Release:LIBS += "-L../../../Libraries/boost-1.70.0-win-x64/lib" -lboost_regex-vc141-mt-x64-1_70 -lboost_bzip2-vc141-mt-x64-1_70
    Debug:LIBS += "-L../../../Libraries/boost-1.70.0-win-x64/lib" -lboost_regex-vc141-mt-gd-x64-1_70 -lboost_bzip2-vc141-mt-gd-x64-1_70
}

RESOURCES += \
    resources.qrc \
    assets/tiles/tiles_isometric \
    assets/tiles/icons_isometric \
    assets/icons
