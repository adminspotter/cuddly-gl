/* quadtree.h                                              -*- C++ -*-
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 29 Jun 2016, 08:17:08 tquirk
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
 * This file contains a quadtree declaration, which we'll use for
 * sorting and searching UI elements.
 *
 * Things to do
 *
 */

#ifndef __INC_R9_QUADTREE_H__
#define __INC_R9_QUADTREE_H__

#include <list>

#include <glm/vec2.hpp>

#include "panel.h"

class Quadtree
{
  private:
    glm::ivec2 center, min, max;
    std::list<ui::panel *>  contents;
    Quadtree *quadrant[4], *parent;

    static const int quad0 = 1, quad1 = 2, quad2 = 4, quad3 = 8;

    int classify(ui::panel *);

  public:
    Quadtree(Quadtree *, glm::ivec2&, glm::ivec2&, int, int = 0);
    ~Quadtree();

    void insert(ui::panel *);
    void remove(ui::panel *);
    void clear(void);

    ui::panel *search(const glm::ivec2&);
};

#endif /* __INC_R9_QUADTREE_H__ */
