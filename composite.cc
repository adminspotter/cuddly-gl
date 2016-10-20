/* composite.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 10 Oct 2016, 09:01:43 tquirk
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
 * This file contains the method definitions for the composite object.
 * It's a widget which can have children, which will then be
 * subclassable into more interesting manager widgets.
 *
 * Things to do
 *
 */

#include <algorithm>

#include <glm/vec3.hpp>

#include "ui_defs.h"
#include "composite.h"

const int ui::composite::tree_max_depth = 4;

int ui::composite::get_size(GLuint t, void *v)
{
    int ret = 0;

    switch (t)
    {
      case ui::size::all:    *(glm::ivec2 *)v = this->dim;  break;
      case ui::size::width:  *(int *)v = this->dim.x;       break;
      case ui::size::height: *(int *)v = this->dim.y;       break;
      default:               ret = 1;                       break;
    }
    return ret;
}

void ui::composite::set_size(GLuint d, void *v)
{
    switch (d)
    {
      case ui::size::all:     this->dim = *(glm::ivec2 *)v;  break;
      case ui::size::width:   this->dim.x = *(int *)v;       break;
      case ui::size::height:  this->dim.y = *(int *)v;       break;
    }

    /* Regenerate our search tree */
    glm::ivec2 ul = {0, 0};
    if (this->tree != NULL)
        delete this->tree;
    this->tree = new ui::quadtree(NULL,
                                  ul, this->dim,
                                  ui::composite::tree_max_depth);
    for (auto i = this->children.begin(); i != this->children.end(); ++i)
        this->tree->insert(*i);
}

int ui::composite::get_resize(GLuint t, void *v)
{
    *(GLuint *)v = this->resize;
    return 0;
}

void ui::composite::set_resize(GLuint t, void *v)
{
    GLuint new_v = *((GLuint *)v);

    if (new_v <= ui::resize::all)
        this->resize = new_v;
}

int ui::composite::get_pixel_size(GLuint t, void *v)
{
    int ret = 0;

    switch (t)
    {
      case ui::size::all:
        {
            glm::vec3 sz(2.0f / (float)this->dim.x,
                         2.0f / (float)this->dim.y,
                         0.0f);
            *(glm::vec3 *)v = sz;
            break;
        }

      case ui::size::width:   *(float *)v = 2.0f / (float)this->dim.x;  break;
      case ui::size::height:  *(float *)v = 2.0f / (float)this->dim.y;  break;
      default:                ret = 1;
    }
    return ret;
}

void ui::composite::close_pending(void)
{
    auto child = this->children.begin();

    while (child != this->children.end())
        /* If we delete the pointer and erase(), we get an apparent
         * double-free segfault, so it seems(?) that erase() actually
         * deletes pointers that it owns?
         */
        if ((*child)->to_close == true)
            child = this->children.erase(child);
        else
            ++child;
}

ui::composite::composite(composite *c, GLuint w, GLuint h)
    : ui::rect::rect(w, h), children(), old_pos(0, 0)
{
    int nothing = 0;

    this->parent = c;
    this->tree = NULL;
    this->old_child = NULL;
    this->set_size(0, &nothing);
}

ui::composite::~composite()
{
    delete this->tree;

    while (!this->children.empty())
        delete this->children.front();
}

int ui::composite::get(GLuint e, GLuint t, void *v)
{
    switch (e)
    {
      case ui::element::size:        return this->get_size(t, v);
      case ui::element::resize:      return this->get_resize(t, v);
      case ui::element::pixel_size:  return this->get_pixel_size(t, v);
      default:                       return 1;
    }
}

void ui::composite::set(GLuint e, GLuint t, void *v)
{
    switch (e)
    {
      case ui::element::size:    this->set_size(t, v);    break;
      case ui::element::resize:  this->set_resize(t, v);  break;
      default:                   return;
    }

    for (auto i = this->children.begin(); i != this->children.end(); ++i)
        (*i)->populate_buffers();
}

void ui::composite::add_child(ui::widget *w)
{
    auto found = std::find(this->children.begin(), this->children.end(), w);
    if (found == this->children.end())
    {
        this->children.push_back(w);
        this->tree->insert(w);
    }
}

void ui::composite::remove_child(ui::widget *w)
{
    this->children.remove(w);
    this->tree->remove(w);
}

void ui::composite::move_child(ui::widget *w)
{
    this->tree->remove(w);
    this->tree->insert(w);
}

void ui::composite::mouse_pos_callback(int x, int y)
{
    glm::ivec2 pos = {x, y}, obj;
    ui::mouse_call_data call_data;
    ui::widget *w = this->tree->search(pos);

    if (w != NULL)
    {
        w->get(ui::element::position, ui::position::all, &obj);
        call_data.location = pos - obj;
        if (this->old_child != w)
            w->call_callbacks(ui::callback::enter, &call_data);
        if (this->old_child != NULL && this->old_child != w)
            this->old_child->call_callbacks(ui::callback::leave, &call_data);
        w->call_callbacks(ui::callback::motion, &call_data);
    }
    else if (this->old_child != NULL)
    {
        this->old_child->get(ui::element::position, ui::position::all, &obj);
        call_data.location = pos - obj;
        this->old_child->call_callbacks(ui::callback::leave, &call_data);
        this->close_pending();
    }

    /* w might no longer exist at this position.  Let's search again,
     * just to make sure.
     */
    this->old_child = this->tree->search(pos);
    this->old_pos = pos;
}

void ui::composite::mouse_btn_callback(int btn, int state)
{
    ui::widget *w = this->tree->search(this->old_pos);

    if (w != NULL)
    {
        glm::ivec2 obj;
        ui::btn_call_data call_data;
        GLuint which = (state == ui::mouse::up
                        ? ui::callback::btn_up
                        : ui::callback::btn_down);

        w->get(ui::element::position, ui::position::all, &obj);
        call_data.location = this->old_pos - obj;
        call_data.button = btn;
        call_data.state = state;
        w->call_callbacks(which, &call_data);
        this->close_pending();
    }

    /* w might no longer exist at this position.  Let's search again,
     * just to make sure.
     */
    this->old_child = this->tree->search(this->old_pos);
}

void ui::composite::key_callback(int key, uint32_t c, int state, int mods)
{
    ui::widget *w = this->tree->search(this->old_pos);

    if (w != NULL)
    {
        glm::ivec2 obj;
        ui::key_call_data call_data;
        GLuint which = (state == ui::key::up
                        ? ui::callback::key_up
                        : ui::callback::key_down);

        w->get(ui::element::position, ui::position::all, &obj);
        call_data.location = this->old_pos - obj;
        call_data.key = key;
        call_data.character = c;
        call_data.state = state;
        call_data.mods = mods;
        w->call_callbacks(which, &call_data);
        this->close_pending();
    }

    /* p might no longer exist at this position.  Let's search again,
     * just to make sure.
     */
    this->old_child = this->tree->search(this->old_pos);
}
