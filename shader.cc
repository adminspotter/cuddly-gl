#include <string.h>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

#include <GL/gl.h>
#include <GL/glext.h>

#include "../l10n.h"

#include "shader.h"

GLuint load_shader(GLenum type, const std::string& file)
{
    std::string src;
    std::ifstream infile(file, std::ios::in | std::ios::binary);

    if (!infile)
    {
        std::ostringstream s;

        s << _("could not open file ") << file;
        throw std::runtime_error(s.str());
    }
    infile.seekg(0, std::ios::end);
    src.resize(infile.tellg());
    infile.seekg(0, std::ios::beg);
    infile.read(&src[0], src.size());
    infile.close();
    return create_shader(type, src);
}

GLuint create_shader(GLenum type, const std::string& src)
{
    GLint res = GL_FALSE;
    int len = 0;
    std::ostringstream s;
    const char *src_chars = src.c_str();
    GLuint shader = glCreateShader(type);

    s << GLenum_to_string(type) << ": ";

    if (shader == 0)
    {
        s << "could not create shader: " << GLenum_to_string(glGetError());
        throw std::runtime_error(s.str());
    }

    glShaderSource(shader, 1, &src_chars, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
    if (len > 0)
    {
        GLchar message[len + 1];

        memset(message, 0, len + 1);
        glGetShaderInfoLog(shader, len, NULL, message);

        s << message;
    }

    /* Delete the shader and throw an exception on failure */
    glGetShaderiv(shader, GL_COMPILE_STATUS, &res);
    if (res == GL_FALSE)
    {
        glDeleteShader(shader);
        throw std::runtime_error(s.str());
    }

    if (len > 0)
        std::clog << s.str() << std::endl;
    return shader;
}

GLuint create_program(GLuint vert, GLuint geom, GLuint frag, const char *out)
{
    GLenum err;
    GLint res = GL_FALSE;
    int len = 0;
    std::ostringstream s;
    GLuint pgm = glCreateProgram();

    s << "GL program: ";

    if (pgm == 0)
    {
        s << "could not create GL program: " << GLenum_to_string(glGetError());
        throw std::runtime_error(s.str());
    }

    glGetError();
    if (vert)
        glAttachShader(pgm, vert);
    if (geom)
        glAttachShader(pgm, geom);
    if (frag)
        glAttachShader(pgm, frag);
    if ((err = glGetError()) != GL_NO_ERROR)
    {
        s << "Could not attach shaders to the shader program: "
          << GLenum_to_string(err);
        glDeleteProgram(pgm);
        throw std::runtime_error(s.str());
    }

    glBindFragDataLocation(pgm, 0, out);
    glLinkProgram(pgm);

    glGetProgramiv(pgm, GL_INFO_LOG_LENGTH, &len);
    if (len > 0)
    {
        GLchar message[len + 1];

        memset(message, 0, len + 1);
        glGetProgramInfoLog(pgm, len, NULL, message);
        s << message;
    }

    glGetProgramiv(pgm, GL_LINK_STATUS, &res);
    if (res == GL_FALSE)
    {
        glDeleteProgram(pgm);
        throw std::runtime_error(s.str());
    }

    if (len > 0)
        std::clog << s.str() << std::endl;
    return pgm;
}

std::string GLenum_to_string(GLenum e)
{
    std::string s;

    switch (e)
    {
      case GL_NO_ERROR:          s = "GL_NO_ERROR"; break;
      case GL_INVALID_ENUM:      s = "GL_INVALID_ENUM"; break;
      case GL_INVALID_VALUE:     s = "GL_INVALID_VALUE"; break;
      case GL_INVALID_OPERATION: s = "GL_INVALID_OPERATION"; break;
      case GL_STACK_OVERFLOW:    s = "GL_STACK_OVERFLOW"; break;
      case GL_STACK_UNDERFLOW:   s = "GL_STACK_UNDERFLOW"; break;
      case GL_OUT_OF_MEMORY:     s = "GL_OUT_OF_MEMORY"; break;
      case GL_TABLE_TOO_LARGE:   s = "GL_TABLE_TOO_LARGE"; break;
      case GL_VERTEX_SHADER:     s = "GL_VERTEX_SHADER"; break;
      case GL_GEOMETRY_SHADER:   s = "GL_GEOMETRY_SHADER"; break;
      case GL_FRAGMENT_SHADER:   s = "GL_FRAGMENT_SHADER"; break;

      default:
        break;
    }
    return s;
}
