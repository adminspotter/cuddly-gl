/* pie_menu.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 25 Oct 2016, 16:49:35 tquirk
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

/* We'll use a parametric function to draw our ellipse.
 *
 * <radius.x * cos(theta), radius.y * sin(theta)>
 */
void ui::popupmenu::populate_buffers(void)
{
    glm::vec2 pixel_sz;
    glm::vec2 radius(this->dim.x / 2.0f, this->dim.y / 2.0f);
    glm::vec2 inner = radius * 0.15f;
    glm::vec2 center, m0, m3, b0, b3;

    if (this->border[0] != 0)
    {
        m0 = radius - glm::vec2(this->margin[0], this->margin[0]);
        b0 = m0 - glm::vec2(this->border[0], this->border[0]);
    }
    if (this->border[3] != 0)
    {
        m3 = inner + glm::vec2(this->margin[3], this->margin[3]);
        b3 = m3 + glm::vec2(this->border[3], this->border[3]);
    }

    int count = std::max(this->dim.x / 3, 15);

    /* We need room for 8 sets of points - inner and outer edges, plus
     * possible inner and outer borders (2 points each).  Each point
     * is 8 floats - 2 xy, 4 color, 2 texture st.  We also need
     * dividers for each sector, so that's another 4 points per
     * divider.
     */
    float vertex[(count * 64) + (this->children.size() * 32)];
    float increment = M_PI * 2.0f / (float)count;
    GLuint element[(count * 18) + (this->children.size() * 6)];
    int i, border_index = count * 16, border_count = count * 2;
    int border_element = count * 6, vert_count = 0;

    this->composite::parent->get(ui::element::pixel_size,
                                 ui::size::all,
                                 &pixel_sz);
    radius.x *= pixel_sz.x;  inner.x *= pixel_sz.x;
    b0.x *= pixel_sz.x;      m0.x *= pixel_sz.x;
    b3.x *= pixel_sz.x;      m3.x *= pixel_sz.x;
    radius.y *= pixel_sz.y;  inner.y *= pixel_sz.y;
    b0.y *= pixel_sz.y;      m0.y *= pixel_sz.y;
    b3.y *= pixel_sz.y;      m3.y *= pixel_sz.y;

    center.x = this->pos.x * pixel_sz.x - 1.0f;
    center.y = -this->pos.y * pixel_sz.y + 1.0f;

    this->vertex_count = this->element_count = 0;

    for (i = 0; i < count; ++i)
    {
        float angle = increment * i;

        vertex[this->vertex_count] = radius.x * cosf(angle) + center.x;
        vertex[this->vertex_count + 1] = radius.y * sinf(angle) + center.y;
        memcpy(&vertex[this->vertex_count + 2],
               glm::value_ptr(this->background),
               sizeof(float) * 4);
        vertex[this->vertex_count + 6] = ui::panel::no_texture;
        vertex[this->vertex_count + 7] = ui::panel::no_texture;

        vertex[this->vertex_count + 8] = inner.x * cosf(angle) + center.x;
        vertex[this->vertex_count + 9] = inner.y * sinf(angle) + center.y;
        memcpy(&vertex[this->vertex_count + 10],
               glm::value_ptr(this->background),
               sizeof(float) * 4);
        vertex[this->vertex_count + 14] = ui::panel::no_texture;
        vertex[this->vertex_count + 15] = ui::panel::no_texture;
        this->vertex_count += 16;
        vert_count += 2;

        element[this->element_count] = vert_count - 2;
        element[this->element_count + 1] = vert_count - 1;
        element[this->element_count + 2] = (vert_count + 1) % (count * 2);
        element[this->element_count + 3] = vert_count - 2;
        element[this->element_count + 4] = (vert_count + 1) % (count * 2);
        element[this->element_count + 5] = vert_count % (count * 2);
        this->element_count += 6;

        /* Outer border */
        if (this->border[0] != 0)
        {
            vertex[border_index] = m0.x * cosf(angle) + center.x;
            vertex[border_index + 1] = m0.y * sinf(angle) + center.y;
            memcpy(&vertex[border_index + 2],
                   glm::value_ptr(this->foreground),
                   sizeof(float) * 4);
            vertex[border_index + 6] = ui::panel::no_texture;
            vertex[border_index + 7] = ui::panel::no_texture;

            vertex[border_index + 8] = b0.x * cosf(angle) + center.x;
            vertex[border_index + 9] = b0.y * sinf(angle) + center.y;
            memcpy(&vertex[border_index + 10],
                   glm::value_ptr(this->foreground),
                   sizeof(float) * 4);
            vertex[border_index + 14] = ui::panel::no_texture;
            vertex[border_index + 15] = ui::panel::no_texture;
            border_index += 16;
            border_count += 2;

            element[border_element] =  border_count - 2;
            element[border_element + 1] = border_count - 1;
            element[border_element + 2]
                = border_count + (this->border[3] != 0 ? 3 : 1);
            element[border_element + 3] = border_count - 2;
            element[border_element + 4] = element[border_element + 2];
            element[border_element + 5]
                = border_count + (this->border[3] != 0 ? 2 : 0);

            /* Fix up the last segment */
            if (i == count - 1)
            {
                element[border_element + 2] = (count * 2) + 1;
                element[border_element + 4] = (count * 2) + 1;
                element[border_element + 5] = count * 2;
            }
            border_element += 6;
        }

        /* Inner border */
        if (this->border[3] != 0)
        {
            vertex[border_index] = b3.x * cosf(angle) + center.x;
            vertex[border_index + 1] = b3.y * sinf(angle) + center.y;
            memcpy(&vertex[border_index + 2],
                   glm::value_ptr(this->foreground),
                   sizeof(float) * 4);
            vertex[border_index + 6] = ui::panel::no_texture;
            vertex[border_index + 7] = ui::panel::no_texture;

            vertex[border_index + 8] = m3.x * cosf(angle) + center.x;
            vertex[border_index + 9] = m3.y * sinf(angle) + center.y;
            memcpy(&vertex[border_index + 10],
                   glm::value_ptr(this->foreground),
                   sizeof(float) * 4);
            vertex[border_index + 14] = ui::panel::no_texture;
            vertex[border_index + 15] = ui::panel::no_texture;
            border_index += 16;
            border_count += 2;

            element[border_element] =  border_count - 2;
            element[border_element + 1] = border_count - 1;
            element[border_element + 2]
                = border_count + (this->border[0] != 0 ? 3 : 1);
            element[border_element + 3] = border_count - 2;
            element[border_element + 4] = element[border_element + 2];
            element[border_element + 5]
                = border_count + (this->border[0] != 0 ? 2 : 0);

            /* Fix up the last segment */
            if (i == count - 1)
            {
                element[border_element + 2]
                    = (count * 2) + (this->border[0] != 0 ? 3 : 1);
                element[border_element + 4]
                    = (count * 2) + (this->border[0] != 0 ? 3 : 1);
                element[border_element + 5]
                    = (count * 2) + (this->border[0] != 0 ? 2 : 0);
            }
            border_element += 6;
        }
    }

    this->vertex_count = border_index;
    vert_count = border_count;
    this->element_count = border_element;

    if (this->children.size() > 0)
    {
        auto child = this->children.begin();
        glm::ivec2 middle = this->dim / 4;
        glm::ivec2 child_pos, child_size;

        increment = M_PI * 2.0f / this->children.size();

        /* We'll use the m elements for our divider endpoints.  If
         * they are already set, nice, and if not, we'll set to the
         * relevant edge of the background polys.
         */
        if (m0.x == 0.0f && m0.y == 0.0f)
            m0 = radius;
        if (m3.x == 0.0f && m3.y == 0.0f)
            m3 = inner;

        for (i = 0; i < this->children.size(); ++i, ++child)
        {
            float angle = increment * i;

            /* Add the dividers for each child */
            vertex[this->vertex_count] = m0.x * cosf(angle) + center.x;
            vertex[this->vertex_count + 1] = m0.y * sinf(angle) + center.y;
            memcpy(&vertex[this->vertex_count + 2],
                   glm::value_ptr(this->foreground),
                   sizeof(float) * 4);
            vertex[this->vertex_count + 6] = ui::panel::no_texture;
            vertex[this->vertex_count + 7] = ui::panel::no_texture;

            vertex[this->vertex_count + 8] = m3.x * cosf(angle) + center.x;
            vertex[this->vertex_count + 9] = m3.y * sinf(angle) + center.y;
            memcpy(&vertex[this->vertex_count + 10],
                   glm::value_ptr(this->foreground),
                   sizeof(float) * 4);
            vertex[this->vertex_count + 14] = ui::panel::no_texture;
            vertex[this->vertex_count + 15] = ui::panel::no_texture;

            /* Make the dividers one degree wide.  There's probably a
             * nice way to do this so the edges will be parallel, but
             * I don't have a network link to be able to research at
             * the moment.
             */
            angle += M_PI * 2.0f / 360;

            vertex[this->vertex_count + 16] = m0.x * cosf(angle) + center.x;
            vertex[this->vertex_count + 17] = m0.y * sinf(angle) + center.y;
            memcpy(&vertex[this->vertex_count + 18],
                   glm::value_ptr(this->foreground),
                   sizeof(float) * 4);
            vertex[this->vertex_count + 22] = ui::panel::no_texture;
            vertex[this->vertex_count + 23] = ui::panel::no_texture;

            vertex[this->vertex_count + 24] = m3.x * cosf(angle) + center.x;
            vertex[this->vertex_count + 25] = m3.y * sinf(angle) + center.y;
            memcpy(&vertex[this->vertex_count + 26],
                   glm::value_ptr(this->foreground),
                   sizeof(float) * 4);
            vertex[this->vertex_count + 30] = ui::panel::no_texture;
            vertex[this->vertex_count + 31] = ui::panel::no_texture;
            this->vertex_count += 32;
            vert_count += 4;

            element[this->element_count] = vert_count - 4;
            element[this->element_count + 1] = vert_count - 2;
            element[this->element_count + 2] = vert_count - 3;
            element[this->element_count + 3] = vert_count - 2;
            element[this->element_count + 4] = vert_count - 1;
            element[this->element_count + 5] = vert_count - 3;
            this->element_count += 6;

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
    glBindVertexArray(this->vao);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(float) * this->vertex_count, vertex,
                 GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(GLuint) * this->element_count, element,
                 GL_DYNAMIC_DRAW);
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

void ui::pie_menu::add_child(ui::widget *p)
{
    this->manager::add_child(p);
    this->populate_buffers();
}

void ui::pie_menu::remove_child(ui::widget *p)
{
    this->manager::remove_child(p);
    this->populate_buffers();
}
