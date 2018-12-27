/* armable.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 26 Dec 2018, 22:36:52 tquirk
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
 * This file contains the armable object definitions.  This is an
 * intermediate class between the label and the various buttons.  It
 * adds the concepts of armed and active.
 *
 * Things to do
 *
 */

#include "armable.h"

void ui::armable::init(ui::composite *c)
{
    this->activated = false;
    this->armed = false;
}

ui::armable::armable(ui::composite *c)
    : ui::label::label(c), ui::active::active(0, 0), ui::rect::rect(0, 0)
{
    this->init(c);
}

ui::armable::~armable()
{
}
