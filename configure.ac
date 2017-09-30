#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
AC_INIT([CuddlyGL], [0.1], [], [cuddly-gl], [])
AC_CONFIG_SRCDIR([rect.h])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([-Wall foreign])

AC_GNU_SOURCE
AC_SYS_LARGEFILE

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL
m4_ifdef([AM_PROG_AR], [AM_PROG_AR])

AC_LANG(C++)

# Libtool init
LT_INIT([pic-only])
LT_LANG([C++])
LT_SYS_MODULE_EXT

# Set the host type for OSX-specific flags
AC_CANONICAL_HOST

# Check for c++11 language support
AX_CXX_COMPILE_STDCXX_11

# Arguments
AC_ARG_ENABLE([debug],
              AS_HELP_STRING([--enable-debug], [Build binaries with debugging symbols]),
              [],
              [enable_debug="no"])
AC_ARG_ENABLE([profile],
              AS_HELP_STRING([--enable-profile], [Build binaries with profiling support]),
              [],
              [enable_profile="no"])

# Development flags (debugging, profiling)
if test "$enable_debug" == "yes"
then
  AC_MSG_NOTICE([building with debug enabled])
  DEBUG_CXXFLAGS="-g"
  DEBUG_LDFLAGS="-g"
fi

if test "$enable_profile" == "yes"
then
  if test "$CXX" == "clang++"
  then
    AC_SEARCH_LIBS([llvm_start_basic_block_tracing], [profile_rt],
        [
          if test "x$ac_cv_search_llvm_start_basic_block_tracing" == "xnone required"
          then
            AC_MSG_NOTICE([libprofile_rt not needed])
          else
            PROFILE_LDLIBS="$ac_cv_search_llvm_start_basic_block_tracing"
          fi
          PROFILE_CXXFLAGS="-fprofile-arcs -ftest-coverage"
          PROFILE_LDFLAGS="--coverage"
          AC_MSG_NOTICE([building with profiling enabled])
        ],
        [AC_MSG_NOTICE([no libprofile_rt available, not enabling profiling])])
  else
    PROFILE_CXXFLAGS="-fprofile-arcs -ftest-coverage"
    PROFILE_LDFLAGS="--coverage"
    PROFILE_LDLIBS="-lgcov"
    AC_MSG_NOTICE([building with profiling enabled])
  fi
fi

# OpenGL libs
case $host_os in
  darwin*)
    LDLIBS="$LDLIBS -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo"
    ;;
  *)
    AC_SEARCH_LIBS([glBindVertexArray], [GL],
        [LDLIBS="$ac_cv_search_glBindVertexArray $LDLIBS"],
        [AC_MSG_ERROR([libGL not valid, can not build library.])],
        [$LDFLAGS])

    AC_SEARCH_LIBS([gluPerspective], [GLU],
        [LDLIBS="$ac_cv_search_gluPerspective $LDLIBS"],
        [AC_MSG_ERROR([libGLU not valid, can not build library.])],
        [$LDFLAGS])
    ;;
esac

# Check for Freetype2
FREETYPE_CPPFLAGS_SAVE="$CPPFLAGS"
CPPFLAGS="$CXXFLAGS"
AC_CHECK_HEADERS([ft2build.h], [], [AC_MSG_ERROR([no valid Freetype2 headers found, can not continue])])
CPPFLAGS="$FREETYPE_CPPFLAGS_SAVE"
AC_SEARCH_LIBS([FT_Init_FreeType], [freetype],
    [
        FREETYPE_LDLIBS="$ac_cv_search_FT_Init_FreeType"
        LDLIBS="$FREETYPE_LDLIBS $LDLIBS"
    ],
    [AC_MSG_ERROR([ not valid, can not build library.])],
    [$LDFLAGS])

CXXFLAGS="$DEBUG_CXXFLAGS $PROFILE_CXXFLAGS $CXXFLAGS"
LDFLAGS="$DEBUG_LDFLAGS $PROFILE_LDFLAGS $LDFLAGS"
LDLIBS="$LDLIBS $PROFILE_LDLIBS"

# We use the GLM for our GL-related mathematics
GLM_CPPFLAGS_SAVE="$CPPFLAGS"
CPPFLAGS="$CXXFLAGS"
AC_CHECK_HEADERS([glm/vec3.hpp glm/gtc/matrix_transform.hpp glm/gtc/type_ptr.hpp], [], [AC_MSG_ERROR([no valid GLM installation found, can not continue])])
CPPFLAGS="$GLM_CPPFLAGS_SAVE"
CXXFLAGS="$CXXFLAGS -DGLM_FORCE_RADIANS"

CXXFLAGS="$DEBUG_CXXFLAGS $PROFILE_CXXFLAGS $CXXFLAGS"
LDFLAGS="$DEBUG_LDFLAGS $PROFILE_LDFLAGS $LDFLAGS"
LIBS="$LIBS $PROFILE_LDLIBS"

AC_CHECK_HEADERS([math.h stdarg.h stdlib.h string.h sys/stat.h sys/types.h unistd.h])

AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_UINT32_T

AC_CHECK_FUNCS([memcpy memmove memset])

AC_CONFIG_FILES([Makefile
                 test/Makefile])
AC_CONFIG_FILES([stamp-h], [echo timestamp > stamp-h])
AC_OUTPUT
