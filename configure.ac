#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
AC_INIT([CuddlyGL], [0.1], [], [cuddly-gl], [])
AC_CONFIG_SRCDIR([rect.h])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([build-aux])

AM_INIT_AUTOMAKE([-Wall foreign subdir-objects])

AC_GNU_SOURCE
AC_SYS_LARGEFILE

# Default to empty CFLAGS/CXXFLAGS
: ${CFLAGS=""}
: ${CXXFLAGS=""}

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
AC_ARG_ENABLE([gui-tests],
              AS_HELP_STRING([--disable-gui-tests], [Don't perform tests with X11 driver]))

AC_ARG_WITH([freetype],
            AS_HELP_STRING([--with-freetype=<path>], [Freetype2 install root]),
            [],
            [with_freetype="no"])
AC_ARG_WITH([freetype-includes],
            AS_HELP_STRING([--with-freetype-includes=<path>], [Freetype2 include path]),
            [],
            [with_freetype_includes="no"])
AC_ARG_WITH([freetype-libraries],
            AS_HELP_STRING([--with-freetype-libraries=<path>], [Freetype2 library path]),
            [],
            [with_freetype_libraries="no"])

AC_ARG_WITH([glm-includes],
            AS_HELP_STRING([--with-glm-includes=<path>], [GLM include path]))

AC_ARG_WITH([glfw],
            AS_HELP_STRING([--without-glfw], [Disable GLFW for test suite]))
AC_ARG_WITH([glfw-includes],
            AS_HELP_STRING([--with-glfw-includes=<path>], [Test suite includes]),
            [with_glfw="yes"],
            [with_glfw_includes="no"])
AC_ARG_WITH([glfw-libraries],
            AS_HELP_STRING([--with-glfw-libraries=<path>], [Test suite libraries]),
            [with_glfw="yes"],
            [with_glfw_libraries="no"])

AC_ARG_WITH([manpages],
            AS_HELP_STRING([--without-manpages], [Disable manpage generation]))
AC_ARG_WITH([ronn],
            AS_HELP_STRING([--with-ronn], [Path of the ronn program]))

# Development flags (debugging, profiling)
AS_IF([test "$enable_profile" == "yes"],
      [AX_ENABLE_PROFILE])

AS_IF([test "$enable_debug" == "yes" || test "x$PROFILE_CFLAGS" != "x"],
      [
        AC_MSG_NOTICE([building with debug enabled])
        DEBUG_CXXFLAGS="-g -O0"
        DEBUG_LDFLAGS="-g -O0"
      ])

# OpenGL libs
AS_CASE([$host_os],
        [darwin*],
        [
    LDLIBS="$LDLIBS -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo"
    CXXFLAGS="$CXXFLAGS -I/opt/X11/include"
        ],
        [
    AC_SEARCH_LIBS([XAutoRepeatOff], [X11],
        [],
        [AC_MSG_ERROR([libX11 not valid, can not build library.])],
        [$LDFLAGS])

    AC_SEARCH_LIBS([glBindVertexArray], [GL],
        [],
        [AC_MSG_ERROR([libGL not valid, can not build library.])],
        [$LDFLAGS])

    AC_SEARCH_LIBS([gluPerspective], [GLU],
        [],
        [AC_MSG_ERROR([libGLU not valid, can not build library.])],
        [$LDFLAGS])

    X11_DEFS="-DUSING_X11"
    AC_SUBST(X11_DEFS)
        ])

# Check for Freetype2
FREETYPE_INCLUDES=""
FREETYPE_LDFLAGS=""
FREETYPE_LDLIBS=""
AX_SAVE_FLAGS_WITH_PREFIX([FREETYPE],
                          [[CPPFLAGS],[CXXFLAGS],[LDFLAGS],[LIBS]])
AS_IF([test "x$with_freetype_includes" != "xno"],
      [FREETYPE_INCLUDES="-I$with_freetype_includes"],
      [test "x$with_freetype" != "xno"],
      [FREETYPE_INCLUDES="-I$with_freetype/include"])
CXXFLAGS="$CXXFLAGS $FREETYPE_INCLUDES"
CPPFLAGS="$CXXFLAGS"
AC_CHECK_HEADERS([ft2build.h], [], [AC_MSG_ERROR([no valid Freetype2 headers found, can not continue])])
# We will only report the results for the following, because there is
# already a header file which has an appropriate define in it.
AC_MSG_CHECKING([if Freetype2 is built with bitmap support])
AC_PREPROC_IFELSE(
    [AC_LANG_PROGRAM(
        [
#include <ft2build.h>
#include FT_CONFIG_OPTIONS_H
#ifndef TT_CONFIG_OPTION_EMBEDDED_BITMAPS
#error "no bitmap support"
#endif
        ],
        [])
    ],
    [AC_MSG_RESULT([yes])],
    [AC_MSG_RESULT([no])])
AS_IF([test "x$with_freetype_libraries" != "xno"],
      [FREETYPE_LDFLAGS="-L$with_freetype_libraries"],
      [test "x$with_freetype" != "xno"],
      [FREETYPE_LDFLAGS="-L$with_freetype/lib"])
LDFLAGS="$LDFLAGS $FREETYPE_LDFLAGS"
AC_SEARCH_LIBS([FT_Init_FreeType], [freetype],
    [FREETYPE_LDLIBS="$ac_cv_search_FT_Init_FreeType"],
    [AC_MSG_ERROR([ not valid, can not build library.])],
    [$LDFLAGS])
AX_RESTORE_FLAGS_WITH_PREFIX([FREETYPE],
                             [[CPPFLAGS],[CXXFLAGS],[LDFLAGS],[LIBS]])

# We use the GLM for our GL-related mathematics
AX_SAVE_FLAGS_WITH_PREFIX([GLM], [[CPPFLAGS],[CXXFLAGS]])
GLM_INCLUDES="-DGLM_FORCE_RADIANS"
AS_IF([test "x$with_glm_includes" != "xno" && test "x$with_glm_includes" != "x"],
      [GLM_INCLUDES="-I$with_glm_includes $GLM_INCLUDES"])
CXXFLAGS="$CXXFLAGS $GLM_INCLUDES"
CPPFLAGS="$CXXFLAGS"
AC_CHECK_HEADERS([glm/vec3.hpp glm/gtc/matrix_transform.hpp glm/gtc/type_ptr.hpp], [], [AC_MSG_ERROR([no valid GLM installation found, can not continue])])
AX_RESTORE_FLAGS_WITH_PREFIX([GLM], [[CPPFLAGS],[CXXFLAGS]])

# Perl module for the test suite
AX_PROG_PERL_MODULES([Test::More],
                     [have_test_more="yes"],
                     [have_test_more="no"])

# GLSL validation
AC_CHECK_PROG([HAVE_GLSLANGVALIDATOR], [glslangValidator], [yes], [no])
AM_CONDITIONAL([VALIDATE_SHADERS], [test "x$have_test_more" = "xyes" && test "x$HAVE_GLSLANGVALIDATOR" = "xyes"])

# GLFW for the test suite
can_do_tests="no"
GLFW_INCLUDES=""
GLFW_LDFLAGS=""
GLFW_LDLIBS=""
AS_IF([test "x$with_glfw" != "xno"],
      [
        AX_SAVE_FLAGS_WITH_PREFIX([GLFW],
                                  [[CPPFLAGS],[CXXFLAGS],[LDFLAGS],[LIBS]])
        AS_IF([test "x$with_glfw_includes" != "xno"],
              [GLFW_INCLUDES="-I$with_glfw_includes"],
              [test "x$with_glfw" != "xyes"],
              [GLFW_INCLUDES="-I$with_glfw/include"])
        CXXFLAGS="$CXXFLAGS $GLFW_INCLUDES"
        CPPFLAGS="$CXXFLAGS"
        AC_CHECK_HEADERS([GLFW/glfw3.h],
                         [can_do_tests="yes"],
                         [AC_MSG_NOTICE([no GLFW headers found, not building GUI test suite])])

        AS_IF([test "x$can_do_tests" == "xyes"],
              [
                AS_IF([test "x$with_glfw_libraries" != "xno"],
                      [GLFW_LDFLAGS="-L$with_glfw_libraries"],
                      [test "x$with_glfw" != "xyes"],
                      [GLFW_LDFLAGS="-L$with_glfw/lib"])
                LDFLAGS="$LDFLAGS $GLFW_LDFLAGS"
                AC_SEARCH_LIBS([glfwInit], [glfw],
                               [GLFW_LDLIBS="$ac_cv_search_glfwInit"],
                               [
                                 AC_MSG_NOTICE([no GLFW libraries found, not building GUI test suite])
                                 can_do_tests="no"
                               ])])
        AX_RESTORE_FLAGS_WITH_PREFIX([GLFW],
                                     [[CPPFLAGS],[CXXFLAGS],[LDFLAGS],[LIBS]])
        AS_IF([test "x$enable_gui_tests" != "xno" && test "x$have_test_more" = "xyes"],
              [AX_PROG_PERL_MODULES([X11::GUITest],
                                    [AC_MSG_NOTICE([enabling GUI test suite])],
                                    [AC_MSG_NOTICE([X11::GUITest is installed enough for our purposes])],
                                    [
                                      AC_MSG_NOTICE([missing perl modules, disabling GUI test suite])
                                      enable_gui_tests="no"
                                    ])
              ],
              [AS_IF([test "x$can_do_tests" == "xyes"],
                     [AC_MSG_NOTICE([disabling GUI test suite by option])])
              ]
        )
      ])
AM_CONDITIONAL([HAVE_TEST_PREREQS], [test "x$can_do_tests" == "xyes"])
AM_CONDITIONAL([WANT_GUI_TESTS], [test "x$enable_gui_tests" != "xno"])
AS_IF([test "x$can_do_tests" == "xyes"],
      [AS_CASE([$host_os],
               [darwin*],
               [AC_DEFINE(NEED_GL_FORWARD_COMPAT, [1], [MacOS needs the OpenGL Forward Compatibility bit set])])])
AC_SUBST(GLFW_INCLUDES)
AC_SUBST(GLFW_LDFLAGS)
AC_SUBST(GLFW_LDLIBS)

PTHREAD_LIBS_SAVE="$LIBS"
LIBS=""
AC_SEARCH_LIBS([pthread_create], [pthread],
    [test "$ac_cv_search_pthread_create" = "none required" || PTHREAD_LIBS="$ac_cv_search_pthread_create"],
    [AC_MSG_ERROR([can not build without pthreads.])])
LIBS="$PTHREAD_LIBS_SAVE"

CXXFLAGS="$DEBUG_CXXFLAGS $PROFILE_CXXFLAGS $GLM_INCLUDES $FREETYPE_INCLUDES $CXXFLAGS"
LDFLAGS="$DEBUG_LDFLAGS $PROFILE_LDFLAGS $FREETYPE_LDFLAGS $LDFLAGS"
LIBS="$LIBS $LDLIBS $FREETYPE_LDLIBS $PTHREAD_LIBS $PROFILE_LDLIBS"
AC_MSG_NOTICE([Using CXXFLAGS $CXXFLAGS])
AC_MSG_NOTICE([Using LDFLAGS $LDFLAGS])
AC_MSG_NOTICE([Using LIBS $LIBS])

AC_CHECK_HEADERS([math.h stdarg.h stdlib.h string.h sys/stat.h sys/types.h unistd.h])

AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_UINT32_T

AC_CHECK_FUNCS([memcpy memmove memset strerror_r])

AS_IF([test "x$with_manpages" != "xno"],
      [
        AC_MSG_CHECKING([for a new enough version of ronn])
        AS_IF([test "x$with_ronn" != "x"],
              [ronn_path=$with_ronn],
              [ronn_path="ronn"])
        ronn_version=$($ronn_path --version | sed -ne '1{s/.*v//;p;}')
        AX_COMPARE_VERSION([$ronn_version], [ge], [0.7.5],
                           [
                             AC_MSG_RESULT([yes, $ronn_path])
                             AC_SUBST([RONN], [$ronn_path])
                           ],
                           [
                             AC_MSG_RESULT([no, disabling manpage generation])
                             with_manpages="no"
                           ])
      ],
      [AC_MSG_NOTICE([Manpage generation disabled by configure option])])
AM_CONDITIONAL([GENERATE_MANPAGES], [test "x$with_manpages" == "xyes"])

AC_CONFIG_FILES([Makefile
                 test/Makefile
                 test/tap++/Makefile
                 shaders/Makefile
                 doc/Makefile])
AC_CONFIG_FILES([stamp-h], [echo timestamp > stamp-h])
AC_REQUIRE_AUX_FILE([tap-driver.sh])
AC_OUTPUT
