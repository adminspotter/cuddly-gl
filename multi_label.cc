/* multi_label.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 07 Sep 2016, 08:18:33 tquirk
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

ui::multi_label::multi_label(ui::composite *p, GLuint w, GLuint h)
    : ui::label::label(p, w, h)
{
}

ui::multi_label::~multi_label()
{
}
