/* button.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *
 * CuddlyGL OpenGL widget toolkit
 * Copyright (C) 2016-2020  Trinity Annabelle Quirk
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

#include "button.h"

void ui::button::set_margin(GLuint s, GLuint v)
{
    GLuint min_val = (this->activated ? 0 : 1) + (this->armed ? 0 : 1);

    if (s & ui::side::top || s & ui::side::bottom)
        if (this->border[0] + this->border[3]
            + ((s & ui::side::top) ? v : this->margin[0])
            + ((s & ui::side::bottom) ? v : this->margin[3]) <= this->dim.y)
        {
            if (s & ui::side::top)
                this->margin[0] = std::max(v, min_val);
            if (s & ui::side::bottom)
                this->margin[3] = std::max(v, min_val);
        }

    if (s & ui::side::left || s & ui::side::right)
        if (this->border[1] + this->border[2]
            + ((s & ui::side::left) ? v : this->margin[1])
            + ((s & ui::side::right) ? v : this->margin[2]) <= this->dim.x)
        {
            if (s & ui::side::left)
                this->margin[1] = std::max(v, min_val);
            if (s & ui::side::right)
                this->margin[2] = std::max(v, min_val);
        }
    this->populate_buffers();
}

void ui::button::set_active_state(bool v)
{
    if (v == this->activated)
        return;
    if ((this->activated = v) == true)
        this->grow_border();
    else
        this->shrink_border();
}

void ui::button::set_arm_state(bool v)
{
    if (v == this->armed)
        return;
    if ((this->armed = v) == true)
        this->grow_border();
    else
        this->shrink_border();
}

void ui::button::grow_border(void)
{
    for (int i = 0; i < 4; ++i)
    {
        ++this->border[i];
        this->margin[i] = (this->margin[i] > 1 ? this->margin[i] - 1 : 0);
    }
    this->populate_buffers();
}

void ui::button::shrink_border(void)
{
    for (int i = 0; i < 4; ++i)
    {
        this->border[i] = (this->border[i] > 1 ? this->border[i] - 1 : 0);
        ++this->margin[i];
    }
    this->populate_buffers();
}

void ui::button::init(ui::composite *c)
{
    for (int i = 0; i < 4; ++i)
        this->margin[i] = std::max(this->margin[i], 2u);
    this->populate_buffers();
}

ui::button::button(ui::composite *c)
    : ui::armable::armable(c), ui::active::active(0, 0), ui::rect::rect(0, 0)
{
    this->init(c);
}

ui::button::~button()
{
}
