/* manager.h                                               -*- C++ -*-
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 28 Jul 2018, 07:58:02 tquirk
 *
 * CuddlyGL OpenGL widget toolkit
 * Copyright (C) 2018  Trinity Annabelle Quirk
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

#ifndef __INC_CUDDLY_MANAGER_H__
#define __INC_CUDDLY_MANAGER_H__

#include "widget.h"
#include "composite.h"

namespace ui
{
    class manager : public widget, public composite
    {
      protected:
        glm::ivec2 child_spacing;

        int get_child_spacing(GLuint, void *) const;
        void set_child_spacing(GLuint, void *);
        virtual void set_resize(GLuint, void *) override;
        virtual int get_size(GLuint, void *) const override;
        virtual void set_size(GLuint, void *) override;
        virtual int get_pixel_size(GLuint, void *) const override;

        glm::ivec2 calculate_max_point(void);
        virtual void set_desired_size(void) override;

        virtual void recalculate_transformation_matrix(void) override;

      public:
        manager(composite *, GLuint, GLuint);
        virtual ~manager();

        virtual int get(GLuint, GLuint, void *) const override;
        virtual void set(GLuint, GLuint, void *) override;

        virtual void draw(GLuint, const glm::mat4&) override;
    };
}

#endif /* __INC_CUDDLY_MANAGER_H__ */
