PROG = t_rect
TC = t_rect
if WANT_TESTS
PROG += uitest fonttest
endif

SUBDIRS = tap++

check_PROGRAMS = $(TC)
noinst_PROGRAMS = $(PROG)

t_rect_SOURCES = t_rect.cc ../rect.h ../rect.cc
t_rect_CXXFLAGS = -Itap++
t_rect_LDADD = tap++/libtap++.la

uitest_SOURCES = uitest.cc ../libcuddly_gl.la
uitest_CXXFLAGS = $(CXXFLAGS) $(GLFW_INCLUDES)
uitest_LDFLAGS = $(LDFLAGS) $(GLFW_LDFLAGS)
uitest_LDADD = ../libcuddly_gl.la $(GLFW_LDLIBS) $(LDLIBS)

fonttest_SOURCES = fonttest.cc ../libcuddly_gl.la
fonttest_CXXFLAGS = $(CXXFLAGS) $(GLFW_INCLUDES)
fonttest_LDFLAGS = $(LDFLAGS) $(GLFW_LDFLAGS)
fonttest_LDADD = ../libcuddly_gl.la $(GLFW_LDLIBS) $(LDLIBS)

install-recursive: SUBDIRS =

clean-local:
	rm -f *.gcno *.gcda *.gcov
