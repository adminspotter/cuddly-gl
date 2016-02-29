/* panel.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 28 Feb 2016, 11:36:37 tquirk
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
 * This file contains the basic panel object method definitions.
 *
 * Things to do
 *
 */

#include <stdarg.h>

#include <string>
#include <stdexcept>

#include "../l10n.h"

#include "panel.h"

void panel::set_size(GLuint w, GLuint h)
{
    if (this->margin[0] + this->border[0]
        + this->border[3] + this->margin[3] <= h)
        this->height = h;
    if (this->margin[1] + this->border[1]
        + this->border[2] + this->margin[2] <= w)
        this->width = w;
}

void panel::set_border(GLuint s, GLuint v)
{
    if (s & ui::side::top & ui::side::bottom)
        if (this->margin[0] + this->margin[3]
            + (s & ui::side::top ? v : this->border[0])
            + (s & ui::side::bottom ? v : this->border[3]) <= this->height)
        {
            if (s & ui::side::top)     this->border[0] = v;
            if (s & ui::side::bottom)  this->border[3] = v;
        }

    if (s & ui::side::left & ui::side::right)
        if (this->margin[1] + this->margin[2]
            + (s & ui::side::left ? v : this->border[1])
            + (s & ui::side::right ? v : this->border[2]) <= this->width)
        {
            if (s & ui::side::left)    this->border[1] = v;
            if (s & ui::side::right)   this->border[2] = v;
        }
}

void panel::set_margin(GLuint s, GLuint v)
{
    if (s & ui::side::top & ui::side::bottom)
        if (this->border[0] + this->border[3]
            + (s & ui::side::top ? v : this->margin[0])
            + (s & ui::side::bottom ? v : this->margin[3]) <= this->height)
        {
            if (s & ui::side::top)     this->margin[0] = v;
            if (s & ui::side::bottom)  this->margin[3] = v;
        }

    if (s & ui::side::left & ui::side::right)
        if (this->border[1] + this->border[2]
            + (s & ui::side::left ? v : this->margin[1])
            + (s & ui::side::right ? v : this->margin[2]) <= this->width)
        {
            if (s & ui::side::left)    this->margin[1] = v;
            if (s & ui::side::right)   this->margin[2] = v;
        }
}

void panel::set_radius(GLuint c, GLuint v)
{
    if (v <= min(this->width / 2, this->height / 2))
    {
        if (c & ui::corner::top_left)     this->radius[0] = v;
        if (c & ui::corner::top_right)    this->radius[1] = v;
        if (c & ui::corner::bottom_left)  this->radius[2] = v;
        if (c & ui::corner::bottom_right) this->radius[3] = v;
    }
}

panel::panel(GLuint w, GLuint h)
{
    this->set_size(w, h);
    this->set_border(ui::side::all, 0);
    this->set_margin(ui::side::all, 0);
    this->set_radius(ui::corner::all, 0);
}

void panel::set(GLuint s, GLuint m, GLuint v)
{
    switch (s)
    {
      case ui::element::size:   this->set_size(m, v);   break;
      case ui::element::border: this->set_border(m, v); break;
      case ui::element::margin: this->set_margin(m, v); break;
      case ui::element::radius: this->set_radius(m, v); break;
    }
}

void panel::set_va(GLuint s, GLuint m, GLuint v, ...)
{
    va_list args;
    GLuint item[3];

    this->set(s, m, v);
    va_start(args, v);
    if ((item[0] = va_arg(args, GLuint)) != -1)
    {
        if ((item[1] = va_arg(args, GLuint) == -1)
            || (item[2] = va_arg(args, GLuint)) == -1)
        {
            std::string s = _("bad argument count in set_va");
            throw std::runtime_error(s);
        }
        this->set(item[0], item[1], item[2]);
    }
    va_end(args);
}
