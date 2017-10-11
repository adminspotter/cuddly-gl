/* shader.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 11 Oct 2017, 18:36:33 tquirk
 *
 * CuddlyGL OpenGL widget toolkit
 * Copyright (C) 2017  Trinity Annabelle Quirk
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 *
 * This file contains the function definitions for OpenGL shader and
 * program handling.
 *
 * Things to do
 *
 */

#include <stdlib.h>
#include <string.h>

#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

#include <GL/gl.h>
#include <GL/glext.h>

#include "shader.h"

static std::string default_shader_path = SHADER_SRC_PATH;

std::string shader_path(const std::string& type)
{
    char *env_path = getenv("CUDDLY_SHADER_PATH");
    std::string path;

    if (env_path != NULL)
        path = env_path;
    else
        path = default_shader_path;

    if (path.find_last_of('/') != path.size() - 1)
        path += "/";

    path += "ui_" + type + '.' + GLSL_version() + ".glsl";

    return path;
}

GLuint load_shader(GLenum type, const std::string& shader_type)
{
    std::string src;
    std::string fname = shader_path(shader_type);
    std::ifstream infile(fname, std::ios::in | std::ios::binary);

    if (!infile)
    {
        std::ostringstream s;

        s << "could not open file " << fname;
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
        s << "could not create GL program: "
          << GLenum_to_string(glGetError());
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

std::map<std::string, std::string> GL_to_GLSL_version =
{
    { "2.0", "110" },
    { "2.1", "120" },
    { "3.0", "130" },
    { "3.1", "140" },
    { "3.2", "150" },
    { "3.3", "330" },
    { "4.0", "400" },
    { "4.1", "410" },
    { "4.2", "420" },
    { "4.3", "430" },
    { "4.4", "440" },
    { "4.5", "450" },
    { "4.6", "460" }
};

std::string GLSL_version(void)
{
    GLint major, minor;
    std::ostringstream version;

    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    version << major << '.' << minor;
    return GL_to_GLSL_version[version.str()];
}
