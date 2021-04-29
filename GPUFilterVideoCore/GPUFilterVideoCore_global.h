#ifndef GPUFILTERVIDEOCORE_GLOBAL_H
#define GPUFILTERVIDEOCORE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(GPUFILTERVIDEOCORE_LIBRARY)
#  define GPUFILTERVIDEOCORE_EXPORT Q_DECL_EXPORT
#else
#  define GPUFILTERVIDEOCORE_EXPORT Q_DECL_IMPORT
#endif

#endif // GPUFILTERVIDEOCORE_GLOBAL_H
