/* panel.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 08 Mar 2016, 08:28:03 tquirk
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

#include "panel.h"

void ui::panel::set_size(GLuint d, GLuint v)
{
    if (d & ui::size::height)
    {
        if (this->margin[0] + this->border[0]
            + this->border[3] + this->margin[3] < v)
            this->height = v;
    }
    if (d & ui::size::width)
    {
        if (this->margin[1] + this->border[1]
            + this->border[2] + this->margin[2] < v)
            this->width = v;
    }
}

void ui::panel::set_border(GLuint s, GLuint v)
{
    if (s & ui::side::top & ui::side::bottom)
        if (this->margin[0] + this->margin[3]
            + (s & ui::side::top ? v : this->border[0])
            + (s & ui::side::bottom ? v : this->border[3]) <= this->height)
        {
            if (s & ui::side::top)
                this->border[0] = v;
            if (s & ui::side::bottom)
                this->border[3] = v;
        }

    if (s & ui::side::left & ui::side::right)
        if (this->margin[1] + this->margin[2]
            + (s & ui::side::left ? v : this->border[1])
            + (s & ui::side::right ? v : this->border[2]) <= this->width)
        {
            if (s & ui::side::left)    this->border[1] = v;
            if (s & ui::side::right)   this->border[2] = v;
        }
}

void ui::panel::set_margin(GLuint s, GLuint v)
{
    if (s & ui::side::top & ui::side::bottom)
        if (this->border[0] + this->border[3]
            + (s & ui::side::top ? v : this->margin[0])
            + (s & ui::side::bottom ? v : this->margin[3]) <= this->height)
        {
            if (s & ui::side::top)     this->margin[0] = v;
            if (s & ui::side::bottom)  this->margin[3] = v;
        }

    if (s & ui::side::left & ui::side::right)
        if (this->border[1] + this->border[2]
            + (s & ui::side::left ? v : this->margin[1])
            + (s & ui::side::right ? v : this->margin[2]) <= this->width)
        {
            if (s & ui::side::left)    this->margin[1] = v;
            if (s & ui::side::right)   this->margin[2] = v;
        }
}

void ui::panel::populate_buffers(void)
{
    /* We'll use an element buffer, so we can get away with fewer
     * points.  This will come in very handy in some of the
     * subclasses, which may have tons of duplicated points.
     */
    float vertex[32];
    float x = this->xpos, y = this->ypos, w = this->width, h = this->height;
    float vw = this->parent->get(ui::element::size, ui::size::width);
    float vh = this->parent->get(ui::element::size, ui::size::height);
    GLuint element[6];

    vertex[0] = x / vw;
    vertex[1] = y / vh;
    vertex[2] = vertex[3] = 0.0f;     /* no normal */
    memcpy(&vertex[4], glm::value_ptr(ui::background), sizeof(float) * 4);
    vertex[8] = vertex[0] + (w / vw);
    vertex[9] = vertex[1];
    vertex[10] = vertex[11] = 0.0f;  /* no normal */
    memcpy(&vertex[12], glm::value_ptr(ui::background), sizeof(float) * 4);
    vertex[16] = vertex[0];
    vertex[17] = vertex[1] + (h / vh);
    vertex[18] = vertex[19] = 0.0f;  /* no normal */
    memcpy(&vertex[20], glm::value_ptr(ui::background), sizeof(float) * 4);
    vertex[24] = vertex[8];
    vertex[25] = vertex[17];
    vertex[26] = vertex[27] = 0.0f;  /* no normal */
    memcpy(&vertex[28], glm::value_ptr(ui::background), sizeof(float) * 4);
    element[0] = 0;
    element[1] = 2;
    element[2] = 1;
    element[3] = 2;
    element[4] = 3;
    element[5] = 1;
    this->element_count = 2;

    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(float) * 32, vertex,
                 GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(GLuint) * 6, element,
                 GL_DYNAMIC_DRAW);
}

ui::panel::panel(ui::context *c, GLuint w, GLuint h)
{
    this->set_size(w, h);
    this->set_border(ui::side::all, 0);
    this->set_margin(ui::side::all, 0);

    this->pos_attr = c->get(ui::element::attribute, ui::attribute::position);
    this->norm_attr = c->get(ui::element::attribute, ui::attribute::normal);
    this->color_attr = c->get(ui::element::attribute, ui::attribute::color);

    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);
    glEnableVertexAttribArray(pos_attr);
    glVertexAttribPointer(pos_attr, 2, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 8, (void *)0);
    glEnableVertexAttribArray(norm_attr);
    glVertexAttribPointer(norm_attr, 2, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 8, (void *)(sizeof(float) * 2));
    glEnableVertexAttribArray(color_attr);
    glVertexAttribPointer(color_attr, 4, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 8, (void *)(sizeof(float) * 4));
    glGenBuffers(1, &this->vbo);
    glGenBuffers(1, &this->ebo);
    this->populate_buffers();

    this->parent = c;
    this->parent += this;
}

ui::panel::~panel()
{
    this->parent -= this;
    glDeleteBuffers(1, &this->ebo);
    glDeleteBuffers(1, &this->vbo);
    glDeleteVertexArrays(1, &this->vao);
}

GLuint ui::panel::get(GLuint e, GLuint t)
{
    switch (e)
    {
      case ui::element::size:
        switch (t)
        {
          case ui::size::width:  return this->width;
          case ui::size::height: return this->height;
          default:               return -1;
        }

      case ui::element::border:
        switch (t)
        {
          case ui::side::top:    return this->border[0];
          case ui::side::left:   return this->border[1];
          case ui::side::right:  return this->border[2];
          case ui::side::bottom: return this->border[3];
          default:               return -1;
        }

      case ui::element::margin:
        switch (t)
        {
          case ui::side::top:    return this->margin[0];
          case ui::side::left:   return this->margin[1];
          case ui::side::right:  return this->margin[2];
          case ui::side::bottom: return this->margin[3];
          default:               return -1;
        }
    }
    return -1;
}

void ui::panel::set(GLuint s, GLuint m, GLuint v)
{
    switch (s)
    {
      case ui::element::size:   this->set_size(m, v);   break;
      case ui::element::border: this->set_border(m, v); break;
      case ui::element::margin: this->set_margin(m, v); break;
    }
    this->populate_buffers();
}

void ui::panel::set_va(GLuint s, GLuint m, GLuint v, ...)
{
    va_list args;
    GLuint item[3];

    this->set(s, m, v);
    va_start(args, v);
    if ((item[0] = va_arg(args, GLuint)) != 0)
    {
        item[1] = va_arg(args, GLuint);
        item[2] = va_arg(args, GLuint);
        this->set(item[0], item[1], item[2]);
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
    delete this;
}
