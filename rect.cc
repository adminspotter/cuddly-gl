/* rect.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 10 Aug 2018, 23:48:28 tquirk
 *
 * CuddlyGL OpenGL widget toolkit
 * Copyright (C) 2018  Trinity Annabelle Quirk
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
 * This file contains the rect object definition.  The rect has a
 * size and the basic get and set methods.
 *
 * Things to do
 *
 */

#include "ui_defs.h"
#include "rect.h"

int ui::rect::get_size(GLuint t, void *v) const
{
    int ret = 0;

    switch (t)
    {
      case ui::size::all:
        *reinterpret_cast<glm::ivec2 *>(v) = this->dim;
        break;
      case ui::size::width:
        *reinterpret_cast<int *>(v) = this->dim.x;
        break;
      case ui::size::height:
        *reinterpret_cast<int *>(v) = this->dim.y;
        break;
      default:
        ret = 1;
        break;
    }
    return ret;
}

void ui::rect::set_size(GLuint t, const void *v)
{
    switch (t)
    {
      case ui::size::all:
        this->dim = *reinterpret_cast<const glm::ivec2 *>(v);
        break;
      case ui::size::width:
        this->dim.x = *reinterpret_cast<const int *>(v);
        break;
      case ui::size::height:
        this->dim.y = *reinterpret_cast<const int *>(v);
        break;
    }
}

ui::rect::rect(GLuint w, GLuint h)
    : dim(w, h)
{
}

ui::rect::~rect()
{
}

int ui::rect::get(GLuint e, GLuint t, void *v) const
{
    int ret = 1;

    if (e == ui::element::size)
        ret = this->get_size(t, v);
    return ret;
}

void ui::rect::set(GLuint e, GLuint t, const void *v)
{
    if (e == ui::element::size)
        this->set_size(t, v);
}
