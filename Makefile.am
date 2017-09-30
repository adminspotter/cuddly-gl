lib_LTLIBRARIES = libcuddly_gl.la

shaderdir = @datadir@/$(PACKAGE)/shaders

libcuddly_gl_la_SOURCES = ui_defs.h image.h \
	font.cc font.h \
	shader.cc shader.h \
	rect.cc rect.h \
	active.cc active.h \
	widget.cc widget.h \
	label.cc label.h \
	button.cc button.h \
	text_field.cc text_field.h \
	password.cc password.h \
	multi_label.cc multi_label.h \
	ui.cc ui.h \
	quadtree.cc quadtree.h \
	composite.cc composite.h \
	manager.cc manager.h \
	row_column.cc row_column.h \
	pie_menu.cc pie_menu.h
libcuddly_gl_la_CXXFLAGS = $(CXXFLAGS) \
	-DSHADER_SRC_PATH=\"$(shaderdir)\" \
	-DGL_GLEXT_PROTOTYPES
libcuddly_gl_la_LIBADD = $(LDLIBS)

clean-local:
	rm -f *.gcno *.gcda *.gcov
