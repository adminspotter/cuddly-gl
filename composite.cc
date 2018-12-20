/* composite.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 20 Dec 2018, 08:10:18 tquirk
 *
 * CuddlyGL OpenGL widget toolkit
 * Copyright (C) 2018  Trinity Annabelle Quirk
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
 * This file contains the method definitions for the composite object.
 * It's a widget which can have children, which will then be
 * subclassable into more interesting manager widgets.
 *
 * Things to do
 *
 */

#include <algorithm>

#include <glm/vec3.hpp>

#include "composite.h"

const int ui::composite::tree_max_depth = 4;

void ui::composite::set_size(GLuint d, GLuint v)
{
    ui::resize_call_data call_data;

    this->rect::set_size(d, v);
    this->regenerate_children();
    this->regenerate_search_tree();
    call_data.new_size = this->dim;
    for (auto& i : this->children)
        i->call_callbacks(ui::callback::resize, &call_data);
}

void ui::composite::set_size(GLuint d, const glm::ivec2& v)
{
    ui::resize_call_data call_data;

    this->rect::set_size(d, v);
    this->regenerate_children();
    this->regenerate_search_tree();
    call_data.new_size = this->dim;
    for (auto i : this->children)
        i->call_callbacks(ui::callback::resize, &call_data);
}

int ui::composite::get_resize(GLuint t, void *v) const
{
    *reinterpret_cast<GLuint *>(v) = this->resize;
    return 0;
}

void ui::composite::set_resize(GLuint t, GLuint v)
{
    if (v <= ui::resize::all)
        this->resize = v;
}

int ui::composite::get_pixel_size(GLuint t, void *v) const
{
    int ret = 0;

    switch (t)
    {
      case ui::size::all:
        {
            glm::vec3 sz(2.0f / (float)this->dim.x,
                         2.0f / (float)this->dim.y,
                         0.0f);
            *reinterpret_cast<glm::vec3 *>(v) = sz;
            break;
        }

      case ui::size::width:
        *reinterpret_cast<float *>(v) = 2.0f / (float)this->dim.x;
        break;
      case ui::size::height:
        *reinterpret_cast<float *>(v) = 2.0f / (float)this->dim.y;
        break;
      default:
        ret = 1;
        break;
    }
    return ret;
}

void ui::composite::set_desired_size(void)
{
    this->clear_removed_children();
}

void ui::composite::reposition_children(void)
{
    for (auto& i : this->children)
        i->recalculate_transformation_matrix();
}

void ui::composite::regenerate_children(void)
{
    for (auto& i : this->children)
    {
        i->recalculate_transformation_matrix();
        i->populate_buffers();
    }
}

void ui::composite::regenerate_search_tree(void)
{
    glm::ivec2 ul(0, 0);

    if (this->tree != NULL)
        delete this->tree;
    this->tree = new ui::quadtree(NULL,
                                  ul, this->dim,
                                  ui::composite::tree_max_depth);
    for (auto& i : this->children)
        if (i->visible == true)
            this->tree->insert(i);
}

void ui::composite::clear_removed_children(void)
{
    if (this->dirty == true)
    {
        if (this->to_remove.size() != 0)
        {
            for (auto& i : this->to_remove)
            {
                this->children.remove(i);
                this->tree->remove(i);
            }
            this->to_remove.clear();
        }
    }
}

void ui::composite::child_motion(ui::widget *w, GLuint type, glm::ivec2& pos)
{
    glm::ivec2 obj;
    ui::mouse_call_data call_data = {pos};
    ui::composite *c = dynamic_cast<ui::composite *>(w);

    w->get(ui::element::position, ui::position::all, &obj);
    call_data.location -= obj;
    if (c != NULL)
        c->mouse_pos_callback(call_data.location);
    else
        w->call_callbacks(type, &call_data);
}

ui::composite::composite(composite *c, GLuint w, GLuint h)
    : ui::active::active(w, h), ui::rect::rect(w, h),
      children(), to_remove(), old_pos(0, 0)
{
    this->parent = c;
    this->tree = NULL;
    this->old_child = NULL;
    this->dirty = false;
    this->regenerate_search_tree();
}

ui::composite::~composite()
{
    delete this->tree;
    for (auto& i : this->children)
        delete i;
    this->children.clear();
}

int ui::composite::get(GLuint e, GLuint t, void *v) const
{
    switch (e)
    {
      case ui::element::size:        return this->get_size(t, v);
      case ui::element::resize:      return this->get_resize(t, v);
      case ui::element::pixel_size:  return this->get_pixel_size(t, v);
      default:                       return 1;
    }
}

void ui::composite::set(GLuint e, GLuint t, GLuint v)
{
    switch (e)
    {
      case ui::element::size:    this->set_size(t, v);    break;
      case ui::element::resize:  this->set_resize(t, v);  break;
    }
}

void ui::composite::add_child(ui::widget *w)
{
    auto found = std::find(this->children.begin(), this->children.end(), w);
    if (found == this->children.end())
    {
        this->children.push_back(w);
        if (w->visible == true)
            this->tree->insert(w);
        this->dirty = true;
    }
}

void ui::composite::remove_child(ui::widget *w)
{
    auto found = std::find(this->children.begin(), this->children.end(), w);
    if (found != this->children.end())
    {
        this->to_remove.push_back(w);
        this->dirty = true;
    }
}

void ui::composite::move_child(ui::widget *w)
{
    this->tree->remove(w);
    if (w->visible == true)
        this->tree->insert(w);
    this->dirty = true;
}

void ui::composite::manage_children(void)
{
    this->set_desired_size();
}

void ui::composite::mouse_pos_callback(int x, int y)
{
    glm::ivec2 pos = {x, y};
    this->mouse_pos_callback(pos);
}

void ui::composite::mouse_pos_callback(glm::ivec2& pos)
{
    ui::mouse_call_data call_data = {pos};
    ui::widget *w = this->tree->search(pos);

    if (w != NULL)
    {
        if (this->old_child != w)
        {
            if (this->old_child != NULL)
                this->child_motion(this->old_child, ui::callback::leave, pos);
            this->child_motion(w, ui::callback::enter, pos);
        }
        this->child_motion(w, ui::callback::motion, pos);
    }
    else
    {
        if (this->old_child != NULL)
            this->child_motion(this->old_child, ui::callback::leave, pos);
        this->call_callbacks(ui::callback::motion, &call_data);
    }

    /* w might no longer exist at this position.  Let's search again,
     * just to make sure.
     */
    this->old_child = this->tree->search(pos);
    this->old_pos = pos;
}

void ui::composite::mouse_btn_callback(int btn, int state, int mods)
{
    ui::btn_call_data call_data =
        {this->old_pos, (GLuint)btn, (GLuint)state, (GLuint)mods};
    this->mouse_btn_callback(call_data);
}

void ui::composite::mouse_btn_callback(ui::btn_call_data& call_data)
{
    GLuint which = (call_data.state == ui::mouse::up
                    ? ui::callback::btn_up
                    : ui::callback::btn_down);
    ui::widget *w = this->tree->search(this->old_pos);

    if (w != NULL)
    {
        glm::ivec2 obj;
        ui::composite *c = dynamic_cast<ui::composite *>(w);

        w->get(ui::element::position, ui::position::all, &obj);
        call_data.location -= obj;
        if (c != NULL)
            c->mouse_btn_callback(call_data);
        else
            w->call_callbacks(which, &call_data);
    }
    else
        this->call_callbacks(which, &call_data);

    /* w might no longer exist at this position.  Let's search again,
     * just to make sure.
     */
    this->old_child = this->tree->search(this->old_pos);
}

void ui::composite::key_callback(int key, uint32_t c, int state, int mods)
{
    ui::key_call_data call_data = {this->old_pos, c, (GLuint)key,
                                   (GLuint)state, (GLuint)mods};
    this->key_callback(call_data);
}

void ui::composite::key_callback(ui::key_call_data& call_data)
{
    GLuint which = (call_data.state == ui::key::up
                    ? ui::callback::key_up
                    : ui::callback::key_down);
    ui::widget *w = this->tree->search(this->old_pos);

    if (w != NULL)
    {
        glm::ivec2 obj;
        ui::composite *c = dynamic_cast<ui::composite *>(w);

        w->get(ui::element::position, ui::position::all, &obj);
        call_data.location -= obj;
        if (c != NULL)
            c->key_callback(call_data);
        else
            w->call_callbacks(which, &call_data);
    }
    else
        this->call_callbacks(which, &call_data);

    /* w might no longer exist at this position.  Let's search again,
     * just to make sure.
     */
    this->old_child = this->tree->search(this->old_pos);
}
