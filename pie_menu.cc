/* pie_menu.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 26 Feb 2017, 09:25:07 tquirk
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
 * This file contains the method definitions for the popup menu.  Our
 * long-desired pie menus are finally implemented!
 *
 * When not visible, the menu lives outside of the screen coordinates,
 * so that it doesn't get found when its parent searches its area.
 * Otherwise, it turns into a dead zone which is the size of the
 * menu's bounding box.
 *
 * Things to do
 *
 */

#include <math.h>

#include <algorithm>

#include <glm/gtc/type_ptr.hpp>

#include "ui_defs.h"
#include "ui.h"
#include "pie_menu.h"

int ui::pie_menu::get_popup(GLuint t, void *v)
{
    int ret = 0;

    switch (t)
    {
      case ui::popup::button:   *(int *)v = this->popup_button;  break;
      default:                  ret = 1;                         break;
    }
    return ret;
}

void ui::pie_menu::set_popup(GLuint t, void *v)
{
    switch (t)
    {
      case ui::popup::button:   this->popup_button = *(int *)v;  break;
    }
}

void ui::pie_menu::set_resize(GLuint t, void *v)
{
    /* No-op, since we don't want this to change */
}

/* This is the callback that we add to our parents, in order that they
 * pop us up under the right conditions.  The client data pointer
 * should point to us.
 */
void ui::pie_menu::show(ui::active *a, void *call, void *client)
{
    ui::pie_menu *pm = (ui::pie_menu *)client;
    ui::btn_call_data *bcd = (ui::btn_call_data *)call;

    if (bcd->button == pm->popup_button && bcd->state == ui::mouse::down)
    {
        bool t = true;
        pm->set_va(ui::element::position, ui::position::all, &(bcd->location),
                   ui::element::state, ui::state::visible, &t, 0);
    }
}

/* We should not assume that the active parameter points to the popup
 * menu.  This may be called from our parent, so we should use the
 * client data, which should always point to us.
 */
void ui::pie_menu::hide(ui::active *a, void *call, void *client)
{
    ui::pie_menu *pm = (ui::pie_menu *)client;
    ui::btn_call_data *bcd = (ui::btn_call_data *)call;

    if (bcd->button == pm->popup_button && bcd->state == ui::mouse::up)
    {
        bool f = false;
        pm->set(ui::element::state, ui::state::visible, &f);
    }
}

ui::vertex_buffer *ui::pie_menu::generate_points(void)
{
    glm::vec2 pixel_sz;
    glm::vec2 center(-1.0f, 1.0f), m0, m3;
    glm::vec2 radius(this->dim.x / 2.0f, this->dim.y / 2.0f);
    float inner_pct = 0.1f, pct;
    int count = std::max(this->dim.x / 3, 15);

    /* We need room for 6 sets of points - inner and outer edges, plus
     * possible inner and outer borders (2 points each).  Each point
     * is 8 floats - 2 xy, 4 color, 2 texture st.  We also need
     * dividers for each sector, so that's another 4 points per
     * divider.
     */
    ui::vertex_buffer *vb = new ui::vertex_buffer(
        (count * 48) + (this->children.size() * 32),
        (count * 18) + (this->children.size() * 6));

    this->composite::parent->get(ui::element::pixel_size,
                                 ui::size::all,
                                 &pixel_sz);
    radius.x *= pixel_sz.x;
    radius.y *= pixel_sz.y;
    m0 = radius - glm::vec2(this->margin[0] * pixel_sz.x,
                            this->margin[0] * pixel_sz.y);
    m3 = (radius * inner_pct)
        + glm::vec2((this->margin[3] + this->border[3]) * pixel_sz.x,
                    (this->margin[3] + this->border[3]) * pixel_sz.y);

    vb->generate_ellipse(center, radius, inner_pct, count, this->background);

    /* Outer border */
    if (this->border[0] != 0)
    {
        pct = (float)this->border[0] / std::min(m0.x, m0.y);
        vb->generate_ellipse(center, m0, pct, count, this->foreground);
    }

    /* Inner border */
    if (this->border[3] != 0)
    {
        pct = (float)this->border[3] / std::min(m3.x, m3.y);
        vb->generate_ellipse(center, m3, pct, count, this->foreground);
    }

    if (this->children.size() > 0)
    {
        auto child = this->children.begin();
        glm::ivec2 middle = this->dim / 4;
        glm::ivec2 child_pos, child_size;
        int increment = M_PI * 2.0f / this->children.size();

        for (int i = 0; i < this->children.size(); ++i, ++child)
        {
            float angle = increment * i;

            pct = m3.x / m0.x;
            vb->generate_ellipse_divider(center, m0, pct, angle,
                                         this->foreground);

            /* Reposition each child to be in the middle of its
             * sector.
             */
            angle += increment / 2.0f;
            (*child)->get(ui::element::size, ui::size::all, &child_size);
            child_pos.x = (int)truncf((float)middle.x * cos(angle))
                + this->pos.x - (child_size.x / 2);
            child_pos.y = (int)truncf((float)middle.y * sin(angle))
                + this->pos.y - (child_size.y / 2);
            (*child)->set(ui::element::position, ui::position::all, &child_pos);
        }
    }

    return vb;
}

ui::pie_menu::pie_menu(composite *c, GLuint w, GLuint h)
    : ui::manager::manager(c, w, h), ui::rect(w, h)
{
    ui::active *a = dynamic_cast<ui::active *>(c);
    this->popup_button = ui::mouse::button2;
    this->resize = ui::resize::none;
    this->visible = false;

    if (a != NULL)
    {
        a->add_callback(ui::callback::btn_down, ui::pie_menu::show, this);
        a->add_callback(ui::callback::btn_up, ui::pie_menu::hide, this);
    }
    this->add_callback(ui::callback::btn_up, ui::pie_menu::hide, this);

    this->populate_buffers();
}

ui::pie_menu::~pie_menu()
{
    ui::active *a;

    if ((a = dynamic_cast<ui::active *>(this->composite::parent)) != NULL)
    {
        a->remove_callback(ui::callback::btn_down, ui::pie_menu::show, this);
        a->remove_callback(ui::callback::btn_up, ui::pie_menu::hide, this);
    }
}

int ui::pie_menu::get(GLuint e, GLuint t, void *v)
{
    if (e == ui::element::popup)
        return this->get_popup(t, v);
    return this->manager::get(e, t, v);
}

void ui::pie_menu::set(GLuint e, GLuint t, void *v)
{
    if (e == ui::element::popup)
        this->set_popup(t, v);
    else
        this->manager::set(e, t, v);
}

void ui::pie_menu::draw(GLuint trans_uniform, const glm::mat4& parent_trans)
{
    if (this->visible)
        this->manager::draw(trans_uniform, parent_trans);
}
