ACLOCAL_AMFLAGS = -I m4

SD = . test doc
SD_INST = . shaders doc

SUBDIRS = $(SD)

install-recursive: SUBDIRS = $(SD_INST)

EXTRA_DIST = autogen.sh doc

pkginclude_HEADERS = active.h \
	armable.h \
	bidi.h \
	button.h \
	composite.h \
	font.h\
	image.h \
	label.h \
	manager.h \
	password.h \
	pie_menu.h \
	quadtree.h \
	rect.h \
	row_column.h \
	text_field.h \
	toggle.h \
	ui.h \
	ui_defs.h \
	util.h \
	widget.h
lib_LTLIBRARIES = libcuddly_gl.la

shaderdir = @datadir@/$(PACKAGE)/shaders
connectordir = @datadir@/$(PACKAGE)/connectors

connector_DATA = connect_glfw.cc connect_glfw.h

libcuddly_gl_la_SOURCES = active.cc \
	armable.cc \
	bidi.cc \
	bidi_classes.cc \
	button.cc \
	composite.cc \
	font.cc \
	label.cc \
	manager.cc \
	password.cc \
	pie_menu.cc \
	quadtree.cc \
	rect.cc \
	row_column.cc \
	shader.cc \
	text_field.cc \
	toggle.cc \
	ui.cc \
	util.cc \
	widget.cc
libcuddly_gl_la_CXXFLAGS = $(CXXFLAGS) \
	-DSHADER_SRC_PATH=\"$(shaderdir)\" \
	-DGL_GLEXT_PROTOTYPES
libcuddly_gl_la_LDFLAGS = $(LDFLAGS)
libcuddly_gl_la_LIBADD = $(LDLIBS)

clean-local:
	rm -f *.gcno *.gcda *.gcov
