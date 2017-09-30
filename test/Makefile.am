noinst_PROGRAMS = uitest fonttest

uitest_SOURCES = uitest.cc ../libcuddly_gl.la
uitest_LDADD = ../libcuddly_gl.la $(LDLIBS)

fonttest_SOURCES = fonttest.cc ../libcuddly_gl.la
fonttest_LDADD = ../libcuddly_gl.la $(LDLIBS)

clean-local:
	rm -f *.gcno *.gcda *.gcov
