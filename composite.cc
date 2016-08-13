/* composite.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 13 Aug 2016, 07:35:32 tquirk
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
 * It's a panel which can have children, which will then be
 * subclassable into more interesting manager widgets.
 *
 * Things to do
 *
 */

#include <algorithm>

#include "ui_defs.h"
#include "composite.h"

const int ui::composite::tree_max_depth = 4;

int ui::composite::get_size(GLuint t, void *v)
{
    int ret = 0;

    switch (t)
    {
      case ui::size::width:  *((GLuint *)v) = this->dim.x;  break;
      case ui::size::height: *((GLuint *)v) = this->dim.y;  break;
      default:               ret = 1;                       break;
    }
    return ret;
}

void ui::composite::set_size(GLuint d, void *v)
{
    GLuint new_v = *((GLuint *)v);

    switch (d)
    {
      case ui::size::width:   this->dim.x = new_v;  break;
      case ui::size::height:  this->dim.y = new_v;  break;
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

int ui::composite::get_transform(GLuint t, void *v)
{
    if (t == ui::transform::translate)
    {
        *((glm::mat4 *)v) = this->translate;
        return 0;
    }
    return 1;
}

ui::composite::composite(composite *c, GLuint w, GLuint h)
    : dim((int)w, (int)h), children(), old_pos(0, 0), translate()
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
    if (e == ui::element::size)
        return this->get_size(t, v);
    else if (e == ui::element::transform)
        return this->get_transform(t, v);
    return 1;
}

void ui::composite::set(GLuint e, GLuint t, void *v)
{
    if (e == ui::element::size)
    {
        this->set_size(t, v);
        for (auto i = this->children.begin(); i != this->children.end(); ++i)
            (*i)->populate_buffers();
    }
}

void ui::composite::add_child(ui::panel *p)
{
    auto found = std::find(this->children.begin(), this->children.end(), p);
    if (found == this->children.end())
    {
        this->children.push_back(p);
        this->tree->insert(p);
    }
}

void ui::composite::remove_child(ui::panel *p)
{
    this->children.remove(p);
    this->tree->remove(p);
}

void ui::composite::move_child(ui::panel *p)
{
    this->tree->remove(p);
    this->tree->insert(p);
}

void ui::composite::mouse_pos_callback(int x, int y)
{
    glm::ivec2 pos = {x, y}, obj;
    ui::mouse_call_data call_data;
    ui::panel *p = this->tree->search(pos);

    if (p != NULL)
    {
        p->get_va(ui::element::position, ui::position::x, &obj.x,
                  ui::element::position, ui::position::y, &obj.y, 0);
        call_data.location = pos - obj;
        if (this->old_child == NULL)
            p->call_callbacks(ui::callback::enter, &call_data);
        p->call_callbacks(ui::callback::motion, &call_data);
    }
    else if (this->old_child != NULL)
    {
        this->old_child->get_va(ui::element::position, ui::position::x, &obj.x,
                                ui::element::position, ui::position::y, &obj.y,
                                0);
        call_data.location = pos - obj;
        this->old_child->call_callbacks(ui::callback::leave, &call_data);
    }

    this->old_child = p;
    this->old_pos = pos;
}

void ui::composite::mouse_btn_callback(int btn, int state)
{
    ui::panel *p = this->tree->search(this->old_pos);

    if (p != NULL)
    {
        glm::ivec2 obj;
        ui::btn_call_data call_data;

        p->get_va(ui::element::position, ui::position::x, &obj.x,
                  ui::element::position, ui::position::y, &obj.y, 0);
        call_data.location = this->old_pos - obj;
        call_data.button = btn;
        call_data.state = (state == ui::mouse::up
                           ? ui::callback::btn_up
                           : ui::callback::btn_down);
        p->call_callbacks(call_data.state, &call_data);
    }

    this->old_child = p;
}

void ui::composite::key_callback(int key, uint32_t c, int state, int mods)
{
    ui::panel *p = this->tree->search(this->old_pos);

    if (p != NULL)
    {
        glm::ivec2 obj;
        ui::key_call_data call_data;

        p->get_va(ui::element::position, ui::position::x, &obj.x,
                  ui::element::position, ui::position::y, &obj.y, 0);
        call_data.location = this->old_pos - obj;
        call_data.key = key;
        call_data.character = c;
        call_data.state = (state == ui::key::up
                           ? ui::callback::key_up
                           : ui::callback::key_down);
        call_data.mods = mods;
        p->call_callbacks(call_data.state, &call_data);
    }

    this->old_child = p;
}
