noinst_PROGRAMS = uitest fonttest

lib_LTLIBRARIES = libr9_client_ui.la

shaderdir = @datadir@/$(PACKAGE)/shaders

uitest_SOURCES = uitest.cc libr9_client_ui.la
uitest_CXXFLAGS = $(CLIENT_CXXFLAGS) $(CLIENT_INCLUDES) \
	-DSTORE_PREFIX=\".\"
uitest_LDFLAGS = $(CLIENT_LDFLAGS)
uitest_LDADD = libr9_client_ui.la $(CLIENT_LDLIBS)

fonttest_SOURCES = fonttest.cc libr9_client_ui.la
fonttest_CXXFLAGS = $(CLIENT_CXXFLAGS) $(CLIENT_INCLUDES) \
	-DSTORE_PREFIX=\".\"
fonttest_LDFLAGS = $(CLIENT_LDFLAGS)
fonttest_LDADD = libr9_client_ui.la $(CLIENT_LDLIBS)

libr9_client_ui_la_SOURCES = ui_defs.h shader.cc image.h \
	ui.cc ui.h \
	quadtree.cc quadtree.h \
	panel.cc panel.h \
	composite.cc composite.h \
	manager.cc manager.h \
	row_column.cc row_column.h \
	font.cc font.h \
	label.cc label.h \
	button.cc button.h \
	text_field.cc text_field.h \
	password.cc password.h \
	popupmenu.cc popupmenu.h
libr9_client_ui_la_CXXFLAGS = $(CLIENT_CXXFLAGS) $(CLIENT_INCLUDES) \
	-DSTORE_PREFIX=\"@datadir@/$(PACKAGE)\" \
	-DSHADER_SRC_PATH=\"$(shaderdir)\" \
	-DGL_GLEXT_PROTOTYPES
libr9_client_ui_la_LIBADD = $(CLIENT_LDLIBS)
