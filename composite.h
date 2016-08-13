/* composite.h                                             -*- C++ -*-
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 13 Aug 2016, 07:33:19 tquirk
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

#include <glm/mat4x4.hpp>

#include "quadtree.h"
#include "panel.h"

namespace ui
{
    class quadtree;
    class panel;

    class composite
    {
      protected:
        glm::ivec2 dim;
        composite *parent;
        std::list<panel *> children;
        quadtree *tree;
        glm::mat4 translate;

        glm::ivec2 old_pos;
        panel *old_child;

        const static int tree_max_depth;

        int get_size(GLuint, void *);
        void set_size(GLuint, void *);
        virtual int get_transform(GLuint, void *);

      public:
        composite(composite *, GLuint, GLuint);
        virtual ~composite();

        virtual int get(GLuint, GLuint, void *);
        virtual void set(GLuint, GLuint, void *);

        void add_child(panel *);
        void remove_child(panel *);
        void move_child(panel *);

        void mouse_pos_callback(int, int);
        void mouse_btn_callback(int, int);
        void key_callback(int, uint32_t, int, int);
    };
}

#endif /* __INC_R9_COMPOSITE_H__ */
