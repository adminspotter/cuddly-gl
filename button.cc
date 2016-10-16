/* button.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 13 Oct 2016, 08:38:50 tquirk
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

#include <algorithm>

#include "ui_defs.h"
#include "button.h"

/* ARGSUSED */
int ui::button::get_active_state(GLuint t, void *v)
{
    *((bool *)v) = this->activated;
    return 0;
}

/* ARGSUSED */
void ui::button::set_active_state(GLuint t, void *v)
{
    bool new_act = *((bool *)v);

    if (new_act == this->activated)
        return;
    this->activated = new_act;
    if (this->activated)
        this->grow_border();
    else
        this->shrink_border();
}

/* ARGSUSED */
int ui::button::get_arm_state(GLuint t, void *v)
{
    *((bool *)v) = this->armed;
    return 0;
}

/* ARGSUSED */
void ui::button::set_arm_state(GLuint t, void *v)
{
    bool new_arm =  *((bool *)v);

    if (new_arm == this->armed)
        return;
    this->armed = new_arm;
    if (this->armed)
        this->grow_border();
    else
        this->shrink_border();
}

void ui::button::set_margin(GLuint s, void *v)
{
    GLuint new_v = *((GLuint *)v);
    GLuint min_val = (this->activated ? 0 : 1) + (this->armed ? 0 : 1);

    if (s & ui::side::top || s & ui::side::bottom)
        if (this->border[0] + this->border[3]
            + (s & ui::side::top ? new_v : this->margin[0])
            + (s & ui::side::bottom ? new_v : this->margin[3]) <= this->dim.y)
        {
            if (s & ui::side::top)
                this->margin[0] = std::max(new_v, min_val);
            if (s & ui::side::bottom)
                this->margin[3] = std::max(new_v, min_val);
        }

    if (s & ui::side::left || s & ui::side::right)
        if (this->border[1] + this->border[2]
            + (s & ui::side::left ? new_v : this->margin[1])
            + (s & ui::side::right ? new_v : this->margin[2]) <= this->dim.x)
        {
            if (s & ui::side::left)
                this->margin[1] = std::max(new_v, min_val);
            if (s & ui::side::right)
                this->margin[2] = std::max(new_v, min_val);
        }
}

void ui::button::grow_border(void)
{
    for (int i = 0; i < 4; ++i)
    {
        ++this->border[i];
        this->margin[i] = (this->margin[i] > 1 ? this->margin[i] - 1 : 0);
    }
}

void ui::button::shrink_border(void)
{
    for (int i = 0; i < 4; ++i)
    {
        this->border[i] = (this->border[i] > 1 ? this->border[i] - 1 : 0);
        ++this->margin[i];
    }
}

/* ARGSUSED */
void ui::button::activate(ui::active *a, void *call, void *client)
{
    ui::button *b = dynamic_cast<ui::button *>(a);
    bool active = true;

    if (b != NULL)
        b->set(ui::element::state, ui::state::active, &active);
}

/* ARGSUSED */
void ui::button::deactivate(ui::active *a, void *call, void *client)
{
    ui::button *b = dynamic_cast<ui::button *>(a);
    bool active = false;

    if (b != NULL)
        b->set_va(ui::element::state, ui::state::active, &active,
                  ui::element::state, ui::state::armed, &active, 0);
}

void ui::button::arm(ui::active *a, void *call, void *client)
{
    ui::button *b = dynamic_cast<ui::button *>(a);
    bool armed = true;

    if (b != NULL)
        b->set(ui::element::state, ui::state::armed, &armed);
}

void ui::button::disarm(ui::active *a, void *call, void *client)
{
    ui::button *b = dynamic_cast<ui::button *>(a);
    bool armed = false;

    if (b != NULL)
        b->set(ui::element::state, ui::state::armed, &armed);
}

ui::button::button(ui::composite *c, GLuint w, GLuint h)
    : ui::label::label(c, w, h), ui::rect::rect(w, h)
{
    this->activated = false;
    this->armed = false;

    for (int i = 0; i < 4; ++i)
        this->margin[i] = std::max(this->margin[i], 2u);
    this->populate_buffers();

    this->add_callback(ui::callback::enter,     ui::button::activate, NULL);
    this->add_callback(ui::callback::leave,     ui::button::deactivate, NULL);
    this->add_callback(ui::callback::btn_down,  ui::button::arm, NULL);
    this->add_callback(ui::callback::btn_up,    ui::button::disarm, NULL);
}

ui::button::~button()
{
}

int ui::button::get(GLuint e, GLuint t, void *v)
{
    if (e == ui::element::state)
    {
        switch (t)
        {
          case ui::state::active:  return this->get_active_state(t, v);
          case ui::state::armed:   return this->get_arm_state(t, v);
        }
    }
    return ui::label::get(e, t, v);
}

void ui::button::set(GLuint e, GLuint t, void *v)
{
    if (e == ui::element::state)
        switch (t)
        {
          case ui::state::active:
            this->set_active_state(t, v);
            this->populate_buffers();
            break;

          case ui::state::armed:
            this->set_arm_state(t, v);
            this->populate_buffers();
            break;
        }
    else
        ui::label::set(e, t, v);
}
