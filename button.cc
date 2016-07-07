/* button.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 07 Jul 2016, 08:04:03 tquirk
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
 * This file contains the basic button object definitions.  We derive
 * from the label, which will take care of the box part.  In this
 * class, we'll add some static activate/deactivate functions which
 * show when the button is active.  The click action will still be
 * filled in by the user.
 *
 * Things to do
 *
 */

#include "button.h"

/* ARGSUSED */
int ui::button::get_arm_state(GLuint t, void *v)
{
    *((bool *)v) = this->armed;
    return 0;
}

/* ARGSUSED */
void ui::button::set_arm_state(GLuint t, void *v)
{
    this->armed = *((bool *)v);
}

/* ARGSUSED */
void ui::button::activate(ui::panel *p, void *call, void *client)
{
    for (int i = 0; i < 4; ++i)
    {
        GLuint border;

        p->get(ui::element::border, i, &border);
        ++border;
        p->set(ui::element::border, i, &border);
    }
}

/* ARGSUSED */
void ui::button::deactivate(ui::panel *p, void *call, void *client)
{
    for (int i = 0; i < 4; ++i)
    {
        GLuint border;

        p->get(ui::element::border, i, &border);
        border = (border > 1 ? border - 1 : 0);
        p->set(ui::element::border, i, &border);
    }
}

ui::button::button(ui::context *c, GLuint w, GLuint h)
    : ui::label::label(c, w, h)
{
    this->armed = false;

    this->add_callback(ui::callback::enter, ui::button::activate, NULL);
    this->add_callback(ui::callback::leave, ui::button::deactivate, NULL);
}

ui::button::~button()
{
}

int ui::button::get(GLuint e, GLuint t, void *v)
{
    switch (e)
    {
      case ui::element::arm:  return this->get_arm_state(t, v);
      default:                return ui::label::get(e, t, v);
    }
}

void ui::button::set(GLuint e, GLuint t, void *v)
{
    switch (e)
    {
      case ui::element::arm:
        this->set_arm_state(t, v);
        break;

      default:
        ui::label::set(e, t, v);
        break;
    }
}
