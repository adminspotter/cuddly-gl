/* multi_label.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 21 Sep 2016, 19:01:24 tquirk
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
 * This file contains the multi-line label object definitions.  We
 * derive from the label, which will give us a basic string rendering
 * framework.  In this class, we'll add calculation of line breaks and
 * a desired dimension, not unlike the row-column manager widget.
 *
 * Things to do
 *
 */

#include "multi_label.h"

void ui::multi_label::set_image(GLuint t, void *v)
{
    /* Don't do anything; this doesn't make sense in this widget. */
}

/* We need to be able to break on any arbitrary space when doing
 * multiline strings, so let's get a list of all the valid whitespace
 * that we can break on, in one place.  Found a nice table at
 * https://www.cs.tut.fi/~jkorpela/chars/spaces.html
 */
bool ui::multi_label::is_whitespace(uint32_t c)
{
    if ((c >= 0x000a && c <= 0x000d)
        || c == 0x0020 || c == 0x1680 || c == 0x180e
        || (c >= 0x2000 && c <= 0x200b)
        || c == 0x205f || c == 0x3000)
        return true;
    return false;
}

ui::multi_label::multi_label(ui::composite *p, GLuint w, GLuint h)
    : ui::label::label(p, w, h)
{
}

ui::multi_label::~multi_label()
{
}
