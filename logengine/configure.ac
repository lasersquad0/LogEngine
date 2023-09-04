#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.71])
LT_PREREQ([2.4.6])

AC_INIT([LogEngine],[1.03],[lasersquad@gmail.com])
AM_INIT_AUTOMAKE
LT_INIT

AC_CONFIG_FILES([Makefile])

AC_CONFIG_SRCDIR([prj/msvc8/testLogEngine/MFCTestsRunner/MainFrm.h])
#AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h sys/timeb.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([ftime memmove memset strcasecmp strstr])

AC_CONFIG_FILES([src/Makefile test/Makefile])
AC_OUTPUT
