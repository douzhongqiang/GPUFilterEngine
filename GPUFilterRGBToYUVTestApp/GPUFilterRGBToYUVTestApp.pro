QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

MOC_DIR += $$PWD/temp
OBJECTS_DIR += $$PWD/temp
DESTDIR += $$PWD/../GPUFilterTestApp/bin

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    widget.cpp

HEADERS += \
    widget.h

INCLUDEPATH += $$PWD/../
INCLUDEPATH += $$PWD/../GPUFilterCore
INCLUDEPATH += $$PWD/../GPUFilterVideoCore
INCLUDEPATH += $$PWD/../GPUFilterModel
INCLUDEPATH += $$PWD/../GPUFilterGeneralScene
INCLUDEPATH += $$PWD/../GPUFilterFrameConvert

LIBS += -L$$DESTDIR -lGPUFilterCore
LIBS += -L$$DESTDIR -lGPUFilterVideoCore
LIBS += -L$$DESTDIR -lGPUFilterModel
LIBS += -L$$DESTDIR -lGPUFilterGeneralScene
LIBS += -L$$DESTDIR -lGPUFilterCore

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

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
