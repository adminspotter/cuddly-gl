/* widget.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 12 Oct 2016, 08:27:17 tquirk
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
    : ui::active(w, h), pos(0, 0), pos_transform()
{
    GLuint pos_attr, color_attr, texture_attr;

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
}

void ui::widget::set(GLuint e, GLuint t, void *v)
{
}

void ui::widget::draw(GLuint trans_uniform, const glm::mat4& parent_trans)
{
}
