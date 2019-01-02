/* toggle.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 02 Jan 2019, 09:21:36 tquirk
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

/* ARGSUSED */
void ui::toggle::check(ui::active *a, void *call, void *client)
{
    ui::toggle *t = dynamic_cast<ui::toggle *>(a);

    if (t != NULL)
    {
        bool active, armed, checked;

        t->get(ui::element::state, ui::state::active, &active,
               ui::element::state, ui::state::armed, &armed,
               ui::element::state, ui::state::checked, &checked);
        if (active == true && armed == true)
            t->set(ui::element::state, ui::state::checked, !checked);
    }
}

void ui::toggle::init(ui::composite *c)
{
    this->checked = false;

    /* We need to rearrange the callback list a bit, so that we can
     * intercept the active/armed states before the armable might
     * change them.
     */
    this->remove_callback(ui::callback::btn_up, ui::armable::disarm, NULL);
    this->add_callback(ui::callback::btn_up, ui::toggle::check, NULL);
    this->add_callback(ui::callback::btn_up, ui::armable::disarm, NULL);
}

ui::toggle::toggle(ui::composite *c)
    : ui::armable::armable(c), ui::active::active(0, 0), ui::rect::rect(0, 0)
{
    this->init(c);
}

ui::toggle::~toggle()
{
}
