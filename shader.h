/* shader.h                                                -*- C++ -*-
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 13 Mar 2016, 10:34:49 tquirk
 *
 * Revision IX game client
 * Copyright (C) 2016  Trinity Annabelle Quirk
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
 * This file contains the shader handling prototypes.
 *
 * Things to do
 *
 */

#ifndef __INC_R9_SHADER_H__
#define __INC_R9_SHADER_H__

#include <string>

#include <GL/gl.h>

GLuint load_shader(GLenum, const std::string&);
GLuint create_shader(GLenum, const std::string&);
GLuint create_program(GLuint, GLuint, GLuint, const char *);
std::string GLenum_to_string(GLenum);

#endif /* __INC_R9_SHADER_H__ */
