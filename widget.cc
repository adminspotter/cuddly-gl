/* widget.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 06 Nov 2016, 09:57:18 tquirk
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
 * This file contains the basic widget object method definitions.
 *
 * Things to do
 *
 */

#include <math.h>

#include <stdexcept>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ui_defs.h"
#include "widget.h"
#include "../l10n.h"

const float ui::vertex_buffer::no_texture = -1000.0;

ui::vertex_buffer::vertex_buffer(GLuint verts, GLuint elts)
{
    this->vertex = new float[verts];
    this->element = new GLuint[elts];
    this->vertex_index = 0;
    this->vertex_count = 0;
    this->element_index = 0;
}

ui::vertex_buffer::~vertex_buffer()
{
    delete[] this->element;
    delete[] this->vertex;
}

void ui::vertex_buffer::generate_box(glm::vec2 ul, glm::vec2 lr,
                                     const glm::vec4& color)
{
    vertex[this->vertex_index] = ul.x;
    vertex[this->vertex_index + 1] = ul.y;
    memcpy(&vertex[this->vertex_index + 2],
           glm::value_ptr(color), sizeof(float) * 4);
    vertex[this->vertex_index + 6] = ui::vertex_buffer::no_texture;
    vertex[this->vertex_index + 7] = ui::vertex_buffer::no_texture;

    vertex[this->vertex_index + 8] = lr.x;
    vertex[this->vertex_index + 9] = vertex[this->vertex_index + 1];
    memcpy(&vertex[this->vertex_index + 10],
           glm::value_ptr(color), sizeof(float) * 4);
    vertex[this->vertex_index + 14] = ui::vertex_buffer::no_texture;
    vertex[this->vertex_index + 15] = ui::vertex_buffer::no_texture;

    vertex[this->vertex_index + 16] = vertex[this->vertex_index];
    vertex[this->vertex_index + 17] = lr.y;
    memcpy(&vertex[this->vertex_index + 18],
           glm::value_ptr(color), sizeof(float) * 4);
    vertex[this->vertex_index + 22] = ui::vertex_buffer::no_texture;
    vertex[this->vertex_index + 23] = ui::vertex_buffer::no_texture;

    vertex[this->vertex_index + 24] = vertex[this->vertex_index + 8];
    vertex[this->vertex_index + 25] = vertex[this->vertex_index + 17];
    memcpy(&vertex[this->vertex_index + 26],
           glm::value_ptr(color), sizeof(float) * 4);
    vertex[this->vertex_index + 30] = ui::vertex_buffer::no_texture;
    vertex[this->vertex_index + 31] = ui::vertex_buffer::no_texture;
    this->vertex_index += 32;
    this->vertex_count += 4;

    element[this->element_index] = this->vertex_count - 4;
    element[this->element_index + 1] = this->vertex_count - 2;
    element[this->element_index + 2] = this->vertex_count - 3;
    element[this->element_index + 3] = this->vertex_count - 2;
    element[this->element_index + 4] = this->vertex_count - 1;
    element[this->element_index + 5] = this->vertex_count - 3;
    this->element_index += 6;
}

/* We'll use a parametric function to draw our ellipse.
 *
 * <radius.x * cos(theta), radius.y * sin(theta)>
 *
 * The inner_pct is a percentage of the entire radius, rather than a
 * raw screen coord value of any kind.  For example, if the radius is
 * (100, 100), and the inner_pct is 0.15, the inner radius is (15,
 * 15).
 *
 * TODO:  There may be some degenerate stuff that occurs with
 * inner_pct of 0.0, so we should test this.
 */
void ui::vertex_buffer::generate_ellipse(glm::vec2 center, glm::vec2 radius,
                                         float inner_pct, int segments,
                                         const glm::vec4& color)
{
    /* Clamp inner_pct and segments to reasonable ranges */
    if (inner_pct < 0.0)  inner_pct = 0.0;
    if (inner_pct >= 1.0) inner_pct = 0.99;

    if (segments < 15)  segments = 15;
    if (segments > 720) segments = 720;

    glm::vec2 inner = radius * inner_pct;
    float increment = M_PI * 2.0f / (float)segments;
    GLuint vertex_start_count = this->vertex_count;

    for (int i = 0; i < segments; ++i)
    {
        float angle = increment * i;

        vertex[this->vertex_index] = radius.x * cosf(angle) + center.x;
        vertex[this->vertex_index + 1] = radius.y * sinf(angle) + center.y;
        memcpy(&vertex[this->vertex_index + 2],
               glm::value_ptr(color), sizeof(float) * 4);
        vertex[this->vertex_index + 6] = ui::vertex_buffer::no_texture;
        vertex[this->vertex_index + 7] = ui::vertex_buffer::no_texture;

        vertex[this->vertex_index + 8] = inner.x * cosf(angle) + center.x;
        vertex[this->vertex_index + 9] = inner.y * sinf(angle) + center.y;
        memcpy(&vertex[this->vertex_index + 10],
               glm::value_ptr(color), sizeof(float) * 4);
        vertex[this->vertex_index + 14] = ui::vertex_buffer::no_texture;
        vertex[this->vertex_index + 15] = ui::vertex_buffer::no_texture;
        this->vertex_index += 16;
        this->vertex_count += 2;

        element[this->element_index] = this->vertex_count - 2;
        element[this->element_index + 1] = this->vertex_count - 1;
        element[this->element_index + 2] = this->vertex_count + 1;
        element[this->element_index + 3] = this->vertex_count - 2;
        element[this->element_index + 4] = this->vertex_count + 1;
        element[this->element_index + 5] = this->vertex_count;
        this->element_index += 6;
    }

    /* Fix up the last segment */
    element[this->element_index - 4] = vertex_start_count + 1;
    element[this->element_index - 2] = vertex_start_count + 1;
    element[this->element_index - 1] = vertex_start_count;
}

void ui::vertex_buffer::generate_ellipse_divider(glm::vec2 center,
                                                 glm::vec2 radius,
                                                 float pct, float angle,
                                                 const glm::vec4& color)
{
    glm::vec2 inner = radius * pct;

    vertex[this->vertex_index] = radius.x * cosf(angle) + center.x;
    vertex[this->vertex_index + 1] = radius.y * sinf(angle) + center.y;
    memcpy(&vertex[this->vertex_index + 2],
           glm::value_ptr(color), sizeof(float) * 4);
    vertex[this->vertex_index + 6] = ui::vertex_buffer::no_texture;
    vertex[this->vertex_index + 7] = ui::vertex_buffer::no_texture;

    vertex[this->vertex_index + 8] = inner.x * cosf(angle) + center.x;
    vertex[this->vertex_index + 9] = inner.y * sinf(angle) + center.y;
    memcpy(&vertex[this->vertex_index + 10],
           glm::value_ptr(color), sizeof(float) * 4);
    vertex[this->vertex_index + 14] = ui::vertex_buffer::no_texture;
    vertex[this->vertex_index + 15] = ui::vertex_buffer::no_texture;

    /* Make the dividers one degree wide.  There's probably a nice way
     * to do this so the edges will be parallel, but these don't look
     * too bad for now.
     */
    angle += M_PI * 2.0f / 360;

    vertex[this->vertex_index + 16] = radius.x * cosf(angle) + center.x;
    vertex[this->vertex_index + 17] = radius.y * sinf(angle) + center.y;
    memcpy(&vertex[this->vertex_index + 18],
           glm::value_ptr(color), sizeof(float) * 4);
    vertex[this->vertex_index + 22] = ui::vertex_buffer::no_texture;
    vertex[this->vertex_index + 23] = ui::vertex_buffer::no_texture;

    vertex[this->vertex_index + 24] = inner.x * cosf(angle) + center.x;
    vertex[this->vertex_index + 25] = inner.y * sinf(angle) + center.y;
    memcpy(&vertex[this->vertex_index + 26],
           glm::value_ptr(color), sizeof(float) * 4);
    vertex[this->vertex_index + 30] = ui::vertex_buffer::no_texture;
    vertex[this->vertex_index + 31] = ui::vertex_buffer::no_texture;
    this->vertex_index += 32;
    this->vertex_count += 4;

    element[this->element_index] = this->vertex_count - 4;
    element[this->element_index + 1] = this->vertex_count - 2;
    element[this->element_index + 2] = this->vertex_count - 3;
    element[this->element_index + 3] = this->vertex_count - 2;
    element[this->element_index + 4] = this->vertex_count - 1;
    element[this->element_index + 5] = this->vertex_count - 3;
    this->element_index += 6;
}

size_t ui::vertex_buffer::vertex_size(void)
{
    return sizeof(float) * this->vertex_index;
}

size_t ui::vertex_buffer::element_size(void)
{
    return sizeof(GLuint) * this->element_index;
}

int ui::widget::get_position(GLuint t, void *v)
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

void ui::widget::set_position(GLuint t, void *v)
{
    GLuint new_v = *((GLuint *)v);

    switch (t)
    {
      case ui::position::all:  this->pos = *(glm::ivec2 *)v;  break;
      case ui::position::x:    this->pos.x = *(int *)v;       break;
      case ui::position::y:    this->pos.y = *(int *)v;       break;
    }

    this->parent->move_child(this);
    this->recalculate_transformation_matrix();
}

int ui::widget::get_state(GLuint t, void *v)
{
    if (t == ui::state::visible)
    {
        *(bool *)v = this->visible;
        return 0;
    }
    return 1;
}

void ui::widget::set_state(GLuint t, void *v)
{
    if (t == ui::state::visible)
    {
        this->visible = *(bool *)v;
        this->parent->move_child(this);
    }
}

int ui::widget::get_border(GLuint t, void *v)
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

void ui::widget::set_border(GLuint t, void *v)
{
    GLuint new_v = *((GLuint *)v);

    if (t & ui::side::top || t & ui::side::bottom)
        if (this->margin[0] + this->margin[3]
            + (t & ui::side::top ? new_v : this->border[0])
            + (t & ui::side::bottom ? new_v : this->border[3]) <= this->dim.y)
        {
            if (t & ui::side::top)     this->border[0] = new_v;
            if (t & ui::side::bottom)  this->border[3] = new_v;
        }

    if (t & ui::side::left || t & ui::side::right)
        if (this->margin[1] + this->margin[2]
            + (t & ui::side::left ? new_v : this->border[1])
            + (t & ui::side::right ? new_v : this->border[2]) <= this->dim.x)
        {
            if (t & ui::side::left)    this->border[1] = new_v;
            if (t & ui::side::right)   this->border[2] = new_v;
        }

    this->populate_buffers();
}

int ui::widget::get_margin(GLuint t, void *v)
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

void ui::widget::set_margin(GLuint t, void *v)
{
    GLuint new_v = *((GLuint *)v);

    if (t & ui::side::top || t & ui::side::bottom)
        if (this->border[0] + this->border[3]
            + (t & ui::side::top ? new_v : this->margin[0])
            + (t & ui::side::bottom ? new_v : this->margin[3]) <= this->dim.y)
        {
            if (t & ui::side::top)     this->margin[0] = new_v;
            if (t & ui::side::bottom)  this->margin[3] = new_v;
        }

    if (t & ui::side::left || t & ui::side::right)
        if (this->border[1] + this->border[2]
            + (t & ui::side::left ? new_v : this->margin[1])
            + (t & ui::side::right ? new_v : this->margin[2]) <= this->dim.x)
        {
            if (t & ui::side::left)    this->margin[1] = new_v;
            if (t & ui::side::right)   this->margin[2] = new_v;
        }

    this->populate_buffers();
}

int ui::widget::get_color(GLuint t, void *v)
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

void ui::widget::set_color(GLuint t, void *v)
{
    if (t & ui::color::foreground)
        memcpy(glm::value_ptr(this->foreground), v, sizeof(float) * 4);

    if (t & ui::color::background)
        memcpy(glm::value_ptr(this->background), v, sizeof(float) * 4);

    this->populate_buffers();
}

void ui::widget::set_size(GLuint t, void *v)
{
    this->rect::set_size(t, v);
    this->populate_buffers();
    this->parent->move_child(this);
}

void ui::widget::recalculate_transformation_matrix(void)
{
    glm::vec3 pixel_sz;
    glm::mat4 new_trans;

    this->parent->get(ui::element::pixel_size, ui::size::all, &pixel_sz);
    pixel_sz.x *= this->pos.x;
    pixel_sz.y = -(pixel_sz.y * this->pos.y);
    this->pos_transform = glm::translate(new_trans, pixel_sz);
}

ui::vertex_buffer *ui::widget::generate_points(void)
{
    ui::vertex_buffer *vb = new ui::vertex_buffer(160, 60);
    float w = this->dim.x, h = this->dim.y, m[4], b[4];
    glm::vec2 psz;

    this->parent->get(ui::element::pixel_size, ui::size::all, &psz);
    psz.y = -psz.y;
    w *= psz.x;
    h *= psz.y;
    m[0] = this->margin[0] * psz.y;  b[0] = this->border[0] * psz.y;
    m[1] = this->margin[1] * psz.x;  b[1] = this->border[1] * psz.x;
    m[2] = this->margin[2] * psz.x;  b[2] = this->border[2] * psz.x;
    m[3] = this->margin[3] * psz.y;  b[3] = this->border[3] * psz.y;

    vb->generate_box(glm::vec2(-1.0f, 1.0f),
                     glm::vec2(-1.0f + w, 1.0f + h),
                     this->background);

    if (this->border[0] != 0)
        vb->generate_box(glm::vec2(vb->vertex[0] + m[1],
                                   vb->vertex[1] + m[0]),
                         glm::vec2(vb->vertex[8] - m[2],
                                   vb->vertex[1] + m[0] + b[0]),
                         this->foreground);
    if (this->border[1] != 0)
        vb->generate_box(glm::vec2(vb->vertex[0] + m[1],
                                   vb->vertex[1] + m[0]),
                         glm::vec2(vb->vertex[0] + m[1] + b[1],
                                   vb->vertex[17] - m[3]),
                         this->foreground);
    if (this->border[2] != 0)
        vb->generate_box(glm::vec2(vb->vertex[8] - m[2] - b[2],
                                   vb->vertex[1] + m[0]),
                         glm::vec2(vb->vertex[8] - m[2],
                                   vb->vertex[17] - m[3]),
                         this->foreground);
    if (this->border[3] != 0)
        vb->generate_box(glm::vec2(vb->vertex[0] + m[1],
                                   vb->vertex[17] - m[3] - b[3]),
                         glm::vec2(vb->vertex[8] - m[2],
                                   vb->vertex[17] - m[3]),
                         this->foreground);

    return vb;
}

void ui::widget::populate_buffers(void)
{
    ui::vertex_buffer *vb = this->generate_points();

    if (vb == NULL)
        return;

    this->element_count = vb->element_index;
    glBindVertexArray(this->vao);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 vb->vertex_size(), vb->vertex,
                 GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 vb->element_size(), vb->element,
                 GL_DYNAMIC_DRAW);
    delete vb;
}

ui::widget::widget(ui::composite *c, GLuint w, GLuint h)
    : ui::active::active(w, h), ui::rect::rect(w, h),
      pos(0, 0), pos_transform(),
      foreground(1.0f, 1.0f, 1.0f, 1.0f), background(0.5f, 0.5f, 0.5f, 1.0f)
{
    GLuint pos_attr, color_attr, texture_attr;

    for (int i = 0; i < 4; ++i)
        this->border[i] = this->margin[i] = 0;

    if (c == NULL)
        throw std::runtime_error(_("Widget must have a parent."));
    this->parent = c;
    this->parent->add_child(this);

    this->parent->get_va(ui::element::attribute,
                         ui::attribute::position,
                         &pos_attr,
                         ui::element::attribute,
                         ui::attribute::color,
                         &color_attr,
                         ui::element::attribute,
                         ui::attribute::texture,
                         &texture_attr, 0);

    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);
    glGenBuffers(1, &this->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glGenBuffers(1, &this->ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
    glEnableVertexAttribArray(pos_attr);
    glVertexAttribPointer(pos_attr, 2, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 8, (void *)0);
    glEnableVertexAttribArray(color_attr);
    glVertexAttribPointer(color_attr, 4, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 8, (void *)(sizeof(float) * 2));
    glEnableVertexAttribArray(texture_attr);
    glVertexAttribPointer(texture_attr, 2, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 8, (void *)(sizeof(float) * 6));

    this->element_count = 0;
    this->visible = true;

    this->populate_buffers();
}

ui::widget::~widget()
{
    this->parent->remove_child(this);
    glDeleteBuffers(1, &this->ebo);
    glDeleteBuffers(1, &this->vbo);
    glDeleteVertexArrays(1, &this->vao);
}

int ui::widget::get(GLuint e, GLuint t, void *v)
{
    int ret;

    switch (e)
    {
      case ui::element::position:  ret = this->get_position(t, v);    break;
      case ui::element::state:     ret = this->get_state(t, v);       break;
      case ui::element::border:    ret = this->get_border(t, v);      break;
      case ui::element::margin:    ret = this->get_margin(t, v);      break;
      case ui::element::color:     ret = this->get_color(t, v);       break;
      default:                     ret = this->active::get(e, t, v);  break;
    }
    return ret;
}

void ui::widget::set(GLuint e, GLuint t, void *v)
{
    switch (e)
    {
      case ui::element::position:  this->set_position(t, v);    break;
      case ui::element::state:     this->set_state(t, v);       break;
      case ui::element::border:    this->set_border(t, v);      break;
      case ui::element::margin:    this->set_margin(t, v);      break;
      case ui::element::color:     this->set_color(t, v);       break;
      default:                     this->active::set(e, t, v);  break;
    }
}

void ui::widget::draw(GLuint trans_uniform, const glm::mat4& parent_trans)
{
    if (this->visible == true)
    {
        glm::mat4 trans = pos_transform * parent_trans;

        glBindVertexArray(this->vao);
        glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
        glUniformMatrix4fv(trans_uniform, 1, GL_FALSE, glm::value_ptr(trans));
        glDrawElements(GL_TRIANGLES, this->element_count, GL_UNSIGNED_INT, 0);
    }
}

void ui::widget::close(void)
{
    this->visible = false;
    this->parent->close_child(this);
}
