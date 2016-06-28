/* quadtree.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 28 Jun 2016, 06:33:02 tquirk
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
 * This file contains the quadtree method definitions.
 *
 * Things to do
 *
 */

#include "quadtree.h"

Quadtree::Quadtree(Quadtree *p,
                   glm::ivec2& pt1, glm::ivec2& pt2,
                   int max_depth, int cur_depth)
    : center(), min(), max(), contents()
{
}

Quadtree::~Quadtree()
{
}

void Quadtree::insert(void *obj)
{
}

void Quadtree::remove(void *obj)
{
}

void Quadtree::clear(void)
{
}

std::list<void *> Quadtree::search(const glm::ivec2& pt)
{
}
