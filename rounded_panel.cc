/* rounded_panel.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 08 Mar 2016, 08:18:01 tquirk
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

void ui::rounded_panel::set_radius(GLuint c, GLuint v)
{
    if (v <= min(this->width / 2, this->height / 2))
    {
        if (c & ui::corner::top_left)
            this->radius[0] = (v == 0 ? false : true);
        if (c & ui::corner::top_right)
            this->radius[1] = (v == 0 ? false : true);
        if (c & ui::corner::bottom_left)
            this->radius[2] = (v == 0 ? false : true);
        if (c & ui::corner::bottom_right)
            this->radius[3] = (v == 0 ? false : true);
        if (c == ui::corner::all || v != 0)
            this->radius_val = v;
    }
}

void ui::rounded_panel::populate_buffers(void)
{
    float vertex[96];
    float x = this->xpos, y = this->ypos, w = this->width, h = this->height;
    float rad = this->radius_val;
    float vw = this->parent->get(ui::element::size, ui::size::width);
    float vh = this->parent->get(ui::element::size, ui::size::height);
    GLuint element[34];
    GLuint vert_count = 0, vert_index = 0, elem_index = 0;

    /* The main part of the panel */
    /* Upper left */
    vertex[0] = (x + (this->radius[0] || this->radius[2] ? rad : 0)) / vw;
    vertex[1] = (y + (this->radius[0] || this->radius[1] ? rad : 0)) / vh;
    vertex[2] = vertex[3] = 0.0f;     /* no normal */
    memcpy(&vertex[4], glm::value_ptr(ui::background), sizeof(float) * 4);

    /* Upper right */
    vertex[8] = (x + w - (this->radius[1] || this->radius[3] ? rad : 0)) / vw;
    vertex[9] = vertex[1];
    vertex[10] = vertex[11] = 0.0f;  /* no normal */
    memcpy(&vertex[12], glm::value_ptr(ui::background), sizeof(float) * 4);

    /* Lower left */
    vertex[16] = vertex[0];
    vertex[17] = (y + h - (this->radius[2] || this->radius[3] ? rad : 0)) / vh;
    vertex[18] = vertex[19] = 0.0f;  /* no normal */
    memcpy(&vertex[20], glm::value_ptr(ui::background), sizeof(float) * 4);

    /* Lower right */
    vertex[24] = vertex[8];
    vertex[25] = vertex[17];
    vertex[26] = vertex[27] = 0.0f;  /* no normal */
    memcpy(&vertex[28], glm::value_ptr(ui::background), sizeof(float) * 4);
    vert_count += 4;
    vert_index = 32;

    element[0] = 0;
    element[1] = 1;
    element[2] = 2;
    element[3] = 2;
    element[4] = 3;
    element[5] = 1;
    this->elem_count = 2;
    elem_index = 6;

    if (this->radius[0] || this->radius[1])
    {
        /* Top */
        vertex[vert_index] = vertex[0];
        vertex[vert_index + 1] = y / vh;
        vertex[vert_index + 2] = 0.0f;
        vertex[vert_index + 3] = 1.0f;
        memcpy(&vertex[vert_index + 4],
               glm::value_ptr(ui::background),
               sizeof(float) * 4);
        vertex[vert_index + 8] = vertex[8];
        vertex[vert_index + 9] = vertex[vert_index + 1];
        vertex[vert_index + 10] = 0.0f;
        vertex[vert_index + 11] = 1.0f;
        memcpy(&vertex[vert_index + 12],
               glm::value_ptr(ui::background),
               sizeof(float) * 4);
        vert_count += 2;
        vert_index += 16;
        element[elem_index] = vert_count - 2;
        element[elem_index + 1] = 0;
        element[elem_index + 2] = vert_count - 1;
        element[elem_index + 3] = 0;
        element[elem_index + 4] = 1;
        element[elem_index + 5] = vert_count - 1;
        this->elem_count += 2;
        elem_index += 6;
    }
    if (this->radius[0] || this->radius[2])
    {
        /* Left */
        vertex[vert_index] = x / vw;
        vertex[vert_index + 1] = vertex[1];
        vertex[vert_index + 2] = -1.0f;
        vertex[vert_index + 3] = 0.0f;
        memcpy(&vertex[vert_index + 4],
               glm::value_ptr(ui::background),
               sizeof(float) * 4);
        vertex[vert_index + 8] = vertex[vert_index];
        vertex[vert_index + 9] = vertex[13];
        vertex[vert_index + 10] = -1.0f;
        vertex[vert_index + 11] = 0.0f;
        memcpy(&vertex[vert_index + 12],
               glm::value_ptr(ui::background),
               sizeof(float) * 4);
        vert_count += 2;
        vert_index += 16;
        element[elem_index] = vert_count - 2;
        element[elem_index + 1] = vert_count - 1;
        element[elem_index + 2] = 0;
        element[elem_index + 3] = vert_count - 1;
        element[elem_index + 4] = 2;
        element[elem_index + 5] = 0;
        this->elem_count += 2;
        elem_index += 6;
        if (this->radius[0])
        {
            element[elem_index] = 0;
            element[elem_index + 1] = 4;
            element[elem_index + 2] = 6;
            this->elem_count += 1;
            elem_index += 3;
        }
    }
    if (this->radius[1] || this->radius[3])
    {
        /* Right */
        vertex[vert_index] = (x + w) / vw;
        vertex[vert_index + 1] = vertex[1];
        vertex[vert_index + 2] = 1.0f;
        vertex[vert_index + 3] = 0.0f;
        memcpy(&vertex[vert_index + 4],
               glm::value_ptr(ui::background),
               sizeof(float) * 4);
        vertex[vert_index + 8] = vertex[vert_index];
        vertex[vert_index + 9] = vertex[13];
        vertex[vert_index + 10] = 1.0f;
        vertex[vert_index + 11] = 0.0f;
        memcpy(&vertex[vert_index + 12],
               glm::value_ptr(ui::background),
               sizeof(float) * 4);
        vert_count += 2;
        vert_index += 16;
        element[elem_index] = 1;
        element[elem_index + 1] = 3;
        element[elem_index + 2] = vert_count - 2;
        element[elem_index + 3] = 3;
        element[elem_index + 4] = vert_count - 1;
        element[elem_index + 5] = vert_count - 2;
        this->elem_count += 2;
        elem_index += 6;
        if (this->radius[1])
        {
            element[elem_index] = 5;
            element[elem_index + 1] = 1;
            element[elem_index + 2] = vert_count - 2;
            this->elem_count += 1;
            elem_index += 3;
        }
    }
    if (this->radius[2] || this->radius[3])
    {
        /* Bottom */
        vertex[vert_index] = vertex[0];
        vertex[vert_index + 1] = (y + h) / vh;
        vertex[vert_index + 2] = 0.0f;
        vertex[vert_index + 3] = -1.0f;
        memcpy(&vertex[vert_index + 4],
               glm::value_ptr(ui::background),
               sizeof(float) * 4);
        vertex[vert_index + 8] = vertex[8];
        vertex[vert_index + 9] = vertex[vert_index + 1];
        vertex[vert_index + 10] = 0.0f;
        vertex[vert_index + 11] = -1.0f;
        memcpy(&vertex[vert_index + 12],
               glm::value_ptr(ui::background),
               sizeof(float) * 4);
        vert_count += 2;
        vert_index += 16;
        element[elem_index] = 2;
        element[elem_index + 1] = vert_count - 2;
        element[elem_index + 2] = 3;
        element[elem_index + 3] = vert_count - 2;
        element[elem_index + 4] = vert_count - 1;
        element[elem_index + 5] = 3;
        this->elem_count += 2;
        elem_index += 6;
        if (this->radius[2])
        {
            element[elem_index] = vert_count - 2;
            element[elem_index + 1] = 2;
            element[elem_index + 2] = vert_count
                - (this->radius[1] || this->radius[3] ? 5 : 3);
            this->elem_count += 1;
            elem_index += 3;
        }
        if (this->radius[3])
        {
            element[elem_index] = vert_count - 3;
            element[elem_index + 1] = 3;
            element[elem_index + 2] = vert_count - 1;
            this->elem_count += 1;
            elem_index += 3;
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(float) * vert_index, vertex,
                 GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(GLuint) * elem_index, element,
                 GL_DYNAMIC_DRAW);
}

ui::rounded_panel::rounded_panel(ui::context *c, GLuint w, GLuint h)
    : ui::panel::panel(c, w, h)
{
    this->set_radius(ui::corner::all, 0);
}

ui::rounded_panel::~rounded_panel()
{
}

GLuint ui::rounded_panel::get(GLuint e, GLuint t)
{
    if (e == ui::element::radius)
        switch (t)
        {
          case ui::corner::top_left:
            return (this->radius[0] ? this->radius_val : 0);
          case ui::corner::top_right:
            return (this->radius[1] ? this->radius_val : 0);
          case ui::corner::bottom_left:
            return (this->radius[2] ? this->radius_val : 0);
          case ui::corner::bottom_right:
            return (this->radius[3] ? this->radius_val : 0);
          default:
            return -1;
        }
    return ui::panel::get(e, t);
}

void ui::rounded_panel::set(GLuint s, GLuint m, GLuint v)
{
    if (s == ui::element::radius)
        this->set_radius(m, v);
    else
        ui::panel::set(s, m, v);
}
