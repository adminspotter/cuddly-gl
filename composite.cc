/* composite.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 08 Aug 2016, 22:45:09 tquirk
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
 * This file contains the method definitions for the composite object.
 * It's a panel which can have children, which will then be
 * subclassable into more interesting manager widgets.
 *
 * Things to do
 *
 */

#include "composite.h"

ui::composite::composite(context *c, GLuint w, GLuint h)
    : ui::panel::panel(c, w, h)
{
}

ui::composite::~composite()
{
}

int ui::composite::get(GLuint e, GLuint t, void *v)
{
}

void ui::composite::set(GLuint e, GLuint t, void *v)
{
}

void ui::composite::draw(void)
{
}
