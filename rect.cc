/* rect.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 07 Oct 2016, 08:04:46 tquirk
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
 * This file contains the rect object definition.  The rect has a
 * size and the basic get and set methods.
 *
 * Things to do
 *
 */

#include "rect.h"

int ui::rect::get_size(GLuint t, void *v)
{
    int ret = 0;

    switch (t)
    {
      case ui::size::all:    *(glm::ivec2 *)v = this->dim;  break;
      case ui::size::width:  *(int *)v = this->dim.x;       break;
      case ui::size::height: *(int *)v = this->dim.y;       break;
      default:               ret = 1;                       break;
    }
    return ret;
}

void ui::rect::set_size(GLuint t, void *v)
{
    switch (t)
    {
      case ui::size::all:     this->dim = *(glm::ivec2 *)v;  break;
      case ui::size::width:   this->dim.x = *(int *)v;       break;
      case ui::size::height:  this->dim.y = *(int *)v;       break;
    }
}

ui::rect::rect(GLuint w, GLuint h)
    : dim(w, h)
{
}

ui::rect::~rect()
{
}

int ui::rect::get(GLuint e, GLuint t, void *v)
{
    int ret = 1;

    if (e == ui::element::size)
        ret = this->get_size(t, v);
    return ret;
}

void ui::rect::set(GLuint e, GLuint t, void *v)
{
    if (e == ui::element::size)
        this->set_size(t, v);
}

void ui::rect::get_va(GLuint e, GLuint t, void *v, ...)
{
    va_list args;
    GLuint item[2];
    void *ptr;

    this->get(e, t, v);
    va_start(args, v);
    while ((item[0] = va_arg(args, GLuint)) != 0)
    {
        item[1] = va_arg(args, GLuint);
        ptr = va_arg(args, void *);
        this->get(item[0], item[1], ptr);
    }
    va_end(args);
}

void ui::rect::set_va(GLuint e, GLuint t, void *v, ...)
{
    va_list args;
    GLuint item[2];
    void *ptr;

    this->set(e, t, v);
    va_start(args, v);
    while ((item[0] = va_arg(args, GLuint)) != 0)
    {
        item[1] = va_arg(args, GLuint);
        ptr = va_arg(args, void *);
        this->set(item[0], item[1], ptr);
    }
    va_end(args);
}
