/* composite.h                                             -*- C++ -*-
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 16 May 2017, 17:37:10 tquirk
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

#include "ui_defs.h"
#include "rect.h"
#include "quadtree.h"
#include "widget.h"

namespace ui
{
    class quadtree;
    class widget;

    class composite : public virtual rect
    {
      protected:
        composite *parent;
        std::list<widget *> children, to_remove;
        quadtree *tree;
        GLuint resize;
        bool dirty;

        glm::ivec2 old_pos;
        widget *old_child;

        const static int tree_max_depth;

        virtual int get_size(GLuint, void *) override;
        virtual void set_size(GLuint, void *) override;
        virtual int get_resize(GLuint, void *);
        virtual void set_resize(GLuint, void *);
        virtual int get_pixel_size(GLuint, void *);

        virtual void set_desired_size(void);

        void reposition_children(void);
        void regenerate_children(void);
        void regenerate_search_tree(void);

        void clear_removed_children(void);

      public:
        composite(composite *, GLuint, GLuint);
        virtual ~composite();

        virtual int get(GLuint, GLuint, void *) override;
        virtual void set(GLuint, GLuint, void *) override;

        virtual void add_child(widget *);
        virtual void remove_child(widget *, GLuint = ui::child::sync);
        virtual void move_child(widget *, GLuint = ui::child::sync);

        void mouse_pos_callback(int, int);
        virtual void mouse_btn_callback(int, int);
        void key_callback(int, uint32_t, int, int);
    };
}

#endif /* __INC_R9_COMPOSITE_H__ */
