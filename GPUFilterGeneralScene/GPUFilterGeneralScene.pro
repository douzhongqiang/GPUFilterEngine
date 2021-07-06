QT       += core gui opengl

TEMPLATE = lib
DEFINES += GPUFILTERGENERALSCENE_LIBRARY

CONFIG += c++11

MOC_DIR += $$PWD/temp
OBJECTS_DIR += $$PWD/temp
DESTDIR += $$PWD/../GPUFilterTestApp/bin

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$PWD/../
INCLUDEPATH += $$PWD/../GPUFilterCore
INCLUDEPATH += $$PWD/../GPUFilterVideoCore
INCLUDEPATH += $$PWD/../GPUFilterModel

LIBS += -L$$DESTDIR -lGPUFilterCore
LIBS += -L$$DESTDIR -lGPUFilterVideoCore
LIBS += -L$$DESTDIR -lGPUFilterModel

INCLUDEPATH += $$PWD/../3party/ffmpeg/include
LIBS += -L$$PWD/../3party/ffmpeg/libs -lavcodec -lavformat -lavutil -lswscale -lswresample

INCLUDEPATH += $$PWD/../3party/assimp/include
CONFIG(debug, debug|release) {
    #VS2019
    LIBS += -L$$PWD/../3party/assimp/lib/Debug -lassimp-vc140-mt
}
else{
    LIBS += -L$$PWD/../3party/assimp/lib/Release -lassimp-vc140-mt
}

SOURCES += \
    GPUFilterFloorMesh.cpp \
    GPUFilterInvertedMesh.cpp \
    GPUFilterVideoPlayerScene.cpp

HEADERS += \
    GPUFilterFloorMesh.h \
    GPUFilterGeneralScene_global.h \
    GPUFilterInvertedMesh.h \
    GPUFilterVideoPlayerScene.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
