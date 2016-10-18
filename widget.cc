/* widget.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 18 Oct 2016, 09:17:02 tquirk
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

#include <stdexcept>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ui_defs.h"
#include "widget.h"
#include "../l10n.h"

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

    /* Recalculate position translation matrix */
    glm::vec3 pixel_sz;
    glm::mat4 new_trans;

    this->parent->get(ui::element::pixel_size, ui::size::all, &pixel_sz);
    pixel_sz.x *= this->pos.x;
    pixel_sz.y = -(pixel_sz.y * this->pos.y);
    this->pos_transform = glm::translate(new_trans, pixel_sz);
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
}

void ui::widget::set_size(GLuint t, void *v)
{
    this->rect::set_size(t, v);
    this->populate_buffers();
    this->parent->move_child(this);
}

void ui::widget::generate_points(void)
{
}

void ui::widget::populate_buffers(void)
{
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

    this->vertex_count = this->element_count = 0;
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
