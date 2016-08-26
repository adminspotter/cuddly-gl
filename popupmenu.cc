/* popupmenu.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 25 Aug 2016, 23:44:15 tquirk
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
 * Things to do
 *
 */

#include <math.h>

#include <algorithm>

#include <glm/gtc/type_ptr.hpp>

#include "ui_defs.h"
#include "ui.h"
#include "popupmenu.h"

int ui::popupmenu::get_popup(GLuint t, void *v)
{
    int ret = 0;

    switch (t)
    {
      case ui::popup::visible:  *(bool *)v = this->visible;      break;
      case ui::popup::button:   *(int *)v = this->popup_button;  break;
      default:                  ret = 1;                         break;
    }
    return ret;
}

void ui::popupmenu::set_popup(GLuint t, void *v)
{
    switch (t)
    {
      case ui::popup::visible:  this->visible = *(bool *)v;      break;
      case ui::popup::button:   this->popup_button = *(int *)v;  break;
    }
}

void ui::popupmenu::set_resize(GLuint t, void *v)
{
    /* No-op, since we don't want this to change */
}

void ui::popupmenu::show(ui::event_target *p, void *call, void *client)
{
    ui::popupmenu *pm = (ui::popupmenu *)client;
    ui::btn_call_data *bcd = (ui::btn_call_data *)call;
    bool t = true;

    if (bcd->button == pm->popup_button && bcd->state == ui::mouse::down)
    {
        glm::ivec2 tmp_vec;

        pm->get(ui::element::size, ui::size::all, &tmp_vec);
        tmp_vec = bcd->location - (tmp_vec / 2);
        pm->set_va(ui::element::position, ui::position::all, &tmp_vec,
                   ui::element::popup, ui::popup::visible, &t, 0);
    }
}

void ui::popupmenu::hide(ui::event_target *p, void *call, void *client)
{
    ui::popupmenu *pm = (ui::popupmenu *)client;
    ui::btn_call_data *bcd = (ui::btn_call_data *)call;
    bool f = false;

    if (bcd->button == pm->popup_button && bcd->state == ui::mouse::up)
        pm->set(ui::element::popup, ui::popup::visible, &f);
}

/* We'll use a parametric function to draw our ellipse.
 *
 * <radius.x * cos(theta), radius.y * sin(theta)>
 */
void ui::popupmenu::populate_buffers(void)
{
    glm::vec2 pixel_sz;
    glm::vec2 radius(this->size.x / 2.0f, this->size.y / 2.0f);
    glm::vec2 inner = radius * 0.2f;
    glm::vec2 m0, m3, b0, b3;

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

    int count = std::max(this->size.x / 3, 15);

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

    this->vertex_count = this->element_count = 0;

    for (i = 0; i < count; ++i)
    {
        float angle = increment * i;

        vertex[this->vertex_count] = radius.x * cosf(angle) + this->pos.x;
        vertex[this->vertex_count + 1] = radius.y * sinf(angle) + this->pos.y;
        memcpy(&vertex[this->vertex_count + 2],
               glm::value_ptr(this->background),
               sizeof(float) * 4);
        vertex[this->vertex_count + 6] = ui::panel::no_texture;
        vertex[this->vertex_count + 7] = ui::panel::no_texture;

        vertex[this->vertex_count + 8] = inner.x * cosf(angle) + this->pos.x;
        vertex[this->vertex_count + 9] = inner.y * sinf(angle) + this->pos.y;
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
            vertex[border_index] = m0.x * cosf(angle) + this->pos.x;
            vertex[border_index + 1] = m0.y * sinf(angle) + this->pos.y;
            memcpy(&vertex[border_index + 2],
                   glm::value_ptr(this->foreground),
                   sizeof(float) * 4);
            vertex[border_index + 6] = ui::panel::no_texture;
            vertex[border_index + 7] = ui::panel::no_texture;

            vertex[border_index + 8] = b0.x * cosf(angle) + this->pos.x;
            vertex[border_index + 9] = b0.y * sinf(angle) + this->pos.y;
            memcpy(&vertex[border_index + 10],
                   glm::value_ptr(this->foreground),
                   sizeof(float) * 4);
            vertex[border_index + 14] = ui::panel::no_texture;
            vertex[border_index + 15] = ui::panel::no_texture;
            border_index += 16;
            border_count += 2;

            element[border_element] =  border_count - 2;
            element[border_element + 1] = border_count - 1;
            element[border_element + 2] = border_count + 1;
            element[border_element + 3] = border_count - 2;
            element[border_element + 4] = border_count + 1;
            element[border_element + 5] = border_count;
            border_element += 6;
        }

        /* Inner border */
        if (this->border[3] != 0)
        {
            vertex[border_index] = b3.x * cosf(angle) + this->pos.x;
            vertex[border_index + 1] = b3.y * sinf(angle) + this->pos.y;
            memcpy(&vertex[border_index + 2],
                   glm::value_ptr(this->foreground),
                   sizeof(float) * 4);
            vertex[border_index + 6] = ui::panel::no_texture;
            vertex[border_index + 7] = ui::panel::no_texture;

            vertex[border_index + 8] = m3.x * cosf(angle) + this->pos.x;
            vertex[border_index + 9] = m3.y * sinf(angle) + this->pos.y;
            memcpy(&vertex[border_index + 10],
                   glm::value_ptr(this->foreground),
                   sizeof(float) * 4);
            vertex[border_index + 14] = ui::panel::no_texture;
            vertex[border_index + 15] = ui::panel::no_texture;
            border_index += 16;
            border_count += 2;

            element[border_element] =  border_count - 2;
            element[border_element + 1] = border_count - 1;
            element[border_element + 2] = border_count + 1;
            element[border_element + 3] = border_count - 2;
            element[border_element + 4] = border_count + 1;
            element[border_element + 5] = border_count;
            border_element += 6;
        }
    }

    increment = M_PI * 2.0f / this->children.size();

    /* We'll use the m elements for our divider endpoints.  If they
     * are already set, nice, and if not, we'll set to the relevant
     * edge of the background polys.
     */
    if (m0.x == 0.0f && m0.y == 0.0f)
        m0 = radius;
    if (m3.x == 0.0f && m3.y == 0.0f)
        m3 = inner;

    this->vertex_count = border_index;
    vert_count = border_count;
    this->element_count = border_element;

    for (i = 0; i < this->children.size(); ++i)
    {
        float angle = increment * i;

        /* Add the dividers for each child */
        vertex[this->vertex_count] = m0.x * cosf(angle) + this->pos.x;
        vertex[this->vertex_count + 1] = m0.y * sinf(angle) + this->pos.y;
        memcpy(&vertex[this->vertex_count + 2],
               glm::value_ptr(this->foreground),
               sizeof(float) * 4);
        vertex[this->vertex_count + 6] = ui::panel::no_texture;
        vertex[this->vertex_count + 7] = ui::panel::no_texture;

        vertex[this->vertex_count + 8] = m3.x * cosf(angle) + this->pos.x;
        vertex[this->vertex_count + 9] = m3.y * sinf(angle) + this->pos.y;
        memcpy(&vertex[this->vertex_count + 10],
               glm::value_ptr(this->foreground),
               sizeof(float) * 4);
        vertex[this->vertex_count + 14] = ui::panel::no_texture;
        vertex[this->vertex_count + 15] = ui::panel::no_texture;

        /* Make the dividers one degree wide.  There's probably a nice
         * way to do this so the edges will be parallel, but I don't
         * have a network link to be able to research at the moment.
         */
        angle += M_PI * 2.0f / 360;

        vertex[this->vertex_count + 16] = m0.x * cosf(angle) + this->pos.x;
        vertex[this->vertex_count + 17] = m0.y * sinf(angle) + this->pos.y;
        memcpy(&vertex[this->vertex_count + 18],
               glm::value_ptr(this->foreground),
               sizeof(float) * 4);
        vertex[this->vertex_count + 22] = ui::panel::no_texture;
        vertex[this->vertex_count + 23] = ui::panel::no_texture;

        vertex[this->vertex_count + 24] = m3.x * cosf(angle) + this->pos.x;
        vertex[this->vertex_count + 25] = m3.y * sinf(angle) + this->pos.y;
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

        /* Reposition each child to be in the middle of its sector */
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

ui::popupmenu::popupmenu(composite *c, GLuint w, GLuint h)
    : ui::manager::manager(c, w, h)
{
    ui::context *ctx = dynamic_cast<ui::context *>(c);
    ui::manager *mgr = dynamic_cast<ui::manager *>(c);
    this->popup_button = ui::mouse::button2;
    this->resize = ui::resize::none;

    /* Our parent could be either a UI context, or a regular manager */
    if (ctx != NULL)
        ctx->set(ui::element::popup, ui::popup::menu, this);
    else if (mgr != NULL)
    {
        mgr->add_callback(ui::callback::btn_down, ui::popupmenu::show, this);
        mgr->add_callback(ui::callback::btn_up, ui::popupmenu::hide, this);
    }

    this->prep_vao_vbo(&this->vao, &this->vbo);
    glGenBuffers(1, &this->ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
    this->populate_buffers();
}

ui::popupmenu::~popupmenu()
{
    ui::context *ctx = dynamic_cast<ui::context *>(this->composite::parent);
    ui::manager *mgr = dynamic_cast<ui::manager *>(this->composite::parent);

    /* Our parent could be either a UI context, or a regular manager */
    if (ctx != NULL)
        ctx->set(ui::element::popup, ui::popup::menu, NULL);
    else if (mgr != NULL)
    {
        mgr->remove_callback(ui::callback::btn_down, ui::popupmenu::show, this);
        mgr->remove_callback(ui::callback::btn_up, ui::popupmenu::hide, this);
    }
}

int ui::popupmenu::get(GLuint e, GLuint t, void *v)
{
    if (e == ui::element::popup)
        return this->get_popup(t, v);
    return this->manager::get(e, t, v);
}

void ui::popupmenu::set(GLuint e, GLuint t, void *v)
{
    if (e == ui::element::popup)
        this->set_popup(t, v);
    else
        this->manager::set(e, t, v);
}

void ui::popupmenu::draw(void)
{
    if (this->visible)
        this->panel::draw();
}

void ui::popupmenu::add_child(panel *p)
{
    this->manager::add_child(p);
    this->populate_buffers();
}

void ui::popupmenu::remove_child(panel *p)
{
    this->manager::remove_child(p);
    this->populate_buffers();
}
