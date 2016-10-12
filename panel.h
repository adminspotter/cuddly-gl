/* panel.h                                                 -*- C++ -*-
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 10 Oct 2016, 08:52:54 tquirk
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
 * This file contains the basic panel object declaration and some
 * utility namespace definitions for the R9 UI widget set.
 *
 * We have callback lists here, but for the most part they will be
 * empty.  Once we get to the buttons and things, they'll start having
 * default actions in some of the callback lists.
 *
 * Things to do
 *
 */

#ifndef __INC_R9_PANEL_H__
#define __INC_R9_PANEL_H__

#include <GL/gl.h>
#include <GL/glext.h>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include "composite.h"
#include "active.h"

namespace ui
{
    /* Forward declarations for multi-include problems */
    class composite;

    class panel : public active
    {
      protected:
        const static float no_texture;

        composite *parent;
        GLuint vao, vbo, ebo, vertex_count, element_count;
        glm::ivec2 size, pos;
        GLuint margin[4], border[4];
        glm::vec4 foreground, background;
        bool to_close;

        virtual int get_position(GLuint, void *);
        virtual void set_position(GLuint, void *);
        virtual int get_size(GLuint, void *);
        virtual void set_size(GLuint, void *);
        virtual int get_border(GLuint, void *);
        virtual void set_border(GLuint, void *);
        virtual int get_margin(GLuint, void *);
        virtual void set_margin(GLuint, void *);
        int get_color(GLuint, void *);
        void set_color(GLuint, void *);

        void prep_vao_vbo(GLuint *, GLuint *);
        virtual void generate_points(float *, GLuint *);
        virtual void populate_buffers(void);

      public:
        panel(composite *, GLuint, GLuint);
        virtual ~panel();

        virtual int get(GLuint, GLuint, void *);
        virtual void set(GLuint, GLuint, void *);
        void get_va(GLuint, GLuint, void *, ...);
        void set_va(GLuint, GLuint, void *, ...);

        virtual void draw(void);

        void close(void);

        /* The composite may change sizes, so it needs to be able to
         * call populate_buffers() in the event of a window resize.
         */
        friend class composite;
    };
}

#endif /* __INC_R9_PANEL_H__ */
