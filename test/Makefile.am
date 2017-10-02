PROG =
if WANT_TESTS
PROG += uitest fonttest
endif

check_PROGRAMS = $(PROG)

uitest_SOURCES = uitest.cc ../libcuddly_gl.la
uitest_CXXFLAGS = $(CXXFLAGS) $(GLFW_INCLUDES)
uitest_LDFLAGS = $(LDFLAGS) $(GLFW_LDFLAGS)
uitest_LDADD = ../libcuddly_gl.la $(GLFW_LDLIBS) $(LDLIBS)

fonttest_SOURCES = fonttest.cc ../libcuddly_gl.la
fonttest_CXXFLAGS = $(CXXFLAGS) $(GLFW_INCLUDES)
fonttest_LDFLAGS = $(LDFLAGS) $(GLFW_LDFLAGS)
fonttest_LDADD = ../libcuddly_gl.la $(GLFW_LDLIBS) $(LDLIBS)

clean-local:
	rm -f *.gcno *.gcda *.gcov
