#include <string>

#include <GL/gl.h>

GLuint load_shader(GLenum, const std::string&);
GLuint create_shader(GLenum, const std::string&);
GLuint create_program(GLuint, GLuint, GLuint, const char *);
std::string GLenum_to_string(GLenum);
