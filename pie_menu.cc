/* pie_menu.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 20 Nov 2019, 08:05:27 tquirk
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
 * This file contains the method definitions for the popup menu.  Our
 * long-desired pie menus are finally implemented!
 *
 * Things to do
 *
 */

#include <math.h>

#include <algorithm>

#include <glm/gtc/type_ptr.hpp>
#include <glm/geometric.hpp>

#include "ui_defs.h"
#include "ui.h"
#include "pie_menu.h"

#define TWO_PI  M_PI * 2.0f

#define INNER_PCT  0.1f

int ui::pie_menu::get_popup(GLuint t, GLuint *v) const
{
    switch (t)
    {
      case ui::popup::button:  *v = this->popup_button;  return 0;
      default:                                           return 1;
    }
}

void ui::pie_menu::set_popup(GLuint t, GLuint v)
{
    if (t == ui::popup::button)
        this->popup_button = v;
}

void ui::pie_menu::set_resize(GLuint t, GLuint v)
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
        glm::ivec2 new_loc(bcd->location);
        new_loc.x -= pm->dim.x / 2;
        new_loc.y -= pm->dim.y / 2;
        pm->set(ui::element::position, ui::position::all, new_loc,
                ui::element::state, ui::state::visible, true);
        pm->composite::parent->move_child(pm);
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
        pm->set(ui::element::state, ui::state::visible, false);
}

void ui::pie_menu::set_desired_size(void)
{
    this->composite::set_desired_size();

    if (this->children.size() > 0)
    {
        float increment = TWO_PI / this->children.size();
        auto child = this->children.begin();
        glm::ivec2 child_pos, child_size;
        glm::ivec2 middle = this->dim / 4;
        glm::ivec2 center = this->dim / 2;

        for (int i = 0; i < this->children.size(); ++i, ++child)
        {
            float angle = increment * (i + 0.5);

            /* Reposition each child to be in the middle of its
             * sector.
             */
            (*child)->get(ui::element::size, ui::size::all, &child_size);
            child_pos.x = center.x + (int)truncf((float)middle.x * cos(angle))
                - (child_size.x / 2);
            child_pos.y = center.y + (int)truncf((float)middle.y * sin(angle))
                - (child_size.y / 2);
            (*child)->set(ui::element::position, ui::position::all, child_pos);
        }
    }
    this->dirty = false;
    this->populate_buffers();
}

ui::vertex_buffer *ui::pie_menu::generate_points(void)
{
    glm::vec3 pixel_sz;
    glm::vec2 center(-1.0f, 1.0f), m0, m3;
    glm::vec2 radius(this->dim.x / 2.0f, this->dim.y / 2.0f);
    float inner_pct = INNER_PCT, pct;
    int count = std::max(this->dim.x / 3, 15);
    ui::vertex_buffer *vb = new ui::vertex_buffer();

    this->composite::parent->get(ui::element::pixel_size,
                                 ui::size::all,
                                 &pixel_sz);
    radius.x *= pixel_sz.x;
    radius.y *= pixel_sz.y;
    center.x += this->dim.x / 2.0 * pixel_sz.x;
    center.y -= this->dim.y / 2.0 * pixel_sz.y;

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
        float increment = TWO_PI / this->children.size();
        float angle = 0.0f;

        pct = m3.x / m0.x;
        for (int i = 0; i < this->children.size(); ++i)
        {
            angle += increment;
            vb->generate_ellipse_divider(center, m0, pct, angle,
                                         this->foreground);
        }
    }

    return vb;
}

int ui::pie_menu::which_sector(glm::ivec2& loc)
{
    float y_factor = (float)this->dim.y / (float)this->dim.x;
    glm::vec2 sector_pos(loc.x - (this->dim.x / 2),
                         (loc.y - (this->dim.y / 2)) / y_factor);
    float length = glm::length(sector_pos);
    float angle = atan2f(sector_pos.y, sector_pos.x);
    glm::vec2 radius(this->dim.x / 2.0f * cosf(angle),
                     this->dim.y / 2.0f * sinf(angle) / y_factor);
    float outer_length = glm::length(radius);

    if (length < outer_length * INNER_PCT || length > outer_length)
        return -1;

    if (angle < 0.0f)
        angle += TWO_PI;
    float increment = TWO_PI / this->children.size();

    return truncf(angle / increment);
}

ui::widget *ui::pie_menu::which_child(glm::ivec2& loc)
{
    int sector = this->which_sector(loc);

    if (sector == -1)
        return NULL;

    /* There's no index operator for the std::list, which is how our
     * children are held, so we'll have to sort of fake one.
     */
    std::list<ui::widget *>::iterator child = this->children.begin();
    for (int i = 0; i < sector; ++i, ++child)
        ;
    return (*child);
}

void ui::pie_menu::init(ui::composite *c)
{
    this->popup_button = ui::mouse::button2;
    this->resize = ui::resize::none;
    this->visible = false;

    if (c != NULL)
    {
        c->add_callback(ui::callback::btn_down, ui::pie_menu::show, this);
        c->add_callback(ui::callback::btn_up, ui::pie_menu::hide, this);
    }
    this->add_callback(ui::callback::btn_up, ui::pie_menu::hide, this);

    this->populate_buffers();
}

ui::pie_menu::pie_menu(composite *c)
    : ui::manager::manager(c), ui::active::active(0, 0), ui::rect(0, 0)
{
    this->init(c);
}

ui::pie_menu::~pie_menu()
{
    if (this->composite::parent != NULL)
    {
        this->composite::parent->remove_callback(ui::callback::btn_down,
                                                 ui::pie_menu::show,
                                                 this);
        this->composite::parent->remove_callback(ui::callback::btn_up,
                                                 ui::pie_menu::hide,
                                                 this);
    }
}

int ui::pie_menu::get(GLuint e, GLuint t, GLuint *v) const
{
    if (e == ui::element::popup)
        return this->get_popup(t, v);
    return this->manager::get(e, t, v);
}

void ui::pie_menu::set(GLuint e, GLuint t, GLuint v)
{
    if (e == ui::element::popup)
        this->set_popup(t, v);
    else
        this->manager::set(e, t, v);
}

void ui::pie_menu::mouse_pos_callback(glm::ivec2& pos)
{
    ui::mouse_call_data call_data = {pos};
    ui::widget *w = this->which_child(pos);

    if (w != NULL)
    {
        if (this->old_child != w)
        {
            if (this->old_child != NULL)
                this->old_child->call_callbacks(ui::callback::leave,
                                                &call_data);
            w->call_callbacks(ui::callback::enter, &call_data);
        }
        w->call_callbacks(ui::callback::motion, &call_data);
    }
    else if (this->old_child != NULL)
        this->old_child->call_callbacks(ui::callback::leave, &call_data);

    /* The composite's callbacks all search again at the end of
     * execution, in case the widget we worked on disappeared or was
     * deleted during the callback.  A menu which changes while you're
     * using it would be strange, and searching the pie menu is kind
     * of expensive, so we won't do that here.
     */
    this->old_child = w;
    this->old_pos = pos;
}

void ui::pie_menu::mouse_btn_callback(ui::btn_call_data& call_data)
{
    GLuint which = (call_data.state == ui::mouse::up
                    ? ui::callback::btn_up
                    : ui::callback::btn_down);
    ui::widget *w = this->which_child(call_data.location);

    if (w != NULL)
        w->call_callbacks(which, &call_data);

    /* We'll have the hide callback in our list, which is likely
     * appropriate.
     */
    this->call_callbacks(which, &call_data);
}

void ui::pie_menu::draw(GLuint trans_uniform, const glm::mat4& parent_trans)
{
    if (this->visible)
        this->manager::draw(trans_uniform, parent_trans);
}
