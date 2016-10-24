/* quadtree.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 09 Oct 2016, 14:43:09 tquirk
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
 * +---+---+
 * | 0 | 1 |
 * +---+---+
 * | 2 | 3 |
 * +---+---+
 *
 * Things to do
 *
 */

#include <algorithm>

#include "ui_defs.h"
#include "quadtree.h"

int ui::quadtree::classify(ui::widget *p)
{
    glm::ivec2 ul, lr;
    int retval = 0;

    p->get_va(ui::element::position, ui::position::all, &ul,
              ui::element::size, ui::size::all, &lr, 0);
    /* Width and height are not absolute screen coords */
    lr += ul;
    retval |= this->quad_mask(ul);
    retval |= this->quad_mask(ul.x, lr.y);
    retval |= this->quad_mask(lr.x, ul.y);
    retval |= this->quad_mask(lr);
    return retval;
}

ui::quadtree::quadtree(ui::quadtree *p,
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

        this->quadrant[0] = new quadtree(this,
                                         this->center, this->min,
                                         max_depth, ++cur_depth);
        tmp_pt.x = this->max.x;
        tmp_pt.y = this->min.y;
        this->quadrant[1] = new quadtree(this,
                                         this->center, tmp_pt,
                                         max_depth, cur_depth);
        tmp_pt.x = this->min.x;
        tmp_pt.y = this->max.y;
        this->quadrant[2] = new quadtree(this,
                                         this->center, tmp_pt,
                                         max_depth, cur_depth);
        this->quadrant[3] = new quadtree(this,
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

ui::quadtree::~quadtree()
{
    if (this->quadrant[0] != NULL) delete this->quadrant[0];
    if (this->quadrant[1] != NULL) delete this->quadrant[1];
    if (this->quadrant[2] != NULL) delete this->quadrant[2];
    if (this->quadrant[3] != NULL) delete this->quadrant[3];
}

void ui::quadtree::insert(ui::widget *obj)
{
    int which = this->classify(obj);

    if (which == 0)
        return;
    this->contents.push_front(obj);
    if (which & 0x01 && this->quadrant[0] != NULL)
        this->quadrant[0]->insert(obj);
    if (which & 0x02 && this->quadrant[1] != NULL)
        this->quadrant[1]->insert(obj);
    if (which & 0x04 && this->quadrant[2] != NULL)
        this->quadrant[2]->insert(obj);
    if (which & 0x08 && this->quadrant[3] != NULL)
        this->quadrant[3]->insert(obj);
}

void ui::quadtree::remove(ui::widget *obj)
{
    int which = this->classify(obj);

    this->contents.remove(obj);
    if (which & 0x01 && this->quadrant[0] != NULL)
        this->quadrant[0]->remove(obj);
    if (which & 0x02 && this->quadrant[1] != NULL)
        this->quadrant[1]->remove(obj);
    if (which & 0x04 && this->quadrant[2] != NULL)
        this->quadrant[2]->remove(obj);
    if (which & 0x08 && this->quadrant[3] != NULL)
        this->quadrant[3]->remove(obj);
}

void ui::quadtree::clear(void)
{
    this->contents.clear();
    if (this->quadrant[0] != NULL) this->quadrant[0]->clear();
    if (this->quadrant[1] != NULL) this->quadrant[1]->clear();
    if (this->quadrant[2] != NULL) this->quadrant[2]->clear();
    if (this->quadrant[3] != NULL) this->quadrant[3]->clear();
}

ui::widget *ui::quadtree::search(const glm::ivec2& pt)
{
    if (this->contents.empty())
        return NULL;

    int which = this->quad_index(pt);
    if (this->quadrant[which] != NULL)
        return this->quadrant[which]->search(pt);

    std::list<ui::widget *>::iterator i;
    for (i = this->contents.begin(); i != this->contents.end(); ++i)
    {
        glm::ivec2 ul, lr;

        (*i)->get_va(ui::element::position, ui::position::all, &ul,
                     ui::element::size, ui::size::all, &lr, 0);
        lr += ul;
        if (pt.x >= ul.x && pt.x < lr.x && pt.y >= ul.y && pt.y < lr.y)
            return *i;
    }
    return NULL;
}
