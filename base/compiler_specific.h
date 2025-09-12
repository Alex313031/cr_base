// Copyright 2008 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MINI_CHROMIUM_BASE_COMPILER_SPECIFIC_H_
#define MINI_CHROMIUM_BASE_COMPILER_SPECIFIC_H_

#include "build/build_config.h"

// Specify memory alignment for structs, classes, etc.
// Use like:
//   class ALIGNAS(16) MyClass { ... }
//   ALIGNAS(16) int array[4];
#if defined(COMPILER_MSVC)
#define ALIGNAS(byte_alignment) __declspec(align(byte_alignment))
#elif defined(COMPILER_GCC)
#define ALIGNAS(byte_alignment) __attribute__((aligned(byte_alignment)))
#endif

#if defined(COMPILER_MSVC)
#define PRINTF_FORMAT(format_param, dots_param)
#else
#define PRINTF_FORMAT(format_param, dots_param) \
    __attribute__((format(printf, format_param, dots_param)))
#endif

// Sanitizers annotations.
#if defined(__has_attribute)
#if __has_attribute(no_sanitize)
#define NO_SANITIZE(what) __attribute__((no_sanitize(what)))
#endif
#endif
#if !defined(NO_SANITIZE)
#define NO_SANITIZE(what)
#endif

// DISABLE_CFI_ICALL -- Disable Control Flow Integrity indirect call checks.
#if BUILDFLAG(IS_WIN)
// Windows also needs __declspec(guard(nocf)).
#define DISABLE_CFI_ICALL NO_SANITIZE("cfi-icall") __declspec(guard(nocf))
#else
#define DISABLE_CFI_ICALL NO_SANITIZE("cfi-icall")
#endif

// Compiler feature-detection.
// clang.llvm.org/docs/LanguageExtensions.html#has-feature-and-has-extension
#if defined(__has_feature)
#define HAS_FEATURE(FEATURE) __has_feature(FEATURE)
#else
#define HAS_FEATURE(FEATURE) 0
#endif

// A wrapper around `__has_attribute()`, which is similar to the C++20-standard
// `__has_cpp_attribute()`, but tests for support for `__attribute__(())`s.
// Compilers that do not support this (e.g. MSVC) are also assumed not to
// support `__attribute__`, so this is simply mapped to `0` there.
//
// See also:
//   https://clang.llvm.org/docs/LanguageExtensions.html#has-attribute
#if defined(__has_attribute)
 #define HAS_ATTRIBUTE(x) __has_attribute(x)
#else
 #define HAS_ATTRIBUTE(x) 0
#endif // defined(__has_attribute)

// A wrapper around `__has_builtin`, similar to `HAS_ATTRIBUTE()`.
//
// See also:
//   https://clang.llvm.org/docs/LanguageExtensions.html#has-builtin
#if defined(__has_builtin)
 #define HAS_BUILTIN(x) __has_builtin(x)
#else
 #define HAS_BUILTIN(x) 0
#endif // defined(__has_builtin)

// A wrapper around `__has_feature`, similar to `HAS_ATTRIBUTE()`.
//
// See also:
//   https://clang.llvm.org/docs/LanguageExtensions.html#has-feature-and-has-extension
#if defined(__has_feature)
 #define HAS_FEATURE(FEATURE) __has_feature(FEATURE)
#else
 #define HAS_FEATURE(FEATURE) 0
#endif // defined(__has_feature)

// Annotates a function indicating it should not be inlined.
//
// You may also want `noopt` if your goal is to preserve a function call even
// for the most trivial cases; see
// https://stackoverflow.com/questions/54481855/clang-ignoring-attribute-noinline/54482070#54482070.
//
// See also:
//   https://clang.llvm.org/docs/AttributeReference.html#noinline
//
// Usage:
// ```
//   _NOINLINE void Func() {
//     // This body will not be inlined into callers.
//   }
// ```
#ifndef _NOINLINE
 #if __has_cpp_attribute(clang::noinline) // Clang
  #define _NOINLINE [[clang::noinline]]
 #elif __has_cpp_attribute(gnu::noinline) // GCC
  #define _NOINLINE [[gnu::noinline]]
 #elif __has_cpp_attribute(msvc::noinline) // MSVC
  #define _NOINLINE [[msvc::noinline]]
 #else // Dummy
  #define _NOINLINE
 #endif
#endif // _NOINLINE

// Annotates a function indicating it should always be inlined.
//
// See also:
//   https://clang.llvm.org/docs/AttributeReference.html#always-inline-force-inline
//
// Usage:
// ```
//   _INLINE void Func() {
//     // This body will be inlined into callers whenever possible.
//   }
// ```
//
// Since `_INLINE` is performance-oriented but can hamper debugging,
// ignore it in debug mode.
#if defined(NDEBUG)
 #if __has_cpp_attribute(clang::always_inline) // Clang
  #define _INLINE [[clang::always_inline]] inline
 #elif __has_cpp_attribute(gnu::always_inline // GCC
  #define _INLINE [[gnu::always_inline]] inline
 #elif defined(COMPILER_MSVC) // MSVC
  #define _INLINE __forceinline
 #else
  #define _INLINE inline
 #endif
#endif

// Same as above but always inline no matter what.
#if __has_cpp_attribute(clang::always_inline)
#define ALWAYS_INLINE [[clang::always_inline]] inline
#elif __has_cpp_attribute(gnu::always_inline)
#define ALWAYS_INLINE [[gnu::always_inline]] inline
#elif defined(COMPILER_MSVC)
#define ALWAYS_INLINE __forceinline
#else
#define ALWAYS_INLINE inline
#endif

#endif  // MINI_CHROMIUM_BASE_COMPILER_SPECIFIC_H_
