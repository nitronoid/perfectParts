TARGET=perfectParts
DESTDIR=./

SOURCES+=\
    $$PWD/src/main.cpp \
    $$PWD/src/Scene.cpp \
    $$PWD/src/Particle.cpp \
    $$PWD/src/FireworkParticle.cpp \
    $$PWD/src/FlameParticle.cpp \
    $$PWD/src/ExplosionParticle.cpp \
    $$PWD/src/Emitter.cpp \
    $$PWD/src/pngutils.cpp \
    $$PWD/imgui/src/imgui.cpp \
    $$PWD/imgui/src/imgui_draw.cpp \
    $$PWD/imgui/src/ImGUIImpl.cpp \
    $$PWD/imgui/src/colourpick.cpp \
    $$PWD/imgui/src/ImGuizmo.cpp

CONFIG += c++11
CONFIG-=app_bundle
CONFIG += debug

QT += opengl
QT -= core
QT -=xml

HEADERS += \
    $$PWD/include/Scene.h \
    $$PWD/include/Particle.h \
    $$PWD/include/FireworkParticle.h \
    $$PWD/include/FlameParticle.h \
    $$PWD/include/ExplosionParticle.h \
    $$PWD/include/Emitter.h \
    $$PWD/include/pngutils.h \
    $$PWD/imgui/include/imgui.h \
    $$PWD/imgui/include/stb_rect_pack.h \
    $$PWD/imgui/include/stb_truetype.h \
    $$PWD/imgui/include/imconfig.h \
    $$PWD/imgui/include/imgui_internal.h \
    $$PWD/imgui/include/stb_textedit.h \
    $$PWD/imgui/include/ImGUIImpl.h \
    $$PWD/imgui/include/ImGuizmo.h

INCLUDEPATH +=include
INCLUDEPATH +=imgui/include
INCLUDEPATH +=/usr/local/include

OTHER_FILES+=imgui/imgui.ini

QMAKE_CXXFLAGS+= -msse -msse2 -msse3
QMAKE_CXXFLAGS+= -std=c++11 -O3
QMAKE_CXXFLAGS += -isystem "$$PWD/imgui/include" -isystem "$$PWD/imgui/src"

linux-g++:QMAKE_CXXFLAGS +=  -march=native
linux-g++-64:QMAKE_CXXFLAGS +=  -march=native

QMAKE_CFLAGS+=$$system(sdl2-config  --cflags)
message(output from sdl2-config --cflags added to CXXFLAGS= $$QMAKE_CFLAGS)
INCLUDEPATH+=/usr/local/include/SDL2
LIBS+=$$system(sdl2-config  --libs)
message(output from sdl2-config --libs added to LIB=$$LIBS)
LIBS += -L/usr/local/lib -lpng
linux: {
                DEFINES +=GL_GLEXT_PROTOTYPES
                LIBS+=-lGLU
}
