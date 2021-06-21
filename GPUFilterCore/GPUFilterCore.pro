QT += core gui opengl

TEMPLATE = lib
DEFINES += GPUFILTERCORE_LIBRARY

CONFIG += c++11
MOC_DIR += $$PWD/temp
OBJECTS_DIR += $$PWD/temp
DESTDIR += $$PWD/../GPUFilterTestApp/bin

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    3DExtras/GPUFilterGeometryCubeBox.cpp \
    3DExtras/GPUFilterGeometryRect.cpp \
    GPUConverCore/GPUConverScene.cpp \
    GPUConverCore/GPURenderWidget.cpp \
    OpenGLCore/GPUFilterCamera.cpp \
    OpenGLCore/GPUFilterDirectionLight.cpp \
    OpenGLCore/GPUFilterFBO.cpp \
    OpenGLCore/GPUFilterFlashLight.cpp \
    OpenGLCore/GPUFilterLightBase.cpp \
    OpenGLCore/GPUFilterMaterial.cpp \
    OpenGLCore/GPUFilterMesh.cpp \
    OpenGLCore/GPUFilterNodeBase.cpp \
    OpenGLCore/GPUFilterPointLight.cpp \
    OpenGLCore/GPUFilterPostProcessScene.cpp \
    OpenGLCore/GPUFilterRectMesh.cpp \
    OpenGLCore/GPUFilterScene.cpp \
    OpenGLCore/GPUFilterShaderProgram.cpp \
    OpenGLCore/GPUFilterSpotLight.cpp \
    OpenGLCore/GPUFilterTexture.cpp \
    OpenGLCore/GPUFilterTool.cpp \
    gpufiltercore.cpp

HEADERS += \
    3DExtras/GPUFilterGeometryCubeBox.h \
    3DExtras/GPUFilterGeometryRect.h \
    GPUConverCore/GPUConverScene.h \
    GPUConverCore/GPURenderWidget.h \
    GPUFilterCore_global.h \
    OpenGLCore/GPUFilterCamera.h \
    OpenGLCore/GPUFilterDirectionLight.h \
    OpenGLCore/GPUFilterFBO.h \
    OpenGLCore/GPUFilterFlashLight.h \
    OpenGLCore/GPUFilterLightBase.h \
    OpenGLCore/GPUFilterMaterial.h \
    OpenGLCore/GPUFilterMesh.h \
    OpenGLCore/GPUFilterNodeBase.h \
    OpenGLCore/GPUFilterPointLight.h \
    OpenGLCore/GPUFilterPostProcessScene.h \
    OpenGLCore/GPUFilterRectMesh.h \
    OpenGLCore/GPUFilterScene.h \
    OpenGLCore/GPUFilterShaderProgram.h \
    OpenGLCore/GPUFilterSpotLight.h \
    OpenGLCore/GPUFilterTexture.h \
    OpenGLCore/GPUFilterTool.h \
    gpufiltercore.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    shader.qrc
