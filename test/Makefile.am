TC = fonttest \
	t_active \
	t_rect \
	t_bidi
TS = fonttest \
	t_active \
	t_rect \
	t_bidi

if HAVE_TEST_PREREQS
TC += uitest
if WANT_GUI_TESTS
TS += run-uitest
endif
endif

SUBDIRS = tap++ .

TEST_LOG_DRIVER = env AM_TAP_AWK='$(AWK)' $(SHELL) \
	$(top_srcdir)/build-aux/tap-driver.sh
TESTS = $(TS)

check_PROGRAMS = $(TC)

t_active_SOURCES = t_active.cc
t_active_CXXFLAGS = -Itap++
t_active_LDADD = tap++/libtap++.la ../libcuddly_gl.la

t_rect_SOURCES = t_rect.cc
t_rect_CXXFLAGS = -Itap++
t_rect_LDADD = tap++/libtap++.la ../libcuddly_gl.la

t_bidi_SOURCES = t_bidi.cc
t_bidi_CXXFLAGS = -Itap++
t_bidi_LDADD = tap++/libtap++.la ../libcuddly_gl.la

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
