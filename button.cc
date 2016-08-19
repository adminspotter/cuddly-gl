/* button.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 19 Aug 2016, 16:05:33 tquirk
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
    *((bool *)v) = this->active;
    return 0;
}

/* ARGSUSED */
void ui::button::set_active_state(GLuint t, void *v)
{
    bool new_act = *((bool *)v);

    if (new_act == this->active)
        return;
    this->active = new_act;
    if (this->active)
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
    GLuint min_val = (this->active ? 0 : 1) + (this->armed ? 0 : 1);

    if (s & ui::side::top || s & ui::side::bottom)
        if (this->border[0] + this->border[3]
            + (s & ui::side::top ? new_v : this->margin[0])
            + (s & ui::side::bottom ? new_v : this->margin[3]) <= this->height)
        {
            if (s & ui::side::top)
                this->margin[0] = std::max(new_v, min_val);
            if (s & ui::side::bottom)
                this->margin[3] = std::max(new_v, min_val);
        }

    if (s & ui::side::left || s & ui::side::right)
        if (this->border[1] + this->border[2]
            + (s & ui::side::left ? new_v : this->margin[1])
            + (s & ui::side::right ? new_v : this->margin[2]) <= this->width)
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
void ui::button::activate(ui::panel *p, void *call, void *client)
{
    bool active = true;

    p->set(ui::element::active, 0, &active);
}

/* ARGSUSED */
void ui::button::deactivate(ui::panel *p, void *call, void *client)
{
    bool active = false;

    p->set_va(ui::element::active, 0, &active,
              ui::element::arm, 0, &active, 0);
}

void ui::button::arm(ui::panel *p, void *call, void *client)
{
    bool armed = true;

    p->set(ui::element::arm, 0, &armed);
}

void ui::button::disarm(ui::panel *p, void *call, void *client)
{
    bool is_armed = false;

    p->set(ui::element::arm, 0, &is_armed);
}

ui::button::button(ui::composite *c, GLuint w, GLuint h)
    : ui::label::label(c, w, h)
{
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
    switch (e)
    {
      case ui::element::active:  return this->get_active_state(t, v);
      case ui::element::arm:     return this->get_arm_state(t, v);
      default:                   return ui::label::get(e, t, v);
    }
}

void ui::button::set(GLuint e, GLuint t, void *v)
{
    switch (e)
    {
      case ui::element::active:
        this->set_active_state(t, v);
        this->populate_buffers();
        break;

      case ui::element::arm:
        this->set_arm_state(t, v);
        this->populate_buffers();
        break;

      default:
        ui::label::set(e, t, v);
        break;
    }
}
