/* text_field.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 17 Jul 2016, 17:58:42 tquirk
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
 * This file contains the text field method definitions for the R9 UI
 * widget set.
 *
 * Things to do
 *
 */

#include "text_field.h"

int ui::text_field::get_cursor_pos(GLuint t, void *v)
{
    *((GLuint *)v) = this->cursor_pos;
    return 1;
}

void ui::text_field::set_cursor_pos(GLuint t, void *v)
{
    GLuint new_v = *((GLuint *)v);

    if (new_v > this->str.size())
        new_v = this->str.size();
    this->cursor_pos = new_v;
}

ui::text_field::text_field(ui::context *c, GLuint w, GLuint h)
    : ui::label::label(c, w, h)
{
    this->cursor_pos = 0;
}

ui::text_field::~text_field()
{
}
