#ifndef IGNIS_PLATFORM_DEFINES_H
#define IGNIS_PLATFORM_DEFINES_H

#if defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)
#  define PLATFORM_WINDOWS
#else
#  define PLATFORM_UNIX
#endif

#endif
