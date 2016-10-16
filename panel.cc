/* panel.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 15 Oct 2016, 14:23:19 tquirk
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
 * This file contains the basic panel object method definitions.
 *
 * Things to do
 *
 */

#include <stdarg.h>

#include <GL/gl.h>
#include <GL/glext.h>

#include <glm/gtc/type_ptr.hpp>

#include "ui_defs.h"
#include "panel.h"

const float ui::panel::no_texture = -1000.0;

int ui::panel::get_position(GLuint t, void *v)
{
    int ret = 0;

    switch (t)
    {
      case ui::position::all: *((glm::ivec2 *)v) = this->pos;  break;
      case ui::position::x:   *(int *)v = this->pos.x;         break;
      case ui::position::y:   *(int *)v = this->pos.y;         break;
      default:                ret = 1;                         break;
    }
    return ret;
}

void ui::panel::set_position(GLuint t, void *v)
{
    GLuint new_v = *((GLuint *)v);

    switch (t)
    {
      case ui::position::all:  this->pos = *(glm::ivec2 *)v;  break;
      case ui::position::x:    this->pos.x = *(int *)v;       break;
      case ui::position::y:    this->pos.y = *(int *)v;       break;
    }
    this->parent->move_child(this);
}

int ui::panel::get_size(GLuint t, void *v)
{
    int ret = 0;

    switch (t)
    {
      case ui::size::all:    *(glm::ivec2 *)v = this->size;  break;
      case ui::size::width:  *(int *)v = this->size.x;       break;
      case ui::size::height: *(int *)v = this->size.y;       break;
      default:               ret = 1;                        break;
    }
    return ret;
}

void ui::panel::set_size(GLuint d, void *v)
{
    glm::ivec2 new_sz = this->dim;

    if (d == ui::size::all)
        new_sz = *(glm::ivec2 *)v;
    else if (d == ui::size::width)
        new_sz.x = *(int *)v;
    else if (d == ui::size::height)
        new_sz.y = *(int *)v;
    else
        return;

    if ((d == ui::size::height || d == ui::size::all)
        && this->dim.x != new_sz.x)
        if (this->margin[0] + this->border[0]
            + this->border[3] + this->margin[3] < new_sz.x)
            this->dim.x = new_sz.x;
    if ((d == ui::size::width || d == ui::size::all)
        && this->dim.y != new_sz.y)
        if (this->margin[1] + this->border[1]
            + this->border[2] + this->margin[2] < new_sz.y)
            this->dim.y = new_sz.y;

    this->parent->move_child(this);
}

int ui::panel::get_border(GLuint t, void *v)
{
    int ret = 0;

    switch (t)
    {
      case ui::side::top:    *((GLuint *)v) = this->border[0]; break;
      case ui::side::left:   *((GLuint *)v) = this->border[1]; break;
      case ui::side::right:  *((GLuint *)v) = this->border[2]; break;
      case ui::side::bottom: *((GLuint *)v) = this->border[3]; break;
      default:               ret = 1;                          break;
    }
    return ret;
}

void ui::panel::set_border(GLuint s, void *v)
{
    GLuint new_v = *((GLuint *)v);

    if (s & ui::side::top || s & ui::side::bottom)
        if (this->margin[0] + this->margin[3]
            + (s & ui::side::top ? new_v : this->border[0])
            + (s & ui::side::bottom ? new_v : this->border[3]) <= this->size.y)
        {
            if (s & ui::side::top)     this->border[0] = new_v;
            if (s & ui::side::bottom)  this->border[3] = new_v;
        }

    if (s & ui::side::left || s & ui::side::right)
        if (this->margin[1] + this->margin[2]
            + (s & ui::side::left ? new_v : this->border[1])
            + (s & ui::side::right ? new_v : this->border[2]) <= this->size.x)
        {
            if (s & ui::side::left)    this->border[1] = new_v;
            if (s & ui::side::right)   this->border[2] = new_v;
        }
}

int ui::panel::get_margin(GLuint t, void *v)
{
    int ret = 0;

    switch (t)
    {
      case ui::side::top:    *((GLuint *)v) = this->margin[0]; break;
      case ui::side::left:   *((GLuint *)v) = this->margin[1]; break;
      case ui::side::right:  *((GLuint *)v) = this->margin[2]; break;
      case ui::side::bottom: *((GLuint *)v) = this->margin[3]; break;
      default:               ret = 1;                          break;
    }
    return ret;
}

void ui::panel::set_margin(GLuint s, void *v)
{
    GLuint new_v = *((GLuint *)v);

    if (s & ui::side::top || s & ui::side::bottom)
        if (this->border[0] + this->border[3]
            + (s & ui::side::top ? new_v : this->margin[0])
            + (s & ui::side::bottom ? new_v : this->margin[3]) <= this->size.y)
        {
            if (s & ui::side::top)     this->margin[0] = new_v;
            if (s & ui::side::bottom)  this->margin[3] = new_v;
        }

    if (s & ui::side::left || s & ui::side::right)
        if (this->border[1] + this->border[2]
            + (s & ui::side::left ? new_v : this->margin[1])
            + (s & ui::side::right ? new_v : this->margin[2]) <= this->size.x)
        {
            if (s & ui::side::left)    this->margin[1] = new_v;
            if (s & ui::side::right)   this->margin[2] = new_v;
        }
}

int ui::panel::get_color(GLuint t, void *v)
{
    int ret = 0;

    switch (t)
    {
      case ui::color::foreground:
        memcpy(v, glm::value_ptr(this->foreground), sizeof(float) * 4);
        break;

      case ui::color::background:
        memcpy(v, glm::value_ptr(this->background), sizeof(float) * 4);
        break;

      default:
        ret = 1;
        break;
    }
    return ret;
}

void ui::panel::set_color(GLuint s, void *v)
{
    if (s & ui::color::foreground)
        memcpy(glm::value_ptr(this->foreground), v, sizeof(float) * 4);

    if (s & ui::color::background)
        memcpy(glm::value_ptr(this->background), v, sizeof(float) * 4);
}

void ui::panel::prep_vao_vbo(GLuint *vao, GLuint *vbo)
{
    GLuint pos_attr, color_attr, texture_attr;

    this->parent->get(ui::element::attribute,
                      ui::attribute::position,
                      &pos_attr);
    this->parent->get(ui::element::attribute,
                      ui::attribute::color,
                      &color_attr);
    this->parent->get(ui::element::attribute,
                      ui::attribute::texture,
                      &texture_attr);

    glGenVertexArrays(1, vao);
    glBindVertexArray(*vao);
    glGenBuffers(1, vbo);
    glBindBuffer(GL_ARRAY_BUFFER, *vbo);
    glEnableVertexAttribArray(pos_attr);
    glVertexAttribPointer(pos_attr, 2, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 8, (void *)0);
    glEnableVertexAttribArray(color_attr);
    glVertexAttribPointer(color_attr, 4, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 8, (void *)(sizeof(float) * 2));
    glEnableVertexAttribArray(texture_attr);
    glVertexAttribPointer(texture_attr, 2, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 8, (void *)(sizeof(float) * 6));
}

/* The input parameters for this method should be at least 160
 * elements for the vertex buffer, and at least 60 elements for the
 * element buffer.
 */
void ui::panel::generate_points(float *vertex, GLuint *element)
{
    /* We'll use an element buffer, so we can get away with fewer
     * points.  This will come in very handy in some of the
     * subclasses, which may have tons of duplicated points.
     */
    float x = this->pos.x, y = this->pos.y, w = this->dim.x, h = this->dim.y;
    float pw, ph, m[4], b[4];
    GLuint vert_count = 0, temp;

    this->parent->get(ui::element::pixel_size, ui::size::width, &pw);
    this->parent->get(ui::element::pixel_size, ui::size::height, &ph);
    ph = -ph;
    m[0] = this->margin[0] * ph;  b[0] = this->border[0] * ph;
    m[1] = this->margin[1] * pw;  b[1] = this->border[1] * pw;
    m[2] = this->margin[2] * pw;  b[2] = this->border[2] * pw;
    m[3] = this->margin[3] * ph;  b[3] = this->border[3] * ph;

    vertex[0] = x * pw - 1.0f;
    vertex[1] = y * ph + 1.0f;
    memcpy(&vertex[2], glm::value_ptr(this->background), sizeof(float) * 4);
    vertex[6] = vertex[7] = ui::panel::no_texture;

    vertex[8] = vertex[0] + (w * pw);
    vertex[9] = vertex[1];
    memcpy(&vertex[10], glm::value_ptr(this->background), sizeof(float) * 4);
    vertex[14] = vertex[15] = ui::panel::no_texture;

    vertex[16] = vertex[0];
    vertex[17] = vertex[1] + (h * ph);
    memcpy(&vertex[18], glm::value_ptr(this->background), sizeof(float) * 4);
    vertex[22] = vertex[23] = ui::panel::no_texture;

    vertex[24] = vertex[8];
    vertex[25] = vertex[17];
    memcpy(&vertex[26], glm::value_ptr(this->background), sizeof(float) * 4);
    vertex[30] = vertex[31] = ui::panel::no_texture;
    this->vertex_count = 32;
    vert_count = 4;

    element[0] = 0;
    element[1] = 2;
    element[2] = 1;
    element[3] = 2;
    element[4] = 3;
    element[5] = 1;
    this->element_count = 6;

    /* Top border */
    if (this->border[0] != 0)
    {
        vertex[this->vertex_count] = vertex[0] + m[1];
        vertex[this->vertex_count + 1] = vertex[1] + m[0];
        memcpy(&vertex[this->vertex_count + 2],
               glm::value_ptr(this->foreground), sizeof(float) * 4);
        vertex[this->vertex_count + 6] = ui::panel::no_texture;
        vertex[this->vertex_count + 7] = ui::panel::no_texture;

        vertex[this->vertex_count + 8] = vertex[8] - m[2];
        vertex[this->vertex_count + 9] = vertex[this->vertex_count + 1];
        memcpy(&vertex[this->vertex_count + 10],
               glm::value_ptr(this->foreground), sizeof(float) * 4);
        vertex[this->vertex_count + 14] = ui::panel::no_texture;
        vertex[this->vertex_count + 15] = ui::panel::no_texture;

        vertex[this->vertex_count + 16] = vertex[this->vertex_count];
        vertex[this->vertex_count + 17] = vertex[1] + m[0] + b[0];
        memcpy(&vertex[this->vertex_count + 18],
               glm::value_ptr(this->foreground), sizeof(float) * 4);
        vertex[this->vertex_count + 22] = ui::panel::no_texture;
        vertex[this->vertex_count + 23] = ui::panel::no_texture;

        vertex[this->vertex_count + 24] = vertex[this->vertex_count + 8];
        vertex[this->vertex_count + 25] = vertex[this->vertex_count + 17];
        memcpy(&vertex[this->vertex_count + 26],
               glm::value_ptr(this->foreground), sizeof(float) * 4);
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
    }

    /* Left border */
    if (this->border[1] != 0)
    {
        vertex[this->vertex_count] = vertex[0] + m[1];
        vertex[this->vertex_count + 1] = vertex[1] + m[0];
        memcpy(&vertex[this->vertex_count + 2],
               glm::value_ptr(this->foreground), sizeof(float) * 4);
        vertex[this->vertex_count + 6] = ui::panel::no_texture;
        vertex[this->vertex_count + 7] = ui::panel::no_texture;

        vertex[this->vertex_count + 8] = vertex[this->vertex_count] + b[1];
        vertex[this->vertex_count + 9] = vertex[this->vertex_count + 1];
        memcpy(&vertex[this->vertex_count + 10],
               glm::value_ptr(this->foreground), sizeof(float) * 4);
        vertex[this->vertex_count + 14] = ui::panel::no_texture;
        vertex[this->vertex_count + 15] = ui::panel::no_texture;

        vertex[this->vertex_count + 16] = vertex[this->vertex_count];
        vertex[this->vertex_count + 17] = vertex[17] - m[3];
        memcpy(&vertex[this->vertex_count + 18],
               glm::value_ptr(this->foreground), sizeof(float) * 4);
        vertex[this->vertex_count + 22] = ui::panel::no_texture;
        vertex[this->vertex_count + 23] = ui::panel::no_texture;

        vertex[this->vertex_count + 24] = vertex[this->vertex_count + 8];
        vertex[this->vertex_count + 25] = vertex[this->vertex_count + 17];
        memcpy(&vertex[this->vertex_count + 26],
               glm::value_ptr(this->foreground), sizeof(float) * 4);
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
    }

    /* Right border */
    if (this->border[2] != 0)
    {
        vertex[this->vertex_count] = vertex[8] - m[2] - b[2];
        vertex[this->vertex_count + 1] = vertex[1] + m[0];
        memcpy(&vertex[this->vertex_count + 2],
               glm::value_ptr(this->foreground), sizeof(float) * 4);
        vertex[this->vertex_count + 6] = ui::panel::no_texture;
        vertex[this->vertex_count + 7] = ui::panel::no_texture;

        vertex[this->vertex_count + 8] = vertex[this->vertex_count] + b[2];
        vertex[this->vertex_count + 9] = vertex[this->vertex_count + 1];
        memcpy(&vertex[this->vertex_count + 10],
               glm::value_ptr(this->foreground), sizeof(float) * 4);
        vertex[this->vertex_count + 14] = ui::panel::no_texture;
        vertex[this->vertex_count + 15] = ui::panel::no_texture;

        vertex[this->vertex_count + 16] = vertex[this->vertex_count];
        vertex[this->vertex_count + 17] = vertex[17] - m[3];
        memcpy(&vertex[this->vertex_count + 18],
               glm::value_ptr(this->foreground), sizeof(float) * 4);
        vertex[this->vertex_count + 22] = ui::panel::no_texture;
        vertex[this->vertex_count + 23] = ui::panel::no_texture;

        vertex[this->vertex_count + 24] = vertex[this->vertex_count + 8];
        vertex[this->vertex_count + 25] = vertex[this->vertex_count + 17];
        memcpy(&vertex[this->vertex_count + 26],
               glm::value_ptr(this->foreground), sizeof(float) * 4);
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
    }

    /* Bottom border */
    if (this->border[3] != 0)
    {
        vertex[this->vertex_count] = vertex[0] + m[1];
        vertex[this->vertex_count + 1] = vertex[17] - m[3] - b[3];
        memcpy(&vertex[this->vertex_count + 2],
               glm::value_ptr(this->foreground), sizeof(float) * 4);
        vertex[this->vertex_count + 6] = ui::panel::no_texture;
        vertex[this->vertex_count + 7] = ui::panel::no_texture;

        vertex[this->vertex_count + 8] = vertex[8] - m[2];
        vertex[this->vertex_count + 9] = vertex[this->vertex_count + 1];
        memcpy(&vertex[this->vertex_count + 10],
               glm::value_ptr(this->foreground), sizeof(float) * 4);
        vertex[this->vertex_count + 14] = ui::panel::no_texture;
        vertex[this->vertex_count + 15] = ui::panel::no_texture;

        vertex[this->vertex_count + 16] = vertex[this->vertex_count];
        vertex[this->vertex_count + 17] = vertex[17] - m[3];
        memcpy(&vertex[this->vertex_count + 18],
               glm::value_ptr(this->foreground), sizeof(float) * 4);
        vertex[this->vertex_count + 22] = ui::panel::no_texture;
        vertex[this->vertex_count + 23] = ui::panel::no_texture;

        vertex[this->vertex_count + 24] = vertex[this->vertex_count + 8];
        vertex[this->vertex_count + 25] = vertex[this->vertex_count + 17];
        memcpy(&vertex[this->vertex_count + 26],
               glm::value_ptr(this->foreground), sizeof(float) * 4);
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
    }
}

void ui::panel::populate_buffers(void)
{
    float vertex[160];
    GLuint element[60];

    this->generate_points(vertex, element);
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

ui::panel::panel(ui::composite *c, GLuint w, GLuint h)
    : foreground(1.0f, 1.0f, 1.0f, 1.0f), background(0.5f, 0.5f, 0.5f, 1.0f),
      ui::rect::rect(w, h), pos(0, 0), ui::active::active(w, h)
{
    GLuint temp, x, y;

    this->to_close = false;
    this->parent = c;

    for (int i = 0; i < 4; ++i)
    {
        this->border[i] = 0;
        this->margin[i] = 0;
    }

    this->parent->add_child(this);

    this->prep_vao_vbo(&this->vao, &this->vbo);
    glGenBuffers(1, &this->ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
    this->populate_buffers();
}

ui::panel::~panel()
{
    this->parent->remove_child(this);
    glDeleteBuffers(1, &this->ebo);
    glDeleteBuffers(1, &this->vbo);
    glDeleteVertexArrays(1, &this->vao);
}

int ui::panel::get(GLuint e, GLuint t, void *v)
{
    int ret;

    switch (e)
    {
      case ui::element::size:      ret = this->get_size(t, v);      break;
      case ui::element::border:    ret = this->get_border(t, v);    break;
      case ui::element::margin:    ret = this->get_margin(t, v);    break;
      case ui::element::color:     ret = this->get_color(t, v);     break;
      case ui::element::position:  ret = this->get_position(t, v);  break;
      default:                     ret = 1;                         break;
    }
    return ret;
}

void ui::panel::set(GLuint s, GLuint m, void *v)
{
    switch (s)
    {
      case ui::element::size:      this->set_size(m, v);      break;
      case ui::element::border:    this->set_border(m, v);    break;
      case ui::element::margin:    this->set_margin(m, v);    break;
      case ui::element::color:     this->set_color(m, v);     break;
      case ui::element::position:  this->set_position(m, v);  break;
    }
    this->populate_buffers();
}

void ui::panel::get_va(GLuint s, GLuint m, void *v, ...)
{
    va_list args;
    GLuint item[2];
    void *ptr;

    this->get(s, m, v);
    va_start(args, v);
    while ((item[0] = va_arg(args, GLuint)) != 0)
    {
        item[1] = va_arg(args, GLuint);
        ptr = va_arg(args, void *);
        this->get(item[0], item[1], ptr);
    }
    va_end(args);
}

void ui::panel::set_va(GLuint s, GLuint m, void *v, ...)
{
    va_list args;
    GLuint item[2];
    void *ptr;

    this->set(s, m, v);
    va_start(args, v);
    while ((item[0] = va_arg(args, GLuint)) != 0)
    {
        item[1] = va_arg(args, GLuint);
        ptr = va_arg(args, void *);
        this->set(item[0], item[1], ptr);
    }
    va_end(args);
}

void ui::panel::draw(void)
{
    glBindVertexArray(this->vao);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
    glDrawElements(GL_TRIANGLES, this->element_count, GL_UNSIGNED_INT, 0);
}

void ui::panel::close(void)
{
    this->to_close = true;
}
