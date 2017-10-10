PROG =
TC = fonttest\
	t_active \
	t_rect
if HAVE_TEST_PREREQS
PROG += uitest

XVFB_DISPLAY ?= :99

.PHONY: xvfb

xvfb:
	$(XVFB) $(XVFB_DISPLAY) -reset -terminate -screen 0 1280x1024x24+32 -retro +extension XTEST -r &
	DISPLAY=$(XVFB_DISPLAY)
endif

SUBDIRS = tap++

TEST_LOG_DRIVER = env AM_TAP_AWK='$(AWK)' $(SHELL) \
	$(top_srcdir)/build-aux/tap-driver.sh
TESTS = $(TC)

check_PROGRAMS = $(TC)
noinst_PROGRAMS = $(PROG)

t_active_SOURCES = t_active.cc
t_active_CXXFLAGS = -Itap++
t_active_LDADD = tap++/libtap++.la ../libcuddly_gl.la

t_rect_SOURCES = t_rect.cc
t_rect_CXXFLAGS = -Itap++
t_rect_LDADD = tap++/libtap++.la ../libcuddly_gl.la

uitest_SOURCES = uitest.cc ../connect_glfw.cc
uitest_CXXFLAGS = $(CXXFLAGS) $(GLFW_INCLUDES)
uitest_LDFLAGS = $(LDFLAGS) $(GLFW_LDFLAGS)
uitest_LDADD = ../libcuddly_gl.la $(GLFW_LDLIBS) $(LDLIBS)

fonttest_SOURCES = fonttest.cc
fonttest_CXXFLAGS = $(CXXFLAGS) $(GLFW_INCLUDES)
fonttest_LDFLAGS = $(LDFLAGS) $(GLFW_LDFLAGS)
fonttest_LDADD = ../libcuddly_gl.la $(GLFW_LDLIBS) $(LDLIBS)

install-recursive: SUBDIRS =

clean-local:
	rm -f *.gcno *.gcda *.gcov
