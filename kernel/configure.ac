#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.65])
AC_INIT([manifold-kernel], [1.0])
AM_INIT_AUTOMAKE

AC_CONFIG_SRCDIR([src/manifold.cc])
#AC_CONFIG_HEADERS([config.h])

AC_PROG_CXX

# Checks for programs.

# Checks for command-line options

# enable parallel simulation by default
# modifying mitchelle.. default is serial simulation
#AC_ARG_ENABLE([para_sim],
#    [AS_HELP_STRING([--disable-para-sim],
#        [disable parallel simulation @<:@default: no@:>@])],
#    [para_sim=${enableval}], [para_sim=yes])

#if test "x${para_sim}" = xyes ; then
#    AC_CHECK_PROGS([MPICPP], [mpic++], [mpic++])
#    if test "${MPICPP}" = "mpic++" ; then # para_sim enabled; mpic++ found
#        AC_SUBST(CXX, mpic++)
#    else # para_sim enabled; mpic++ not found
#        AC_MSG_WARN([
#	---------------------------------------------
#	mpic++ not found! Building serial simulation!
#	---------------------------------------------])
#    fi
#else # para_sim disabled
AC_DEFINE(NO_MPI)
AC_MSG_WARN([
             ---------------------------------------------------------
             Parallel simulation disabled! Building serial simulation!
             ---------------------------------------------------------])
#fi

AC_PROG_RANLIB

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdint.h stdlib.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T

# Checks for library functions.

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
