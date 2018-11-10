/* rect.h                                                  -*- C++ -*-
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 04 Sep 2018, 07:06:50 tquirk
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
 * This file contains the rect object declaration.  The rect has a
 * size and the basic get and set methods.
 *
 * Things to do
 *
 */

#ifndef __INC_CUDDLY_RECT_H__
#define __INC_CUDDLY_RECT_H__

#include <GL/gl.h>

#include <glm/vec2.hpp>

namespace ui
{
    class rect
    {
      protected:
        glm::ivec2 dim;

        virtual int get_size(GLuint, void *) const;
        virtual void set_size(GLuint, GLuint);
        virtual void set_size(GLuint, const glm::ivec2&);

      public:
        rect(GLuint, GLuint);
        virtual ~rect();

        virtual int get(GLuint, GLuint, void *) const;
        virtual void set(GLuint, GLuint, int);
        virtual void set(GLuint, GLuint, GLuint);
        virtual void set(GLuint, GLuint, const glm::ivec2&);

        GET_VA;
        SET_VA;
    };
}

#endif /* __INC_CUDDLY_RECT_H__ */
