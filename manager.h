/* manager.h                                               -*- C++ -*-
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 26 Feb 2017, 09:36:21 tquirk
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
 * This file contains the declaration for the manager object.  It
 * combines the panel with the composite, so we can have dialog boxes
 * and menu bars and menus and whatnot.
 *
 * Things to do
 *
 */

#ifndef __INC_R9_MANAGER_H__
#define __INC_R9_MANAGER_H__

#include "widget.h"
#include "composite.h"

namespace ui
{
    class manager : public widget, public composite
    {
      protected:
        glm::ivec2 child_spacing;

        int get_child_spacing(GLuint, void *);
        void set_child_spacing(GLuint, void *);
        virtual void set_resize(GLuint, void *) override;
        virtual int get_size(GLuint, void *) override;
        virtual void set_size(GLuint, void *) override;
        virtual int get_pixel_size(GLuint, void *) override;

        glm::ivec2 calculate_max_point(void);
        virtual void set_desired_size(void) override;

        static void leave_callback(active *, void *, void *);
        static void motion_callback(active *, void *, void *);
        static void button_callback(active *, void *, void *);
        static void keypress_callback(active *, void *, void *);

        virtual void recalculate_transformation_matrix(void) override;

      public:
        manager(composite *, GLuint, GLuint);
        virtual ~manager();

        virtual int get(GLuint, GLuint, void *) override;
        virtual void set(GLuint, GLuint, void *) override;

        virtual void draw(GLuint, const glm::mat4&) override;

        virtual void add_child(widget *, GLuint = ui::child::async) override;
    };
}

#endif /* __INC_R9_MANAGER_H__ */
