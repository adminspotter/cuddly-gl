# ===========================================================================
#   http://www.gnu.org/software/autoconf-archive/ax_prog_perl_modules.html
#      modified by tquirk
# ===========================================================================
#
# SYNOPSIS
#
#   AX_PROG_PERL_MODULES([MODULES],
#                        [ACTION-IF-WORKING],
#                        [ACTION-IF-INSTALLED],
#                        [ACTION-IF-MISSING])
#
# DESCRIPTION
#
#   Checks to see if the given perl modules are available. If true the shell
#   commands in ACTION-IF-TRUE are executed. If not the shell commands in
#   ACTION-IF-FALSE are run. Note if $PERL is not set (for example by
#   calling AC_CHECK_PROG, or AC_PATH_PROG), AC_CHECK_PROG(PERL, perl, perl)
#   will be run.
#
#   MODULES is a space separated list of module names. To check for a
#   minimum version of a module, append the version number to the module
#   name, separated by an equals sign.
#
#   Example:
#
#     AX_PROG_PERL_MODULES( Text::Wrap Net::LDAP=1.0.3, ,
#                           AC_MSG_WARN(Need some Perl modules)
#
# LICENSE
#
#   Copyright (c) 2009 Dean Povey <povey@wedgetail.com>
#
#   Copying and distribution of this file, with or without modification, are
#   permitted in any medium without royalty provided the copyright notice
#   and this notice are preserved. This file is offered as-is, without any
#   warranty.

#serial 7

AU_ALIAS([AC_PROG_PERL_MODULES], [AX_PROG_PERL_MODULES])
AC_DEFUN([AX_PROG_PERL_MODULES],[dnl

m4_define([ax_perl_modules])
m4_foreach([ax_perl_module], m4_split(m4_normalize([$1])),
	  [
	   m4_append([ax_perl_modules],
		     [']m4_bpatsubst(ax_perl_module,=,[ ])[' ])
          ])

# Make sure we have perl
if test -z "$PERL"; then
  AC_CHECK_PROG(PERL,perl,perl)
fi

if test -z "$PERLDOC"; then
  AC_CHECK_PROG(PERLDOC,perldoc,perldoc)
  # Make sure perldoc doesn't just always fail
  if test -n "$PERLDOC"; then
    $PERLDOC CORE >/dev/null 2>&AS_MESSAGE_LOG_FD
    if test $? -ne 0; then
      AC_MSG_NOTICE($PERLDOC does not seem to work)
      PERLDOC=''
    fi
  fi
fi

ax_perl_modules_count=0
ax_perl_modules_missing=0
ax_perl_modules_failed=0

for ax_perl_module in $ax_perl_modules; do
  ax_perl_modules_count=`expr $ax_perl_modules_count + 1`

  AS_IF([test "x$PERLDOC" != x],
        [
          AC_MSG_CHECKING($ax_perl_module presence)
          $PERLDOC -l $ax_perl_module >&AS_MESSAGE_LOG_FD 2>&1
          AS_IF([test $? -ne 0],
                [
                  AC_MSG_RESULT(no)
                  ax_perl_modules_missing=`expr $ax_perl_modules_missing + 1`
                ],
                [AC_MSG_RESULT(yes)])])

  AS_IF([test "x$PERL" != x],
        [
          AC_MSG_CHECKING($ax_perl_module usability)
          $PERL -e "use $ax_perl_module;" >&AS_MESSAGE_LOG_FD 2>&1
          AS_IF([test $? -ne 0],
                [
                  AC_MSG_RESULT(no)
                  ax_perl_modules_failed=`expr $ax_perl_modules_missing + 1`
                ],
                [AC_MSG_RESULT(yes)])])
done

# Run optional shell commands
AS_IF([test $ax_perl_modules_count -gt 0],
      [AS_IF([test "$ax_perl_modules_failed" -eq 0], [$2],
             [test "$ax_perl_modules_missing" -eq 0], [$3],
             [$4])])
])dnl
