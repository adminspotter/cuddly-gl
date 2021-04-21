/* manager.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 06 Oct 2019, 07:19:16 tquirk
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
 * This file contains the method definitions for the manager object,
 * which combines the widget with the composite.
 *
 * Things to do
 *
 */

#include <algorithm>

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ui_defs.h"
#include "manager.h"

int ui::manager::get_child_spacing(GLuint t, GLuint *v) const
{
    switch (t)
    {
      case ui::size::width:   *v = this->child_spacing.x;  return 0;
      case ui::size::height:  *v = this->child_spacing.y;  return 0;
      default:                                             return 1;
    }
}

int ui::manager::get_child_spacing(GLuint t, glm::ivec2 *v) const
{
    if (t == ui::size::all)
    {
        *v = this->child_spacing;
        return 0;
    }
    return 1;
}

void ui::manager::set_child_spacing(GLuint t, GLuint v)
{
    switch (t)
    {
      case ui::size::width:   this->child_spacing.x = v;  break;
      case ui::size::height:  this->child_spacing.y = v;  break;
      default:                                            return;
    }
    this->reposition_children();
    this->regenerate_search_tree();
    this->populate_buffers();
}

void ui::manager::set_child_spacing(GLuint t, const glm::ivec2& v)
{
    if (t == ui::size::all)
    {
        this->child_spacing = v;
        this->reposition_children();
        this->regenerate_search_tree();
        this->populate_buffers();
    }
}

void ui::manager::set_resize(GLuint t, GLuint v)
{
    this->composite::set_resize(t, v);
    this->set_desired_size();
}

int ui::manager::get_size(GLuint t, GLuint *v) const
{
    return this->composite::get_size(t, v);
}

int ui::manager::get_size(GLuint t, glm::ivec2 *v) const
{
    return this->composite::get_size(t, v);
}

void ui::manager::set_size(GLuint t, GLuint v)
{
    this->composite::set_size(t, v);
    this->widget::set_size(t, v);
}

void ui::manager::set_size(GLuint t, const glm::ivec2& v)
{
    this->composite::set_size(t, v);
    this->widget::set_size(t, v);
}

int ui::manager::get_pixel_size(GLuint t, float *v) const
{
    if (this->composite::parent != NULL)
        return this->composite::parent->get(ui::element::pixel_size, t, v);
    return 1;
}

int ui::manager::get_pixel_size(GLuint t, glm::vec3 *v) const
{
    if (this->composite::parent != NULL)
        return this->composite::parent->get(ui::element::pixel_size, t, v);
    return 1;
}

int ui::manager::get_state(GLuint t, bool *v) const
{
    if (t == ui::state::radio_box)
        return this->composite::get_state(t, v);
    return this->widget::get_state(t, v);
}

void ui::manager::set_state(GLuint t, bool v)
{
    if (t == ui::state::radio_box)
        this->composite::set_state(t, v);
    else
        this->widget::set_state(t, v);
}

glm::ivec2 ui::manager::calculate_max_point(void)
{
    glm::ivec2 max_pt(0, 0);

    for (auto& i : this->children)
    {
        glm::ivec2 c_sz, c_pos;

        i->get(ui::element::size, ui::size::all, &c_sz,
               ui::element::position, ui::position::all, &c_pos);
        c_sz += c_pos;
        max_pt.x = std::max(max_pt.x, c_sz.x);
        max_pt.y = std::max(max_pt.y, c_sz.y);
    }
    return max_pt;
}

void ui::manager::set_desired_size(void)
{
    glm::ivec2 old_sz(this->dim);
    glm::ivec2 max_pt(0, 0);

    this->composite::set_desired_size();

    if (this->resize == ui::resize::none)
        return;

    max_pt = this->calculate_max_point();
    max_pt.x += this->margin[1] + this->margin[2]
        + this->border[1] + this->border[2]
        + this->child_spacing.x;
    max_pt.y += this->margin[0] + this->margin[3]
        + this->border[0] + this->border[3]
        + this->child_spacing.y;

    /* We could possibly shrink one dimension and grow another, so
     * we'll handle both cases directly (i.e. no 'else').
     */
    if (this->resize & ui::resize::shrink)
    {
        if (max_pt.x < this->dim.x)
            this->dim.x = max_pt.x;
        if (max_pt.y < this->dim.y)
            this->dim.y = max_pt.y;
    }
    if (this->resize & ui::resize::grow)
    {
        if (max_pt.x > this->dim.x)
            this->dim.x = max_pt.x;
        if (max_pt.y > this->dim.y)
            this->dim.y = max_pt.y;
    }
    if (this->dim != old_sz)
    {
        ui::resize_call_data call_data = { this->dim };
        this->call_callbacks(ui::callback::resize, &call_data);
    }
    this->dirty = false;
    this->regenerate_search_tree();
    this->populate_buffers();
}

/* When the UI context resizes, the flow comes through this function,
 * which is why we do our child regeneration.  It's not likely the
 * ideal way to do it, as it seems a little bit magic, but we'll at
 * least note it here, so somebody knows what's going on.
 */
void ui::manager::recalculate_transformation_matrix(void)
{
    this->widget::recalculate_transformation_matrix();
    this->composite::regenerate_children();
}

void ui::manager::init(ui::composite *c)
{
    this->resize = ui::resize::all;
}

ui::manager::manager(ui::composite *c)
    : ui::widget::widget(c), ui::composite::composite(c),
      ui::active::active(0, 0), ui::rect::rect(0, 0), child_spacing(0, 0)
{
    this->init(c);
}

ui::manager::~manager()
{
}

int ui::manager::get(GLuint e, GLuint t, GLuint *v) const
{
    switch (e)
    {
        /* Eventually, the context will be somebody's parent */
      case ui::element::attribute:
        if (this->composite::parent != NULL)
            return this->widget::parent->get(e, t, v);
        return 1;
      case ui::element::child_spacing:  return this->get_child_spacing(t, v);
      case ui::element::resize:         return this->composite::get(e, t, v);
      default:                          return this->widget::get(e, t, v);
    }
    return 1;
}

int ui::manager::get(GLuint e, GLuint t, glm::ivec2 *v) const
{
    if (e == ui::element::child_spacing)
        return this->get_child_spacing(t, v);
    return this->widget::get(e, t, v);
}

int ui::manager::get(GLuint e, GLuint t, bool *v) const
{
    if (t == ui::state::radio_box)
        return this->composite::get(e, t, v);
    return this->widget::get(e, t, v);
}

void ui::manager::set(GLuint e, GLuint t, GLuint v)
{
    switch (e)
    {
      case ui::element::child_spacing:  this->set_child_spacing(t, v);  break;
      case ui::element::resize:         this->composite::set(e, t, v);  break;
      default:                          this->widget::set(e, t, v);     break;
    }
}

void ui::manager::set(GLuint e, GLuint t, const glm::ivec2& v)
{
    this->widget::set(e, t, v);
}

void ui::manager::set(GLuint e, GLuint t, int v)
{
    this->widget::set(e, t, v);
}

void ui::manager::set(GLuint e, GLuint t, bool v)
{
    if (t == ui::state::radio_box)
        this->composite::set(e, t, v);
    else
        this->widget::set(e, t, v);
}

void ui::manager::draw(GLuint trans_uniform, const glm::mat4& parent_trans)
{
    if (this->dirty == true)
        this->set_desired_size();

    if (this->visible == true)
    {
        glm::mat4 trans = this->pos_transform * parent_trans;

        this->widget::draw(trans_uniform, parent_trans);
        for (auto& i : this->children)
            i->draw(trans_uniform, trans);
    }
}
