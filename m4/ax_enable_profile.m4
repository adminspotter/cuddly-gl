# Since some compilers can masquerade as gcc, we need to figure out
# the actual compiler type.  We'll use the compiler's version report
# banner where needed to get the real info.
AC_DEFUN([_AX_PROG_COMPILER_TYPE],
         [
AC_MSG_CHECKING([compiler type for profiling])
is_gcc=$($CC --version 2>/dev/null | grep -io gcc)
is_clang=$($CC --version 2>/dev/null | grep -io clang)
AS_IF([test "x$CC" == "xclang" || test "x$is_clang" != "x"],
      [PROFILE_COMPILER_TYPE=clang]
      [test "x$CC" == "xgcc" || test "x$is_gcc" != "x"],
      [PROFILE_COMPILER_TYPE=gcc],
      [PROFILE_COMPILER_TYPE=unknown])
AC_MSG_RESULT([$PROFILE_COMPILER_TYPE])
         ])

# clang requires the libprofile_rt to be installed in order to
# actually do profiling.  It's often packaged separately from the main
# compiler suite.
AC_DEFUN([_AX_PROG_CLANG_PROFILE_RT],
         [
AX_SAVE_FLAGS_WITH_PREFIX([PROFILE], [[LIBS]])
AC_SEARCH_LIBS([llvm_start_basic_block_tracing], [profile_rt],
               [
AS_IF([test "x$ac_cv_search_llvm_start_basic_block_tracing" == "xnone required"],
      [AC_MSG_NOTICE([libprofile_rt not needed])],
      [PROFILE_LDLIBS="$ac_cv_search_llvm_start_basic_block_tracing"])
PROFILE_CFLAGS="-fprofile-arcs -ftest-coverage"
PROFILE_CXXFLAGS="-fprofile-arcs -ftest-coverage"
PROFILE_LDFLAGS="--coverage"
AC_MSG_NOTICE([enabling profiling for a LLVM/clang compiler])
               ],
               [
AC_MSG_NOTICE([no libprofile_rt found, not enabling profiling])
               ])
AX_RESTORE_FLAGS_WITH_PREFIX([PROFILE], [[LIBS]])
         ])

# Regular gcc typically has libgcov installed along with it.
AC_DEFUN([_AX_PROG_GCC_GCOV],
         [
PROFILE_CFLAGS="-fprofile-arcs -ftest-coverage"
PROFILE_CXXFLAGS="-fprofile-arcs -ftest-coverage"
PROFILE_LDFLAGS="--coverage"
PROFILE_LDLIBS="-lgcov"
AC_MSG_NOTICE([enabling profiling for a GNU/gcc compiler])
         ])

# Check the compiler type, and set the build flags to match.  Flags
# that may be set are:
#   PROFILE_CFLAGS
#   PROFILE_CXXFLAGS
#   PROFILE_LDFLAGS
#   PROFILE_LDLIBS
AC_DEFUN([AX_ENABLE_PROFILE],
         [
           _AX_PROG_COMPILER_TYPE
           AS_CASE([$PROFILE_COMPILER_TYPE],
                   [clang], [_AX_PROG_CLANG_PROFILE_RT],
                   [gcc], [_AX_PROG_GCC_GCOV],
                   [AC_MSG_NOTICE([Unknown compiler type, not enabling profiling])])
         ])
