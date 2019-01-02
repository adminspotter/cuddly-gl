/* toggle.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 02 Jan 2019, 09:14:48 tquirk
 *
 * CuddlyGL OpenGL widget toolkit
 * Copyright (C) 2019  Trinity Annabelle Quirk
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
 * This file contains the toggle button object definitions.  We derive
 * from the armable, which will take care of the arm/active states.
 * In this class, we'll add the check box and mark.
 *
 * Things to do
 *
 */

#include "ui_defs.h"
#include "toggle.h"

int ui::toggle::get_state(GLuint t, bool *v) const
{
    if (t == ui::state::checked)
        return this->get_checked_state(v);
    return this->armable::get_state(t, v);
}

void ui::toggle::set_state(GLuint t, bool v)
{
    switch (t)
    {
      case ui::state::checked:  this->set_checked_state(v);      break;
      default:                  this->armable::set_state(t, v);  break;
    }
}

int ui::toggle::get_checked_state(bool *v) const
{
    *v = this->checked;
    return 0;
}

void ui::toggle::set_checked_state(bool v)
{
    if (v == this->checked)
        return;
    if (this->activated == true && this->armed == true)
        this->checked = v;
}

void ui::toggle::init(ui::composite *c)
{
    this->checked = false;
}

ui::toggle::toggle(ui::composite *c)
    : ui::armable::armable(c), ui::active::active(0, 0), ui::rect::rect(0, 0)
{
    this->init(c);
}

ui::toggle::~toggle()
{
}
