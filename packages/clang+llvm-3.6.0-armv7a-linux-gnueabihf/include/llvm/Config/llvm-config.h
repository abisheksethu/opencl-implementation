/* include/llvm/Config/llvm-config.h.  Generated from llvm-config.h.in by configure.  */
/*===------- llvm/Config/llvm-config.h - llvm configuration -------*- C -*-===*/
/*                                                                            */
/*                     The LLVM Compiler Infrastructure                       */
/*                                                                            */
/* This file is distributed under the University of Illinois Open Source      */
/* License. See LICENSE.TXT for details.                                      */
/*                                                                            */
/*===----------------------------------------------------------------------===*/

/* This file enumerates variables from the LLVM configuration so that they
   can be in exported headers and won't override package specific directives.
   This is a C header that can be included in the llvm-c headers. */

#ifndef LLVM_CONFIG_H
#define LLVM_CONFIG_H

/* Installation directory for binary executables */
#define LLVM_BINDIR "/home/rengolin/devel/llvm/release/final/Phase3/Release/llvmCore-3.6.0-final.install/bin"

/* Time at which LLVM was configured */
#define LLVM_CONFIGTIME "(timestamp not enabled)"

/* Installation directory for data files */
#define LLVM_DATADIR "/home/rengolin/devel/llvm/release/final/Phase3/Release/llvmCore-3.6.0-final.install/share/llvm"

/* Target triple LLVM will generate code for by default */
#define LLVM_DEFAULT_TARGET_TRIPLE "armv7l-unknown-linux-gnueabihf"

/* Installation directory for documentation */
#define LLVM_DOCSDIR "/home/rengolin/devel/llvm/release/final/Phase3/Release/llvmCore-3.6.0-final.install/share/doc/llvm"

/* Define if threads enabled */
#define LLVM_ENABLE_THREADS 1

/* Installation directory for config files */
#define LLVM_ETCDIR "/home/rengolin/devel/llvm/release/final/Phase3/Release/llvmCore-3.6.0-final.install/etc/llvm"

/* Has gcc/MSVC atomic intrinsics */
#define LLVM_HAS_ATOMICS 1

/* Host triple LLVM will be executed on */
#define LLVM_HOST_TRIPLE "armv7l-unknown-linux-gnueabihf"

/* Installation directory for include files */
#define LLVM_INCLUDEDIR "/home/rengolin/devel/llvm/release/final/Phase3/Release/llvmCore-3.6.0-final.install/include"

/* Installation directory for .info files */
#define LLVM_INFODIR "/home/rengolin/devel/llvm/release/final/Phase3/Release/llvmCore-3.6.0-final.install/info"

/* Installation directory for man pages */
#define LLVM_MANDIR "/home/rengolin/devel/llvm/release/final/Phase3/Release/llvmCore-3.6.0-final.install/man"

/* LLVM architecture name for the native architecture, if available */
#define LLVM_NATIVE_ARCH ARM

/* LLVM name for the native AsmParser init function, if available */
#define LLVM_NATIVE_ASMPARSER LLVMInitializeARMAsmParser

/* LLVM name for the native AsmPrinter init function, if available */
#define LLVM_NATIVE_ASMPRINTER LLVMInitializeARMAsmPrinter

/* LLVM name for the native Disassembler init function, if available */
#define LLVM_NATIVE_DISASSEMBLER LLVMInitializeARMDisassembler

/* LLVM name for the native Target init function, if available */
#define LLVM_NATIVE_TARGET LLVMInitializeARMTarget

/* LLVM name for the native TargetInfo init function, if available */
#define LLVM_NATIVE_TARGETINFO LLVMInitializeARMTargetInfo

/* LLVM name for the native target MC init function, if available */
#define LLVM_NATIVE_TARGETMC LLVMInitializeARMTargetMC

/* Define if this is Unixish platform */
#define LLVM_ON_UNIX 1

/* Define if this is Win32ish platform */
/* #undef LLVM_ON_WIN32 */

/* Installation prefix directory */
#define LLVM_PREFIX "/home/rengolin/devel/llvm/release/final/Phase3/Release/llvmCore-3.6.0-final.install"

/* Define if we have the Intel JIT API runtime support library */
#define LLVM_USE_INTEL_JITEVENTS 0

/* Define if we have the oprofile JIT-support library */
#define LLVM_USE_OPROFILE 0

/* Major version of the LLVM API */
#define LLVM_VERSION_MAJOR 3

/* Minor version of the LLVM API */
#define LLVM_VERSION_MINOR 6

/* Patch version of the LLVM API */
#define LLVM_VERSION_PATCH 0

/* LLVM version string */
#define LLVM_VERSION_STRING "3.6.0"

#endif
