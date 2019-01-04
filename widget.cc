/* widget.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 04 Jan 2019, 08:57:09 tquirk
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

const float ui::vertex_buffer::no_texture = -1000.0;

ui::vertex_buffer::vertex_buffer()
    : vertex(), element()
{
}

ui::vertex_buffer::~vertex_buffer()
{
}

void ui::vertex_buffer::generate_box(glm::vec2 ul, glm::vec2 lr,
                                     const glm::vec4& color)
{
    int vert_idx = this->vertex.size(), elt_idx = this->element.size();
    int vert_count = vert_idx / 8;

    this->vertex.insert(this->vertex.end(), 32, 0.0f);
    this->element.insert(this->element.end(), 6, 0);

    vertex[vert_idx] = ul.x;
    vertex[vert_idx + 1] = ul.y;
    memcpy(&vertex[vert_idx + 2], glm::value_ptr(color), sizeof(float) * 4);
    vertex[vert_idx + 6] = ui::vertex_buffer::no_texture;
    vertex[vert_idx + 7] = ui::vertex_buffer::no_texture;

    vertex[vert_idx + 8] = lr.x;
    vertex[vert_idx + 9] = vertex[vert_idx + 1];
    memcpy(&vertex[vert_idx + 10], glm::value_ptr(color), sizeof(float) * 4);
    vertex[vert_idx + 14] = ui::vertex_buffer::no_texture;
    vertex[vert_idx + 15] = ui::vertex_buffer::no_texture;

    vertex[vert_idx + 16] = vertex[vert_idx];
    vertex[vert_idx + 17] = lr.y;
    memcpy(&vertex[vert_idx + 18], glm::value_ptr(color), sizeof(float) * 4);
    vertex[vert_idx + 22] = ui::vertex_buffer::no_texture;
    vertex[vert_idx + 23] = ui::vertex_buffer::no_texture;

    vertex[vert_idx + 24] = vertex[vert_idx + 8];
    vertex[vert_idx + 25] = vertex[vert_idx + 17];
    memcpy(&vertex[vert_idx + 26], glm::value_ptr(color), sizeof(float) * 4);
    vertex[vert_idx + 30] = ui::vertex_buffer::no_texture;
    vertex[vert_idx + 31] = ui::vertex_buffer::no_texture;
    vert_count += 4;

    element[elt_idx] = vert_count - 4;
    element[elt_idx + 1] = vert_count - 2;
    element[elt_idx + 2] = vert_count - 3;
    element[elt_idx + 3] = vert_count - 2;
    element[elt_idx + 4] = vert_count - 1;
    element[elt_idx + 5] = vert_count - 3;
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
    int vert_idx = this->vertex.size(), elt_idx = this->element.size();
    int vert_count = vert_idx / 8, vertex_start_count = vert_count;

    /* Clamp inner_pct and segments to reasonable ranges */
    if (inner_pct < 0.0)  inner_pct = 0.0;
    if (inner_pct >= 1.0) inner_pct = 0.99;

    if (segments < 15)  segments = 15;
    if (segments > 720) segments = 720;

    this->vertex.insert(this->vertex.end(), 16 * segments, 0.0f);
    this->element.insert(this->element.end(), 6 * segments, 0);

    glm::vec2 inner = radius * inner_pct;
    float increment = M_PI * 2.0f / (float)segments;

    for (int i = 0; i < segments; ++i)
    {
        float angle = increment * i;

        vertex[vert_idx] = radius.x * cosf(angle) + center.x;
        vertex[vert_idx + 1] = radius.y * sinf(angle) + center.y;
        memcpy(&vertex[vert_idx + 2],
               glm::value_ptr(color), sizeof(float) * 4);
        vertex[vert_idx + 6] = ui::vertex_buffer::no_texture;
        vertex[vert_idx + 7] = ui::vertex_buffer::no_texture;

        vertex[vert_idx + 8] = inner.x * cosf(angle) + center.x;
        vertex[vert_idx + 9] = inner.y * sinf(angle) + center.y;
        memcpy(&vertex[vert_idx + 10],
               glm::value_ptr(color), sizeof(float) * 4);
        vertex[vert_idx + 14] = ui::vertex_buffer::no_texture;
        vertex[vert_idx + 15] = ui::vertex_buffer::no_texture;
        vert_idx += 16;
        vert_count += 2;

        element[elt_idx] = vert_count - 2;
        element[elt_idx + 1] = vert_count - 1;
        element[elt_idx + 2] = vert_count + 1;
        element[elt_idx + 3] = vert_count - 2;
        element[elt_idx + 4] = vert_count + 1;
        element[elt_idx + 5] = vert_count;
        elt_idx += 6;
    }

    /* Fix up the last segment */
    element[elt_idx - 4] = vertex_start_count + 1;
    element[elt_idx - 2] = vertex_start_count + 1;
    element[elt_idx - 1] = vertex_start_count;
}

void ui::vertex_buffer::generate_ellipse_divider(glm::vec2 center,
                                                 glm::vec2 radius,
                                                 float pct, float angle,
                                                 const glm::vec4& color)
{
    int vert_idx = this->vertex.size(), elt_idx = this->element.size();
    int vert_count = vert_idx / 8;

    this->vertex.insert(this->vertex.end(), 32, 0.0f);
    this->element.insert(this->element.end(), 6, 0);

    glm::vec2 inner = radius * pct;

    vertex[vert_idx] = radius.x * cosf(angle) + center.x;
    vertex[vert_idx + 1] = radius.y * sinf(angle) + center.y;
    memcpy(&vertex[vert_idx + 2], glm::value_ptr(color), sizeof(float) * 4);
    vertex[vert_idx + 6] = ui::vertex_buffer::no_texture;
    vertex[vert_idx + 7] = ui::vertex_buffer::no_texture;

    vertex[vert_idx + 8] = inner.x * cosf(angle) + center.x;
    vertex[vert_idx + 9] = inner.y * sinf(angle) + center.y;
    memcpy(&vertex[vert_idx + 10], glm::value_ptr(color), sizeof(float) * 4);
    vertex[vert_idx + 14] = ui::vertex_buffer::no_texture;
    vertex[vert_idx + 15] = ui::vertex_buffer::no_texture;

    /* Make the dividers one degree wide.  There's probably a nice way
     * to do this so the edges will be parallel, but these don't look
     * too bad for now.
     */
    angle += M_PI * 2.0f / 360;

    vertex[vert_idx + 16] = radius.x * cosf(angle) + center.x;
    vertex[vert_idx + 17] = radius.y * sinf(angle) + center.y;
    memcpy(&vertex[vert_idx + 18], glm::value_ptr(color), sizeof(float) * 4);
    vertex[vert_idx + 22] = ui::vertex_buffer::no_texture;
    vertex[vert_idx + 23] = ui::vertex_buffer::no_texture;

    vertex[vert_idx + 24] = inner.x * cosf(angle) + center.x;
    vertex[vert_idx + 25] = inner.y * sinf(angle) + center.y;
    memcpy(&vertex[vert_idx + 26], glm::value_ptr(color), sizeof(float) * 4);
    vertex[vert_idx + 30] = ui::vertex_buffer::no_texture;
    vertex[vert_idx + 31] = ui::vertex_buffer::no_texture;
    vert_count += 4;

    element[elt_idx] = vert_count - 4;
    element[elt_idx + 1] = vert_count - 2;
    element[elt_idx + 2] = vert_count - 3;
    element[elt_idx + 3] = vert_count - 2;
    element[elt_idx + 4] = vert_count - 1;
    element[elt_idx + 5] = vert_count - 3;
}

const float *ui::vertex_buffer::vertex_data(void)
{
    return this->vertex.data();
}

size_t ui::vertex_buffer::vertex_size(void)
{
    return sizeof(float) * this->vertex.size();
}

const GLuint *ui::vertex_buffer::element_data(void)
{
    return this->element.data();
}

size_t ui::vertex_buffer::element_size(void)
{
    return sizeof(GLuint) * this->element.size();
}

GLuint ui::vertex_buffer::element_count(void)
{
    return this->element.size();
}

int ui::widget::get_position(GLuint t, GLuint *v) const
{
    switch (t)
    {
      case ui::position::x:  *v = this->pos.x;  return 0;
      case ui::position::y:  *v = this->pos.y;  return 0;
      default:                                  return 1;
    }
}

int ui::widget::get_position(GLuint t, glm::ivec2 *v) const
{
    if (t == ui::position::all)
    {
        *v = this->pos;
        return 0;
    }
    return 1;
}

void ui::widget::set_position(GLuint t, GLuint v)
{
    switch (t)
    {
      case ui::position::x:  this->pos.x = v;  break;
      case ui::position::y:  this->pos.y = v;  break;
      default:                                 return;
    }

    this->parent->move_child(this);
    this->recalculate_transformation_matrix();
}

void ui::widget::set_position(GLuint t, const glm::ivec2& v)
{
    if (t == ui::position::all)
    {
        this->pos = v;
        this->parent->move_child(this);
        this->recalculate_transformation_matrix();
    }
}

int ui::widget::get_state(GLuint t, bool *v) const
{
    if (t == ui::state::visible)
    {
        *v = this->visible;
        return 0;
    }
    return 1;
}

void ui::widget::set_state(GLuint t, bool v)
{
    if (t == ui::state::visible)
    {
        this->visible = v;
        this->parent->move_child(this);
    }
}

int ui::widget::get_border(GLuint t, GLuint *v) const
{
    switch (t)
    {
      case ui::side::top:     *v = this->border[0];  return 0;
      case ui::side::left:    *v = this->border[1];  return 0;
      case ui::side::right:   *v = this->border[2];  return 0;
      case ui::side::bottom:  *v = this->border[3];  return 0;
      default:                                       return 1;
    }
}

void ui::widget::set_border(GLuint t, GLuint v)
{
    if (t & ui::side::top)     this->border[0] = v;
    if (t & ui::side::left)    this->border[1] = v;
    if (t & ui::side::right)   this->border[2] = v;
    if (t & ui::side::bottom)  this->border[3] = v;

    this->populate_buffers();
}

int ui::widget::get_margin(GLuint t, GLuint *v) const
{
    switch (t)
    {
      case ui::side::top:     *v = this->margin[0];  return 0;
      case ui::side::left:    *v = this->margin[1];  return 0;
      case ui::side::right:   *v = this->margin[2];  return 0;
      case ui::side::bottom:  *v = this->margin[3];  return 0;
      default:                                       return 1;
    }
}

void ui::widget::set_margin(GLuint t, GLuint v)
{
    if (t & ui::side::top)     this->margin[0] = v;
    if (t & ui::side::left)    this->margin[1] = v;
    if (t & ui::side::right)   this->margin[2] = v;
    if (t & ui::side::bottom)  this->margin[3] = v;

    this->populate_buffers();
}

int ui::widget::get_color(GLuint t, glm::vec4 *v) const
{
    switch (t)
    {
      case ui::color::foreground:  *v = this->foreground;  return 0;
      case ui::color::background:  *v = this->background;  return 0;
      default:                                             return 1;
    }
}

void ui::widget::set_color(GLuint t, const glm::vec4& v)
{
    if (t & ui::color::foreground)  this->foreground = v;
    if (t & ui::color::background)  this->background = v;

    this->populate_buffers();
}

void ui::widget::set_size(GLuint t, GLuint v)
{
    this->rect::set_size(t, v);
    this->populate_buffers();
    this->parent->move_child(this);
}

void ui::widget::set_size(GLuint t, const glm::ivec2& v)
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
    ui::vertex_buffer *vb = new ui::vertex_buffer();
    float w = this->dim.x, h = this->dim.y, m[4], b[4];
    glm::vec3 psz;

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

    this->element_count = vb->element_count();
    glBindVertexArray(this->vao);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 vb->vertex_size(), vb->vertex_data(),
                 GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 vb->element_size(), vb->element_data(),
                 GL_DYNAMIC_DRAW);
    delete vb;
}

void ui::widget::init(ui::composite *c)
{
    GLuint pos_attr = 0, color_attr = 0, texture_attr = 0;

    for (int i = 0; i < 4; ++i)
        this->border[i] = this->margin[i] = 0;

    if (c == NULL)
        throw std::runtime_error("Widget must have a parent.");
    this->parent = c;
    this->parent->add_child(this);

    this->parent->get(ui::element::attribute,
                      ui::attribute::position,
                      &pos_attr,
                      ui::element::attribute,
                      ui::attribute::color,
                      &color_attr,
                      ui::element::attribute,
                      ui::attribute::texture,
                      &texture_attr);

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

ui::widget::widget(ui::composite *c)
    : ui::active::active(0, 0), ui::rect::rect(0, 0),
      pos(0, 0), pos_transform(),
      foreground(1.0f, 1.0f, 1.0f, 1.0f), background(0.5f, 0.5f, 0.5f, 1.0f)
{
    this->init(c);
}

ui::widget::~widget()
{
    this->parent->remove_child(this);
    glDeleteBuffers(1, &this->ebo);
    glDeleteBuffers(1, &this->vbo);
    glDeleteVertexArrays(1, &this->vao);
}

int ui::widget::get(GLuint e, GLuint t, GLuint *v) const
{
    switch (e)
    {
      case ui::element::position:  return this->get_position(t, v);
      case ui::element::border:    return this->get_border(t, v);
      case ui::element::margin:    return this->get_margin(t, v);
      case ui::element::size:      return this->get_size(t, v);
      default:                     return 1;
    }
}

int ui::widget::get(GLuint e, GLuint t, glm::ivec2 *v) const
{
    switch (e)
    {
      case ui::element::position:  return this->get_position(t, v);
      case ui::element::size:      return this->get_size(t, v);
      default:                     return 1;
    }
}

int ui::widget::get(GLuint e, GLuint t, bool *v) const
{
    if (e == ui::element::state)
        return this->get_state(t, v);
    return 1;
}

int ui::widget::get(GLuint e, GLuint t, glm::vec4 *v) const
{
    if (e == ui::element::color)
        return this->get_color(t, v);
    return 1;
}

void ui::widget::set(GLuint e, GLuint t, GLuint v)
{
    switch (e)
    {
      case ui::element::size:      this->set_size(t, v);      break;
      case ui::element::position:  this->set_position(t, v);  break;
      case ui::element::border:    this->set_border(t, v);    break;
      case ui::element::margin:    this->set_margin(t, v);    break;
    }
}

void ui::widget::set(GLuint e, GLuint t, const glm::ivec2& v)
{
    switch (e)
    {
      case ui::element::size:      this->set_size(t, v);      break;
      case ui::element::position:  this->set_position(t, v);  break;
    }
}

void ui::widget::set(GLuint e, GLuint t, bool v)
{
    if (e == ui::element::state)
        this->set_state(t, v);
}

void ui::widget::set(GLuint e, GLuint t, const glm::vec4& v)
{
    if (e == ui::element::color)
        this->set_color(t, v);
}

void ui::widget::draw(GLuint trans_uniform, const glm::mat4& parent_trans)
{
    this->call_timeout();

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
    this->parent->remove_child(this);
}
