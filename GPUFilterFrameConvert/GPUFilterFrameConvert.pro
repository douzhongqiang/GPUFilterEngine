QT       += core gui opengl

TEMPLATE = lib
DEFINES += GPUFILTERFRAMECONVERT_LIBRARY

CONFIG += c++11

MOC_DIR += $$PWD/temp
OBJECTS_DIR += $$PWD/temp
DESTDIR += $$PWD/../GPUFilterTestApp/bin

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$PWD/../
INCLUDEPATH += $$PWD/../GPUFilterCore

LIBS += -L$$DESTDIR -lGPUFilterCore

INCLUDEPATH += $$PWD/../3party/ffmpeg/include
LIBS += -L$$PWD/../3party/ffmpeg/libs -lavcodec -lavformat -lavutil -lswscale -lswresample

SOURCES += \
    GPUFilterFrameConvertCore.cpp \
    gpufilterframeconvert.cpp

HEADERS += \
    GPUFilterFrameConvertCore.h \
    GPUFilterFrameConvert_global.h \
    gpufilterframeconvert.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
