noinst_PROGRAMS = uitest

lib_LTLIBRARIES = libr9_client_ui.la

shaderdir = @datadir@/$(PACKAGE)/shaders

uitest_SOURCES = uitest.cc
uitest_CXXFLAGS = $(CLIENT_CXXFLAGS) $(CLIENT_INCLUDES)
uitest_LDFLAGS = $(CLIENT_LDFLAGS)
uitest_LDADD = libr9_client_ui.la $(CLIENT_LDLIBS)

libr9_client_ui_la_SOURCES = ui_defs.h shader.cc \
	ui.cc ui.h \
	panel.cc panel.h \
	rounded_panel.cc rounded_panel.h
libr9_client_ui_la_CXXFLAGS = $(CLIENT_CXXFLAGS) $(CLIENT_INCLUDES) \
	-DSHADER_SRC_PATH=\"$(shaderdir)\" \
	-DGL_GLEXT_PROTOTYPES
