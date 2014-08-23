#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])

AC_INIT([steamparser], [1.0])
AC_CONFIG_SRCDIR([src/stdafx.cpp])
AM_INIT_AUTOMAKE

AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_CHECK_LIB([m], [main])

# Checks for libraries.

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdint.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_UINT64_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_OUTPUT(Makefile src/Makefile)
