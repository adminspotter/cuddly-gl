/* manager.h                                          -*- C++ -*-
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 15 Aug 2016, 22:13:28 tquirk
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

#include "panel.h"
#include "composite.h"

namespace ui
{
    class manager : public panel, public composite
    {
      protected:
        glm::ivec2 child_spacing;
        GLuint resize;

        int get_child_spacing(GLuint, void *);
        void set_child_spacing(GLuint, void *);
        int get_resize(GLuint, void *);
        void set_resize(GLuint, void *);
        virtual void set_position(GLuint, void *);
        int get_pixel_size(GLuint, void *);

        void set_desired_size(void);

        static void motion_callback(panel *, void *, void *);
        static void button_callback(panel *, void *, void *);
        static void keypress_callback(panel *, void *, void *);

      public:
        manager(composite *, GLuint, GLuint);
        virtual ~manager();

        virtual int get(GLuint, GLuint, void *);
        virtual void set(GLuint, GLuint, void *);

        virtual void draw(void);

        virtual void add_child(panel *);
        virtual void remove_child(panel *);
        virtual void move_child(panel *);
    };
}

#endif /* __INC_R9_MANAGER_H__ */
