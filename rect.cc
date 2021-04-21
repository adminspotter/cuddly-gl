/* rect.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 28 Nov 2020, 10:34:35 tquirk
 *
 * CuddlyGL OpenGL widget toolkit
 * Copyright (C) 2020  Trinity Annabelle Quirk
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

#include "rect.h"

int ui::rect::get_size(GLuint t, GLuint *v) const
{
    switch (t)
    {
      case ui::size::width:   *v = this->dim.x;  return 0;
      case ui::size::height:  *v = this->dim.y;  return 0;
      default:                                   return 1;
    }
}

int ui::rect::get_size(GLuint t, glm::ivec2 *v) const
{
    switch (t)
    {
      case ui::size::all:  *v = this->dim;  return 0;
      default:                              return 1;
    }
}

void ui::rect::set_size(GLuint t, GLuint v)
{
    switch (t)
    {
      case ui::size::width:   this->dim.x = v;  break;
      case ui::size::height:  this->dim.y = v;  break;
    }
}

void ui::rect::set_size(GLuint t, const glm::ivec2& v)
{
    switch (t)
    {
      case ui::size::all:  this->dim = v;  break;
    }
}

ui::rect::rect(GLuint w, GLuint h)
    : dim(w, h)
{
}

ui::rect::~rect()
{
}

int ui::rect::get(GLuint e, GLuint t, GLuint *v) const
{
    if (e == ui::element::size)
        return this->get_size(t, v);
    return 1;
}

int ui::rect::get(GLuint e, GLuint t, glm::ivec2 *v) const
{
    if (e == ui::element::size)
        return this->get_size(t, v);
    return 1;
}

void ui::rect::set(GLuint e, GLuint t, int v)
{
    this->set(e, t, (GLuint)v);
}

void ui::rect::set(GLuint e, GLuint t, GLuint v)
{
    if (e == ui::element::size)
        this->set_size(t, v);
}

void ui::rect::set(GLuint e, GLuint t, const glm::ivec2& v)
{
    if (e == ui::element::size)
        this->set_size(t, v);
}
