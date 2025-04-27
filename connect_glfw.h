/* connect_glfw.h                                          -*- C++ -*-
 *   by Trinity Quirk <tquirk@ymb.net>
 *
 * CuddlyGL OpenGL widget toolkit
 * Copyright (C) 2017-2019  Trinity Annabelle Quirk
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
 * This file contains function prototypes for connecting the CuddlyGL
 * widget toolkit to the GLFW windowing system.
 *
 * Things to do
 *
 */

#ifndef __INC_CONNECT_GLFW_H__
#define __INC_CONNECT_GLFW_H__

#ifndef GLFW_INCLUDE_GL_3
#define GLFW_INCLUDE_GL_3
#endif
#include <GLFW/glfw3.h>

#include "ui.h"

void ui_connect_glfw(ui::context *, GLFWwindow *);
void ui_disconnect_glfw(ui::context *, GLFWwindow *);

#endif /* __INC_CONNECT_GLFW_H__ */
