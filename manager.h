/* manager.h                                               -*- C++ -*-
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 20 Dec 2018, 08:03:27 tquirk
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

        int get_child_spacing(GLuint, GLuint *) const;
        int get_child_spacing(GLuint, glm::ivec2 *) const;
        void set_child_spacing(GLuint, GLuint);
        void set_child_spacing(GLuint, const glm::ivec2&);
        virtual void set_resize(GLuint, GLuint) override;
        virtual int get_size(GLuint, GLuint *) const override;
        virtual int get_size(GLuint, glm::ivec2 *) const override;
        virtual void set_size(GLuint, GLuint) override;
        virtual void set_size(GLuint, const glm::ivec2&) override;
        virtual int get_pixel_size(GLuint, float *) const override;
        virtual int get_pixel_size(GLuint, glm::vec3 *) const override;

        glm::ivec2 calculate_max_point(void);
        virtual void set_desired_size(void) override;

        virtual void recalculate_transformation_matrix(void) override;

        void init(composite *);

      public:
        explicit manager(composite *);
        template<typename... Args>
        manager(composite *c, Args... args)
            : rect(0, 0), active(0, 0), widget(c), composite(c),
              child_spacing(0, 0)
            {
                this->init(c);
                this->set(args...);
            };
        virtual ~manager();

        using ui::widget::get;
        using ui::composite::get;
        virtual int get(GLuint, GLuint, GLuint *) const override;
        virtual int get(GLuint, GLuint, glm::ivec2 *) const override;
        using ui::widget::set;
        using ui::composite::set;
        virtual void set(GLuint, GLuint, GLuint) override;
        virtual void set(GLuint, GLuint, const glm::ivec2&) override;

        GET_VA;
        SET_VA;

        virtual void draw(GLuint, const glm::mat4&) override;
    };
}

#endif /* __INC_CUDDLY_MANAGER_H__ */
