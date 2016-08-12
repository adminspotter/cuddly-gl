/* manager.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 12 Aug 2016, 06:55:03 tquirk
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
 * This file contains the method definitions for the manager object,
 * which combines the panel with the composite.
 *
 * Things to do
 *
 */

#include "ui_defs.h"
#include "manager.h"

void ui::manager::motion_callback(panel *p, void *call, void *client)
{
    ui::manager *m;

    if ((m = dynamic_cast<ui::manager *>(p)) != NULL)
    {
        ui::mouse_call_data *mcd = (ui::mouse_call_data *)call;

        m->mouse_pos_callback(mcd->location.x, mcd->location.y);
    }
}

void ui::manager::button_callback(panel *p, void *call, void *client)
{
    ui::manager *m;

    if ((m = dynamic_cast<ui::manager *>(p)) != NULL)
    {
        ui::btn_call_data *bcd = (ui::btn_call_data *)call;

        m->mouse_btn_callback(bcd->button, bcd->state);
    }
}

void ui::manager::keypress_callback(panel *p, void *call, void *client)
{
    ui::manager *m;

    if ((m = dynamic_cast<ui::manager *>(p)) != NULL)
    {
        ui::key_call_data *kcd = (ui::key_call_data *)call;

        m->key_callback(kcd->key, kcd->character, kcd->state, kcd->mods);
    }
}

ui::manager::manager(ui::composite *c, GLuint w, GLuint h)
    : ui::panel::panel(c, w, h), ui::composite::composite(c, w, h)
{
    this->add_callback(ui::callback::motion,
                       ui::manager::motion_callback, NULL);
    this->add_callback(ui::callback::btn_down,
                       ui::manager::button_callback, NULL);
    this->add_callback(ui::callback::btn_up,
                       ui::manager::button_callback, NULL);
    this->add_callback(ui::callback::key_down,
                       ui::manager::keypress_callback, NULL);
    this->add_callback(ui::callback::key_up,
                       ui::manager::keypress_callback, NULL);
}

ui::manager::~manager()
{
}

void ui::manager::draw(void)
{
    this->panel::draw();
    for (auto i = this->children.begin(); i != this->children.end(); ++i)
        (*i)->draw();
}
