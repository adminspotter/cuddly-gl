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

void ui::toggle::calculate_widget_size(void)
{
    glm::ivec2 size;

    if (this->img.width > 0 && this->img.height > 0)
    {
        /* We want an extra pixel of space between the string and each
         * side, even if there is no border or margin, thus the
         * literal 2s.
         *
         * The extra img.height in the x-dimension is to accommodate
         * the checkbox.
         */
        size.x = this->img.width + this->img.height
            + this->margin[1] + this->margin[2]
            + this->border[1] + this->border[2] + 2;
        size.y = this->img.height
            + this->margin[0] + this->margin[3]
            + this->border[0] + this->border[3] + 2;
        this->set_size(ui::size::all, size);
    }
}

ui::vertex_buffer *ui::toggle::generate_points(void)
{
    ui::vertex_buffer *vb = this->label::generate_points();

    if (this->img.data == NULL)
        return vb;

    float pw = 1.0f / (float)this->img.width;

    /* We want the checkbox on the left, which is going to be as wide
     * as the image is high (so we can make it square), so we need to
     * scoot the image over to the right side.
     */
    vb->vertex[6]  = 0.0f
        - ((this->margin[1] + this->border[1] + 1 + this->img.height) * pw);
    vb->vertex[14] = 1.0f
        + ((this->margin[2] + this->border[2] + 1) * pw);
    vb->vertex[22] = vb->vertex[6];
    vb->vertex[30] = vb->vertex[14];

    return vb;
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
