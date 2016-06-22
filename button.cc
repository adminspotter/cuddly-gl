/* label.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 22 Jun 2016, 17:03:18 tquirk
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
    this->add_callback(ui::callback::enter, ui::button::activate, NULL);
    this->add_callback(ui::callback::leave, ui::button::deactivate, NULL);
}

ui::button::~button()
{
}
