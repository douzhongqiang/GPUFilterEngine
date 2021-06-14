QT += core gui opengl

TEMPLATE = lib
DEFINES += GPUFILTERMODEL_LIBRARY

CONFIG += c++11

MOC_DIR += $$PWD/temp
OBJECTS_DIR += $$PWD/temp
DESTDIR += $$PWD/../GPUFilterTestApp/bin

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    GPUFilterAnimation.cpp \
    GPUFilterAnimator.cpp \
    GPUFilterBone.cpp \
    GPUFilterBoneMesh.cpp \
    GPUFilterModel.cpp \
    GPUFilterNode.cpp

HEADERS += \
    GPUFilterAnimation.h \
    GPUFilterAnimator.h \
    GPUFilterBone.h \
    GPUFilterBoneMesh.h \
    GPUFilterModel.h \
    GPUFilterModel_global.h \
    GPUFilterNode.h

INCLUDEPATH += $$PWD/../GPUFilterCore
INCLUDEPATH += $$PWD/../3party/assimp/include

LIBS += -L$$PWD/../GPUFilterTestApp/bin -lGPUFilterCore
CONFIG(debug, debug|release) {
    #VS2019
    LIBS += -L$$PWD/../3party/assimp/lib/Debug -lassimp-vc140-mt
}
else{
    LIBS += -L$$PWD/../3party/assimp/lib/Release -lassimp-vc140-mt
}

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
