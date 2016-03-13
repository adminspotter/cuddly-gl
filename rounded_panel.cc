/* rounded_panel.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 13 Mar 2016, 06:21:43 tquirk
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
 * This file contains the object method definitions for the panel with
 * rounded corners.
 *
 * Things to do
 *
 */

#include "rounded_panel.h"

#include <glm/gtc/type_ptr.hpp>

int ui::rounded_panel::get_radius(GLuint t, void *v)
{
    int ret = 0;

    switch (t)
    {
      case ui::corner::top_left:
        *((GLuint *)v) = (this->radius[0] ? this->radius_val : 0);
        break;

      case ui::corner::top_right:
        *((GLuint *)v) = (this->radius[1] ? this->radius_val : 0);
        break;

      case ui::corner::bottom_left:
        *((GLuint *)v) = (this->radius[2] ? this->radius_val : 0);
        break;

      case ui::corner::bottom_right:
        *((GLuint *)v) = (this->radius[3] ? this->radius_val : 0);
        break;

      default:
        ret = 1;
        break;
    }
    return ret;
}

void ui::rounded_panel::set_radius(GLuint t, void *v)
{
    GLuint new_v = *((GLuint *)v);

    if (new_v <= std::min(this->width / 2, this->height / 2))
    {
        if (t & ui::corner::top_left)
            this->radius[0] = (new_v == 0 ? false : true);
        if (t & ui::corner::top_right)
            this->radius[1] = (new_v == 0 ? false : true);
        if (t & ui::corner::bottom_left)
            this->radius[2] = (new_v == 0 ? false : true);
        if (t & ui::corner::bottom_right)
            this->radius[3] = (new_v == 0 ? false : true);
        if (t == ui::corner::all || new_v != 0)
            this->radius_val = new_v;
    }
}

void ui::rounded_panel::populate_buffers(void)
{
    float vertex[12 * 6];
    float x = this->xpos, y = this->ypos, w = this->width, h = this->height;
    float rad = this->radius_val;
    float vw, vh;
    GLuint element[14 * 3], temp;
    GLuint vert_count = 0, vert_index = 0;

    this->parent->get(ui::element::size, ui::size::width, &temp);
    vw = (float)temp / 2.0f;
    this->parent->get(ui::element::size, ui::size::height, &temp);
    vh = (float)temp / -2.0f;

    /* The main part of the panel */
    /* Upper left */
    vertex[0]
        = (x + (this->radius[0] || this->radius[2] ? rad : 0)) / vw - 1.0f;
    vertex[1]
        = (y + (this->radius[0] || this->radius[1] ? rad : 0)) / vh + 1.0f;
    memcpy(&vertex[2], glm::value_ptr(this->background), sizeof(float) * 4);

    /* Upper right */
    vertex[6]
        = (x + w - (this->radius[1] || this->radius[3] ? rad : 0)) / vw - 1.0f;
    vertex[7] = vertex[1];
    memcpy(&vertex[8], glm::value_ptr(this->background), sizeof(float) * 4);

    /* Lower left */
    vertex[12] = vertex[0];
    vertex[13]
        = (y + h - (this->radius[2] || this->radius[3] ? rad : 0)) / vh + 1.0f;
    memcpy(&vertex[14], glm::value_ptr(this->background), sizeof(float) * 4);

    /* Lower right */
    vertex[18] = vertex[6];
    vertex[19] = vertex[13];
    memcpy(&vertex[20], glm::value_ptr(this->background), sizeof(float) * 4);
    vert_count += 4;
    vert_index = 24;

    element[0] = 0;
    element[1] = 1;
    element[2] = 2;
    element[3] = 2;
    element[4] = 3;
    element[5] = 1;
    this->element_count = 6;

    if (this->radius[0] || this->radius[1])
    {
        /* Top */
        vertex[vert_index] = vertex[0];
        vertex[vert_index + 1] = y / vh + 1.0f;
        memcpy(&vertex[vert_index + 2],
               glm::value_ptr(this->background),
               sizeof(float) * 4);
        vertex[vert_index + 6] = vertex[6];
        vertex[vert_index + 7] = vertex[vert_index + 1];
        memcpy(&vertex[vert_index + 8],
               glm::value_ptr(this->background),
               sizeof(float) * 4);
        vert_count += 2;
        vert_index += 12;
        element[this->element_count] = vert_count - 2;
        element[this->element_count + 1] = 0;
        element[this->element_count + 2] = vert_count - 1;
        element[this->element_count + 3] = 0;
        element[this->element_count + 4] = 1;
        element[this->element_count + 5] = vert_count - 1;
        this->element_count += 6;
    }
    if (this->radius[0] || this->radius[2])
    {
        /* Left */
        vertex[vert_index] = x / vw - 1.0f;
        vertex[vert_index + 1] = vertex[1];
        memcpy(&vertex[vert_index + 2],
               glm::value_ptr(this->background),
               sizeof(float) * 4);
        vertex[vert_index + 6] = vertex[vert_index];
        vertex[vert_index + 7] = vertex[13];
        memcpy(&vertex[vert_index + 8],
               glm::value_ptr(this->background),
               sizeof(float) * 4);
        vert_count += 2;
        vert_index += 12;
        element[this->element_count] = vert_count - 2;
        element[this->element_count + 1] = vert_count - 1;
        element[this->element_count + 2] = 0;
        element[this->element_count + 3] = vert_count - 1;
        element[this->element_count + 4] = 2;
        element[this->element_count + 5] = 0;
        this->element_count += 6;
        if (this->radius[0])
        {
            element[this->element_count] = 0;
            element[this->element_count + 1] = 4;
            element[this->element_count + 2] = 6;
            this->element_count += 3;
        }
    }
    if (this->radius[1] || this->radius[3])
    {
        /* Right */
        vertex[vert_index] = (x + w) / vw - 1.0f;
        vertex[vert_index + 1] = vertex[1];
        memcpy(&vertex[vert_index + 2],
               glm::value_ptr(this->background),
               sizeof(float) * 4);
        vertex[vert_index + 6] = vertex[vert_index];
        vertex[vert_index + 7] = vertex[13];
        memcpy(&vertex[vert_index + 8],
               glm::value_ptr(this->background),
               sizeof(float) * 4);
        vert_count += 2;
        vert_index += 12;
        element[this->element_count] = 1;
        element[this->element_count + 1] = 3;
        element[this->element_count + 2] = vert_count - 2;
        element[this->element_count + 3] = 3;
        element[this->element_count + 4] = vert_count - 1;
        element[this->element_count + 5] = vert_count - 2;
        this->element_count += 6;
        if (this->radius[1])
        {
            element[this->element_count] = 5;
            element[this->element_count + 1] = 1;
            element[this->element_count + 2] = vert_count - 2;
            this->element_count += 3;
        }
    }
    if (this->radius[2] || this->radius[3])
    {
        /* Bottom */
        vertex[vert_index] = vertex[0];
        vertex[vert_index + 1] = (y + h) / vh + 1.0f;
        memcpy(&vertex[vert_index + 2],
               glm::value_ptr(this->background),
               sizeof(float) * 4);
        vertex[vert_index + 6] = vertex[6];
        vertex[vert_index + 7] = vertex[vert_index + 1];
        memcpy(&vertex[vert_index + 8],
               glm::value_ptr(this->background),
               sizeof(float) * 4);
        vert_count += 2;
        vert_index += 12;
        element[this->element_count] = 2;
        element[this->element_count + 1] = vert_count - 2;
        element[this->element_count + 2] = 3;
        element[this->element_count + 3] = vert_count - 2;
        element[this->element_count + 4] = vert_count - 1;
        element[this->element_count + 5] = 3;
        this->element_count += 6;
        if (this->radius[2])
        {
            element[this->element_count] = vert_count - 2;
            element[this->element_count + 1] = 2;
            element[this->element_count + 2] = vert_count
                - (this->radius[1] || this->radius[3] ? 5 : 3);
            this->element_count += 3;
        }
        if (this->radius[3])
        {
            element[this->element_count] = vert_count - 3;
            element[this->element_count + 1] = 3;
            element[this->element_count + 2] = vert_count - 1;
            this->element_count += 3;
        }
    }

    glBindVertexArray(this->vao);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(float) * vert_index, vertex,
                 GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(GLuint) * this->element_count, element,
                 GL_DYNAMIC_DRAW);
}

ui::rounded_panel::rounded_panel(ui::context *c, GLuint w, GLuint h)
    : ui::panel::panel(c, w, h)
{
    for (int i = 0; i < 4; ++i)
        this->radius[i] = false;
    this->radius_val = 0.0f;
}

ui::rounded_panel::~rounded_panel()
{
}

int ui::rounded_panel::get(GLuint e, GLuint t, void *v)
{
    int ret = 0;

    if (e == ui::element::radius)
        return this->get_radius(t, v);
    return ui::panel::get(e, t, v);
}

void ui::rounded_panel::set(GLuint s, GLuint m, void *v)
{
    if (s == ui::element::radius)
    {
        this->set_radius(m, v);
        this->populate_buffers();
    }
    else
        ui::panel::set(s, m, v);
}
