/* button.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 12 Jul 2016, 10:36:59 tquirk
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

void ui::button::grow_border(ui::panel *p)
{
    int i = 1;

    while (i < 16)
    {
        GLuint border, margin;

        p->get_va(ui::element::border, i, &border,
                  ui::element::margin, i, &margin, 0);
        ++border;
        margin = (margin > 1 ? margin - 1 : 0);
        p->set_va(ui::element::border, i, &border,
                  ui::element::margin, i, &margin, 0);
        i <<= 1;
    }
}

void ui::button::shrink_border(ui::panel *p)
{
    int i = 1;

    while (i < 16)
    {
        GLuint border, margin;

        p->get_va(ui::element::border, i, &border,
                  ui::element::margin, i, &margin, 0);
        border = (border > 1 ? border - 1 : 0);
        ++margin;
        p->set_va(ui::element::border, i, &border,
                  ui::element::margin, i, &margin, 0);
        i <<= 1;
    }
}

int ui::button::get_margin(GLuint t, void *v)
{
    int ret = 0;

    switch (t)
    {
      case ui::side::top:    *((GLuint *)v) = this->margin[0] - 2; break;
      case ui::side::left:   *((GLuint *)v) = this->margin[1] - 2; break;
      case ui::side::right:  *((GLuint *)v) = this->margin[2] - 2; break;
      case ui::side::bottom: *((GLuint *)v) = this->margin[3] - 2; break;
      default:               ret = 1;                              break;
    }
    return ret;
}

void ui::button::set_margin(GLuint s, void *v)
{
    GLuint new_v = *((GLuint *)v);

    if (s & ui::side::top || s & ui::side::bottom)
        if (this->border[0] + this->border[3]
            + (s & ui::side::top ? new_v : this->margin[0])
            + (s & ui::side::bottom ? new_v : this->margin[3]) <= this->height)
        {
            if (s & ui::side::top)     this->margin[0] = new_v + 2;
            if (s & ui::side::bottom)  this->margin[3] = new_v + 2;
        }

    if (s & ui::side::left || s & ui::side::right)
        if (this->border[1] + this->border[2]
            + (s & ui::side::left ? new_v : this->margin[1])
            + (s & ui::side::right ? new_v : this->margin[2]) <= this->width)
        {
            if (s & ui::side::left)    this->margin[1] = new_v + 2;
            if (s & ui::side::right)   this->margin[2] = new_v + 2;
        }
}

/* ARGSUSED */
void ui::button::activate(ui::panel *p, void *call, void *client)
{
    ui::button::grow_border(p);
}

/* ARGSUSED */
void ui::button::deactivate(ui::panel *p, void *call, void *client)
{
    /* p knows whether it's armed or not, so this call will never be
     * inappropriate.
     */
    ui::button::disarm(p, NULL, NULL);

    ui::button::shrink_border(p);
}

void ui::button::arm(ui::panel *p, void *call, void *client)
{
    bool armed = true;

    p->set(ui::element::arm, 0, &armed);
    ui::button::grow_border(p);
}

void ui::button::disarm(ui::panel *p, void *call, void *client)
{
    bool is_armed;

    p->get(ui::element::arm, 0, &is_armed);
    if (is_armed)
    {
        is_armed = false;
        p->set(ui::element::arm, 0, &is_armed);
        ui::button::shrink_border(p);
    }
}

ui::button::button(ui::context *c, GLuint w, GLuint h)
    : ui::label::label(c, w, h)
{
    this->armed = false;

    for (int i = 0; i < 4; ++i)
        this->margin[i] += 2;

    this->add_callback(ui::callback::enter, ui::button::activate, NULL);
    this->add_callback(ui::callback::leave, ui::button::deactivate, NULL);
    this->add_callback(ui::callback::down,  ui::button::arm, NULL);
    this->add_callback(ui::callback::up,    ui::button::disarm, NULL);
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
