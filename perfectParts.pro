TARGET=perfectParts
DESTDIR=./
SOURCES+=\
    $$PWD/src/main.cpp \
    $$PWD/src/Window.cpp \
    $$PWD/src/Particle.cpp \
    $$PWD/src/FireworkParticle.cpp \
    $$PWD/src/FlameParticle.cpp \
    $$PWD/src/Emitter.cpp \
    $$PWD/src/pngutils.cpp \
    $$PWD/imgui/src/imgui.cpp \
    $$PWD/imgui/src/imgui_draw.cpp \
    $$PWD/imgui/src/ColourPicker.cpp \
    $$PWD/src/ImGUIImpl.cpp \
    $$PWD/src/ExplosionParticle.cpp


CONFIG += c++11
CONFIG-=app_bundle
CONFIG += debug

QT += opengl
QT -= core
QT -=xml


HEADERS += \
    $$PWD/include/Window.h \
    $$PWD/include/Particle.h \
    $$PWD/include/FireworkParticle.h \
    $$PWD/include/FlameParticle.h \
    $$PWD/include/Emitter.h \
    $$PWD/include/ParticleType.h \
    $$PWD/include/pngutils.h \
    $$PWD/imgui/include/imgui.h \
    $$PWD/imgui/include/stb_rect_pack.h \
    $$PWD/imgui/include/stb_truetype.h \
    $$PWD/imgui/include/imconfig.h \
    $$PWD/imgui/include/imgui_internal.h \
    $$PWD/imgui/include/stb_textedit.h \
    $$PWD/include/ImGUIImpl.h \
    $$PWD/include/ExplosionParticle.h

INCLUDEPATH +=include
INCLUDEPATH +=imgui/include

OTHER_FILES+=imgui.ini

QMAKE_CXXFLAGS+= -msse -msse2 -msse3
QMAKE_CXXFLAGS += -std=c++11

linux-g++:QMAKE_CXXFLAGS +=  -march=native
linux-g++-64:QMAKE_CXXFLAGS +=  -march=native

QMAKE_CFLAGS+=$$system(sdl2-config  --cflags)
message(output from sdl2-config --cflags added to CXXFLAGS= $$QMAKE_CFLAGS)
INCLUDEPATH+=/usr/local/include/SDL2
LIBS+=$$system(sdl2-config  --libs)
message(output from sdl2-config --libs added to LIB=$$LIBS)
LIBS += -L/usr/local/lib  -lGLU -lGL -lpng
linux-g++* {
                DEFINES +=GL_GLEXT_PROTOTYPES
                LIBS+=-lGLU

}
linux-clang* {
                DEFINES +=GL_GLEXT_PROTOTYPES
                LIBS+=-lGLU
}
