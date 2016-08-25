/* popupmenu.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 25 Aug 2016, 13:14:56 tquirk
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

void ui::popupmenu::show(ui::panel *p, void *call, void *client)
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

void ui::popupmenu::hide(ui::panel *p, void *call, void *client)
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
    int i, border_index = count * 16, border_count = 0;

    this->composite::parent->get(ui::element::pixel_size,
                                 ui::size::all,
                                 &pixel_sz);
    radius.x *= pixel_sz.x;  inner.x *= pixel_sz.x;
    b0.x *= pixel_sz.x;      m0.x *= pixel_sz.x;
    b3.x *= pixel_sz.x;      m3.x *= pixel_sz.x;
    radius.y *= pixel_sz.y;  inner.y *= pixel_sz.y;
    b0.y *= pixel_sz.y;      m0.y *= pixel_sz.y;
    b3.y *= pixel_sz.y;      m3.y *= pixel_sz.y;

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

        element[this->element_count] = i * 2;
        element[this->element_count + 1] = i * 2 + 1;
        element[this->element_count + 2] = (i * 2 + 3) % count;
        element[this->element_count + 3] = element[this->element_count];
        element[this->element_count + 4] = element[this->element_count + 2];
        element[this->element_count + 5] = (i * 2 + 2) % count;

        this->vertex_count += 16;
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
            border_count += 6;
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
            border_count += 6;
        }
    }

    increment = M_PI * 2.0f / this->children.size();
    for (i = 0; i < this->children.size(); ++i)
    {
        float angle = increment * i;

        /* Add the dividers for each child */
        /* Reposition each child to be in the middle of its sector */
    }
}

ui::popupmenu::popupmenu(composite *c, GLuint w, GLuint h)
    : ui::manager::manager(c, w, h)
{
    ui::context *ctx = dynamic_cast<ui::context *>(c);
    ui::manager *mgr = dynamic_cast<ui::manager *>(c);
    this->popup_button = ui::mouse::button2;

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
