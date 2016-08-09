/* composite.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 09 Aug 2016, 09:00:52 tquirk
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

#include "composite.h"

const int ui::composite::tree_max_depth = 4;

ui::composite::composite(composite *c, GLuint w, GLuint h)
    : dim((int)w, (int)h), children()
{
    glm::ivec2 ul = {0, 0};

    this->parent = c;
    this->tree = new quadtree(NULL,
                              ul, this->dim,
                              ui::composite::tree_max_depth);
}

ui::composite::~composite()
{
    delete this->tree;

    while (!this->children.empty())
        delete this->children.front();
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
