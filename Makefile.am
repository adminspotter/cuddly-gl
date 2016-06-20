noinst_PROGRAMS = uitest fonttest

lib_LTLIBRARIES = libr9_client_ui.la

shaderdir = @datadir@/$(PACKAGE)/shaders

uitest_SOURCES = uitest.cc
uitest_CXXFLAGS = $(CLIENT_CXXFLAGS) $(CLIENT_INCLUDES) \
	-DSTORE_PREFIX=\".\"
uitest_LDFLAGS = $(CLIENT_LDFLAGS)
uitest_LDADD = libr9_client_ui.la $(CLIENT_LDLIBS)

fonttest_SOURCES = fonttest.cc
fonttest_CXXFLAGS = $(CLIENT_CXXFLAGS) $(CLIENT_INCLUDES) \
	-DSTORE_PREFIX=\".\"
fonttest_LDFLAGS = $(CLIENT_LDFLAGS)
fonttest_LDADD = libr9_client_ui.la $(CLIENT_LDLIBS)

libr9_client_ui_la_SOURCES = ui_defs.h shader.cc \
	ui.cc ui.h \
	panel.cc panel.h \
	font.cc font.h \
	label.cc label.h
libr9_client_ui_la_CXXFLAGS = $(CLIENT_CXXFLAGS) $(CLIENT_INCLUDES) \
	-DSTORE_PREFIX=\"@datadir@/$(PACKAGE)\" \
	-DSHADER_SRC_PATH=\"$(shaderdir)\" \
	-DGL_GLEXT_PROTOTYPES
