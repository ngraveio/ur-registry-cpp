AC_PREREQ([2.69])
AC_INIT([ur-registry], [0.0.0])
AC_CONFIG_MACRO_DIRS([m4])
AM_INIT_AUTOMAKE([foreign subdir-objects silent-rules])
AC_CONFIG_HEADERS([config.h])

AC_PROG_CXX
AC_PROG_INSTALL

AC_ENABLE_STATIC([yes])
AC_ENABLE_SHARED([yes])
LT_INIT([shared static])

AC_CONFIG_FILES([
  Makefile
  src/Makefile
  tests/Makefile
  ur-registry.pc
])
AC_OUTPUT
