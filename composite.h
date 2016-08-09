/* composite.h                                             -*- C++ -*-
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 09 Aug 2016, 08:17:46 tquirk
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
 * This file contains the declaration for the composite object.  It's
 * a role which handles child management
 *
 * Things to do
 *
 */

#ifndef __INC_R9_COMPOSITE_H__
#define __INC_R9_COMPOSITE_H__

#include <list>

#include "quadtree.h"
#include "panel.h"

namespace ui
{
    class composite
    {
      protected:
        glm::ivec2 dim;
        composite *parent;
        std::list<panel *> children;
        quadtree *tree;

        const static int tree_max_depth;

      public:
        composite(composite *, GLuint, GLuint);
        virtual ~composite();

        void add_child(panel *);
        void remove_child(panel *);
        void move_child(panel *);
    };
}

#endif /* __INC_R9_COMPOSITE_H__ */