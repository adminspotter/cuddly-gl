/* composite.h                                             -*- C++ -*-
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 14 Aug 2019, 08:17:27 tquirk
 *
 * CuddlyGL OpenGL widget toolkit
 * Copyright (C) 2019  Trinity Annabelle Quirk
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

#ifndef __INC_CUDDLY_COMPOSITE_H__
#define __INC_CUDDLY_COMPOSITE_H__

#include <list>

#include "ui_defs.h"
#include "active.h"
#include "quadtree.h"
#include "widget.h"

namespace ui
{
    class quadtree;
    class widget;

    class composite : public virtual active
    {
      protected:
        composite *parent;
        std::list<widget *> children, to_remove;
        quadtree *tree;
        GLuint resize;
        bool dirty, radio_box;

        glm::ivec2 old_pos;
        widget *old_child;

        const static int tree_max_depth;

        int get_radio_state(bool *) const;
        void set_radio_state(bool);
        void set_radio_child(GLuint, ui::widget *);
        virtual void set_size(GLuint, GLuint) override;
        virtual void set_size(GLuint, const glm::ivec2&) override;
        virtual int get_resize(GLuint, GLuint *) const;
        virtual void set_resize(GLuint, GLuint);
        virtual int get_pixel_size(GLuint, float *) const;
        virtual int get_pixel_size(GLuint, glm::vec3 *) const;
        virtual int get_state(GLuint, bool *) const;
        virtual void set_state(GLuint, bool);

        virtual void set_desired_size(void);

        void reposition_children(void);
        void regenerate_children(void);
        void regenerate_search_tree(void);

        void clear_removed_children(void);

        void child_motion(widget *, GLuint, glm::ivec2&);

        void init(composite *);

      public:
        explicit composite(composite *);
        template<typename... Args>
        composite(composite *c, Args... args)
            : rect(0, 0), active(0, 0), children(), to_remove(), old_pos(0, 0)
            {
                this->init(c);
                this->set(args...);
            };
        virtual ~composite();

        using ui::rect::get;
        virtual int get(GLuint, GLuint, GLuint *) const override;
        virtual int get(GLuint, GLuint, float *) const;
        virtual int get(GLuint, GLuint, glm::vec3 *) const;
        virtual int get(GLuint, GLuint, bool *) const;
        using ui::rect::set;
        virtual void set(GLuint, GLuint, GLuint) override;
        virtual void set(GLuint, GLuint, bool);
        virtual void set(GLuint, GLuint, ui::widget *);

        GET_VA;
        SET_VA;

        virtual void add_child(widget *);
        virtual void remove_child(widget *);
        virtual void move_child(widget *);

        void manage_children(void);

        void mouse_pos_callback(int, int);
        virtual void mouse_pos_callback(glm::ivec2&);
        void mouse_btn_callback(int, int, int);
        virtual void mouse_btn_callback(ui::btn_call_data&);
        void key_callback(int, uint32_t, int, int);
        void key_callback(ui::key_call_data&);
    };
}

#endif /* __INC_CUDDLY_COMPOSITE_H__ */
