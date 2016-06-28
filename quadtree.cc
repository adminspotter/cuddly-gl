/* quadtree.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 28 Jun 2016, 06:52:21 tquirk
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

#include <algorithm>

#include "quadtree.h"

Quadtree::Quadtree(Quadtree *p,
                   glm::ivec2& pt1, glm::ivec2& pt2,
                   int max_depth, int cur_depth)
    : center(), min(), max(), contents()
{
    this->min.x = std::min(pt1.x, pt2.x);
    this->min.y = std::min(pt1.y, pt2.y);
    this->max.x = std::max(pt1.x, pt2.x);
    this->max.y = std::max(pt1.y, pt2.y);
    this->center = this->min + ((this->max - this->min) / 2);

    this->parent = p;
    if (cur_depth < max_depth)
    {
        glm::ivec2 tmp_pt;

        this->quadrant[0] = new Quadtree(this,
                                         this->center, this->min,
                                         max_depth, ++cur_depth);
        tmp_pt.x = this->max.x;
        tmp_pt.y = this->min.y;
        this->quadrant[1] = new Quadtree(this,
                                         this->center, tmp_pt,
                                         max_depth, cur_depth);
        tmp_pt.x = this->min.x;
        tmp_pt.y = this->max.y;
        this->quadrant[2] = new Quadtree(this,
                                         this->center, tmp_pt,
                                         max_depth, cur_depth);
        this->quadrant[3] = new Quadtree(this,
                                         this->center, this->max,
                                         max_depth, cur_depth);
    }
    else
    {
        this->quadrant[0] = NULL;
        this->quadrant[1] = NULL;
        this->quadrant[2] = NULL;
        this->quadrant[3] = NULL;
    }
}

Quadtree::~Quadtree()
{
    if (this->quadrant[0] != NULL) delete this->quadrant[0];
    if (this->quadrant[1] != NULL) delete this->quadrant[1];
    if (this->quadrant[2] != NULL) delete this->quadrant[2];
    if (this->quadrant[3] != NULL) delete this->quadrant[3];
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

void *Quadtree::search(const glm::ivec2& pt)
{
    return NULL;
}
