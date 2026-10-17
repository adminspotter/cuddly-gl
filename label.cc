/* label.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *
 * CuddlyGL OpenGL widget toolkit
 * Copyright (C) 2016-2026  Trinity Annabelle Quirk
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
 * This file contains the basic label object definitions.  We derive
 * from the text_label, which will take care of the text handling.
 * In this class, we'll add image handling.  We will handle either
 * text rendering, or image rendering.
 *
 * Things to do
 *
 */

#include "label.h"

/* ARGSUSED */
int ui::label::get_image(GLuint t, ui::image *v) const
{
    *v = this->img;
    return 0;
}

/* ARGSUSED */
void ui::label::set_image(GLuint t, const ui::image& v)
{
    this->str.clear();
    this->img = v;
    this->calculate_widget_size();
}

ui::label::label(ui::composite *c)
    : ui::text_label::text_label(c), ui::active::active(0, 0),
      ui::rect::rect(0, 0)
{
}

ui::label::~label()
{
}

int ui::label::get(GLuint e, GLuint t, ui::image *v) const
{
    if (e == ui::element::image)
        return this->get_image(t, v);
    return 1;
}

void ui::label::set(GLuint e, GLuint t, const ui::image& v)
{
    if (e == ui::element::image)
        this->set_image(t, v);
}
