#pragma once

#include "Compiler.hpp"
#include "OperatingSystem.hpp"

#ifdef NODE_EDITOR_PLATFORM_WINDOWS
#  define NODE_EDITOR_EXPORT __declspec(dllexport)
#  define NODE_EDITOR_IMPORT __declspec(dllimport)
#  define NODE_EDITOR_LOCAL
#elif                                                           \
  NODE_EDITOR_COMPILER_GNU_VERSION_MAJOR >= 4                         || \
  defined (NODE_EDITOR_COMPILER_CLANG)
#  define NODE_EDITOR_EXPORT __attribute__((visibility("default")))
#  define NODE_EDITOR_IMPORT __attribute__((visibility("default")))
#  define NODE_EDITOR_LOCAL  __attribute__((visibility("hidden")))
#else
#  define NODE_EDITOR_EXPORT
#  define NODE_EDITOR_IMPORT
#  define NODE_EDITOR_LOCAL
#endif

#ifdef __cplusplus
#  define NODE_EDITOR_DEMANGLED extern "C"
#else
#  define NODE_EDITOR_DEMANGLED
#endif



