/* composite.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 03 Nov 2019, 15:25:43 tquirk
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
#include "toggle.h"

const int ui::composite::tree_max_depth = 4;

int ui::composite::get_radio_state(bool *v) const
{
    *v = this->radio_box;
    return 0;
}

void ui::composite::set_radio_state(bool v)
{
    this->radio_box = v;
}

int ui::composite::get_radio_child(ui::widget **v) const
{
    if (this->radio_box == true)
    {
        bool checked;
        for (auto i : this->children)
        {
            ui::toggle *r = dynamic_cast<ui::toggle *>(i);
            if (r == NULL)
                continue;
            r->get(ui::element::state, ui::state::checked, &checked);
            if (checked == true)
            {
                *v = i;
                break;
            }
        }
        return 0;
    }
    return 1;
}

void ui::composite::set_radio_child(ui::widget *v)
{
    auto found = std::find(this->children.begin(), this->children.end(), v);
    if (this->radio_box == true && found != this->children.end())
    {
        ui::toggle *set_radio = dynamic_cast<ui::toggle *>(v);
        for (auto i : this->children)
        {
            ui::toggle *r = dynamic_cast<ui::toggle *>(i);
            if (r != NULL && r != set_radio)
                r->set(ui::element::state, ui::state::checked, false);
        }
    }
}

int ui::composite::get_focused_child(ui::widget **v) const
{
    *v = (this->focused == this->children.end() ? NULL : *this->focused);
    return 0;
}

void ui::composite::set_focused_child(ui::widget *w)
{
    if (*this->focused == w
        || (w == NULL && this->focused == this->children.end()))
        return;

    std::list<widget *>::iterator new_focus
        = std::find(this->children.begin(), this->children.end(), w);
    this->focus_child(new_focus);
    if (this->parent != NULL && w == NULL)
        this->parent->set(ui::element::child, ui::child::focused, w);
}

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

int ui::composite::get_resize(GLuint t, GLuint *v) const
{
    *v = this->resize;
    return 0;
}

void ui::composite::set_resize(GLuint t, GLuint v)
{
    if (v <= ui::resize::all)
        this->resize = v;
}

int ui::composite::get_pixel_size(GLuint t, float *v) const
{
    switch (t)
    {
      case ui::size::width:   *v = 2.0f / (float)this->dim.x;  return 0;
      case ui::size::height:  *v = 2.0f / (float)this->dim.y;  return 0;
      default:                                                 return 1;
    }
}

int ui::composite::get_pixel_size(GLuint t, glm::vec3 *v) const
{
    if (t == ui::size::all)
    {
        v->x = 2.0f / (float)this->dim.x;
        v->y = 2.0f / (float)this->dim.y;
        v->z = 0.0f;
        return 0;
    }
    return 1;
}

int ui::composite::get_state(GLuint t, bool *v) const
{
    if (t == ui::state::radio_box)
        return this->get_radio_state(v);
    return 1;
}

void ui::composite::set_state(GLuint t, bool v)
{
    if (t == ui::state::radio_box)
        this->set_radio_state(v);
}

int ui::composite::get_child(GLuint t, ui::widget **v) const
{
    switch (t)
    {
      case ui::child::radio:    return this->get_radio_child(v);
      case ui::child::focused:  return this->get_focused_child(v);
      default:                  return 1;
    }
}

void ui::composite::set_child(GLuint t, ui::widget *v)
{
    switch (t)
    {
      case ui::child::radio:    this->set_radio_child(v);    break;
      case ui::child::focused:  this->set_focused_child(v);  break;
    }
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

void ui::composite::focus_child(std::list<widget *>::iterator new_focus)
{
    if (new_focus != this->focused)
    {
        ui::focus_call_data fcd;

        if (this->focused != this->children.end())
        {
            fcd.focus = false;
            (*this->focused)->call_callbacks(ui::callback::focus, &fcd);
        }
        this->focused = new_focus;
        if (this->focused != this->children.end())
        {
            fcd.focus = true;
            (*this->focused)->call_callbacks(ui::callback::focus, &fcd);
        }
    }
}

void ui::composite::focus_next_child(void)
{
    std::list<widget *>::iterator new_focus = this->focused;

    if (this->focused == this->children.end()
        || ++new_focus == this->children.end())
        new_focus = this->children.begin();
    this->focus_child(new_focus);
}

void ui::composite::focus_previous_child(void)
{
    std::list<widget *>::iterator new_focus = this->focused;

    if (new_focus == this->children.begin())
        new_focus = this->children.end();
    --new_focus;
    this->focus_child(new_focus);
}

void ui::composite::focus_callback(ui::active *a, void *call, void *client)
{
    ui::composite *c = dynamic_cast<ui::composite *>(a);

    if (c != NULL && ((ui::focus_call_data *)call)->focus == false)
        c->set_focused_child(NULL);
}

void ui::composite::init(ui::composite *c)
{
    this->parent = c;
    this->tree = NULL;
    this->focused = this->children.end();
    this->old_child = NULL;
    this->dirty = false;
    this->regenerate_search_tree();

    this->add_callback(ui::callback::focus,
                       ui::composite::focus_callback,
                       NULL);
}

ui::composite::composite(composite *c)
    : ui::active::active(0, 0), ui::rect::rect(0, 0),
      children(), to_remove(), focused(), old_pos(0, 0)
{
    this->init(c);
}

ui::composite::~composite()
{
    delete this->tree;
    for (auto& i : this->children)
        delete i;
    this->children.clear();
}

int ui::composite::get(GLuint e, GLuint t, GLuint *v) const
{
    switch (e)
    {
      case ui::element::size:        return this->get_size(t, v);
      case ui::element::resize:      return this->get_resize(t, v);
      default:                       return 1;
    }
}

int ui::composite::get(GLuint e, GLuint t, float *v) const
{
    if (e == ui::element::pixel_size)
        return this->get_pixel_size(t, v);
    return 1;
}

int ui::composite::get(GLuint e, GLuint t, glm::vec3 *v) const
{
    if (e == ui::element::pixel_size)
        return this->get_pixel_size(t, v);
    return 1;
}

int ui::composite::get(GLuint e, GLuint t, bool *v) const
{
    if (e == ui::element::state)
        return this->get_state(t, v);
    return 1;
}

int ui::composite::get(GLuint e, GLuint t, ui::widget **v) const
{
    if (e == ui::element::child)
        return this->get_child(t, v);
    return 1;
}

void ui::composite::set(GLuint e, GLuint t, GLuint v)
{
    switch (e)
    {
      case ui::element::size:    this->set_size(t, v);    break;
      case ui::element::resize:  this->set_resize(t, v);  break;
    }
}

void ui::composite::set(GLuint e, GLuint t, bool v)
{
    if (e == ui::element::state)
        this->set_state(t, v);
}

void ui::composite::set(GLuint e, GLuint t, ui::widget *v)
{
    if (e == ui::element::child)
        this->set_child(t, v);
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
        if ((*this->focused) == w)
            this->focused = this->children.end();
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

    if (which == ui::callback::btn_down)
        this->set_focused_child(w);
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
    ui::composite *c = NULL;

    if (this->focused != this->children.end())
        c = dynamic_cast<ui::composite *>(*this->focused);

    if (call_data.key == ui::key::tab && call_data.state == ui::key::down)
    {
        if (c != NULL)
            c->key_callback(call_data);
        else
        {
            if (call_data.mods & ui::key_mod::shift)
                this->focus_previous_child();
            else
                this->focus_next_child();
        }
    }
    else if (this->focused != this->children.end())
    {
        glm::ivec2 obj;

        (*this->focused)->get(ui::element::position, ui::position::all, &obj);
        call_data.location -= obj;
        if (c != NULL)
            c->key_callback(call_data);
        else
            (*this->focused)->call_callbacks(which, &call_data);
    }
    else
        this->call_callbacks(which, &call_data);
}
