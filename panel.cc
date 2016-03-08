/* panel.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 08 Mar 2016, 17:05:56 tquirk
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

#include "panel.h"

int ui::panel::get_size(GLuint t, void *v)
{
    int ret = 0;

    switch (t)
    {
      case ui::size::width:  *((GLuint *)v) = this->width;  break;
      case ui::size::height: *((GLuint *)v) = this->height; break;
      default:               ret = 1;                       break;
    }
    return ret;
}

void ui::panel::set_size(GLuint d, void *v)
{
    GLuint new_v = *((GLuint *)v);

    if (d & ui::size::height)
    {
        if (this->margin[0] + this->border[0]
            + this->border[3] + this->margin[3] < new_v)
            this->height = new_v;
    }
    if (d & ui::size::width)
    {
        if (this->margin[1] + this->border[1]
            + this->border[2] + this->margin[2] < new_v)
            this->width = new_v;
    }
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

    if (s & ui::side::top & ui::side::bottom)
        if (this->margin[0] + this->margin[3]
            + (s & ui::side::top ? new_v : this->border[0])
            + (s & ui::side::bottom ? new_v : this->border[3]) <= this->height)
        {
            if (s & ui::side::top)     this->border[0] = new_v;
            if (s & ui::side::bottom)  this->border[3] = new_v;
        }

    if (s & ui::side::left & ui::side::right)
        if (this->margin[1] + this->margin[2]
            + (s & ui::side::left ? new_v : this->border[1])
            + (s & ui::side::right ? new_v : this->border[2]) <= this->width)
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

    if (s & ui::side::top & ui::side::bottom)
        if (this->border[0] + this->border[3]
            + (s & ui::side::top ? new_v : this->margin[0])
            + (s & ui::side::bottom ? new_v : this->margin[3]) <= this->height)
        {
            if (s & ui::side::top)     this->margin[0] = new_v;
            if (s & ui::side::bottom)  this->margin[3] = new_v;
        }

    if (s & ui::side::left & ui::side::right)
        if (this->border[1] + this->border[2]
            + (s & ui::side::left ? new_v : this->margin[1])
            + (s & ui::side::right ? new_v : this->margin[2]) <= this->width)
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

void ui::panel::populate_buffers(void)
{
    /* We'll use an element buffer, so we can get away with fewer
     * points.  This will come in very handy in some of the
     * subclasses, which may have tons of duplicated points.
     */
    float vertex[32];
    float x = this->xpos, y = this->ypos, w = this->width, h = this->height;
    float vw, vh;
    GLuint element[6], temp;

    this->parent->get(ui::element::size, ui::size::width, &temp);
    vw = (float)temp;
    this->parent->get(ui::element::size, ui::size::height, &temp);
    vh = (float)temp;

    vertex[0] = x / vw;
    vertex[1] = y / vh;
    vertex[2] = vertex[3] = 0.0f;     /* no normal */
    memcpy(&vertex[4], glm::value_ptr(this->background), sizeof(float) * 4);
    vertex[8] = vertex[0] + (w / vw);
    vertex[9] = vertex[1];
    vertex[10] = vertex[11] = 0.0f;  /* no normal */
    memcpy(&vertex[12], glm::value_ptr(this->background), sizeof(float) * 4);
    vertex[16] = vertex[0];
    vertex[17] = vertex[1] + (h / vh);
    vertex[18] = vertex[19] = 0.0f;  /* no normal */
    memcpy(&vertex[20], glm::value_ptr(this->background), sizeof(float) * 4);
    vertex[24] = vertex[8];
    vertex[25] = vertex[17];
    vertex[26] = vertex[27] = 0.0f;  /* no normal */
    memcpy(&vertex[28], glm::value_ptr(this->background), sizeof(float) * 4);
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
    GLuint pos_attr, norm_attr, color_attr, zero = 0;

    this->set_size(ui::size::width, &w);
    this->set_size(ui::size::height, &h);
    this->set_border(ui::side::all, &zero);
    this->set_margin(ui::side::all, &zero);

    c->get(ui::element::attribute, ui::attribute::position, &pos_attr);
    c->get(ui::element::attribute, ui::attribute::normal, &norm_attr);
    c->get(ui::element::attribute, ui::attribute::color, &color_attr);

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
    *this->parent += this;
}

ui::panel::~panel()
{
    *this->parent -= this;
    glDeleteBuffers(1, &this->ebo);
    glDeleteBuffers(1, &this->vbo);
    glDeleteVertexArrays(1, &this->vao);
}

int ui::panel::get(GLuint e, GLuint t, void *v)
{
    int ret;

    switch (e)
    {
      case ui::element::size:    ret = this->get_size(t, v);   break;
      case ui::element::border:  ret = this->get_border(t, v); break;
      case ui::element::margin:  ret = this->get_margin(t, v); break;
      case ui::element::color:   ret = this->get_color(t, v);  break;
      default:                   ret = 1;                      break;
    }
    return ret;
}

void ui::panel::set(GLuint s, GLuint m, void *v)
{
    switch (s)
    {
      case ui::element::size:    this->set_size(m, v);   break;
      case ui::element::border:  this->set_border(m, v); break;
      case ui::element::margin:  this->set_margin(m, v); break;
      case ui::element::color:   this->set_color(m, v);  break;
    }
    this->populate_buffers();
}

void ui::panel::set_va(GLuint s, GLuint m, void *v, ...)
{
    va_list args;
    GLuint item[2];
    void *ptr;

    this->set(s, m, v);
    va_start(args, v);
    if ((item[0] = va_arg(args, GLuint)) != 0)
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
    delete this;
}
