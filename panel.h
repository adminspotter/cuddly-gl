/* panel.h                                                 -*- C++ -*-
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 11 Mar 2016, 09:54:23 tquirk
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
 * Things to do
 *
 */

#ifndef __INC_R9_PANEL_H__
#define __INC_R9_PANEL_H__

#include <GL/gl.h>
#include <GL/glext.h>

#include <glm/vec4.hpp>

#include "ui.h"

namespace ui
{
    class panel
    {
      protected:
        context *parent;
        GLuint vao, vbo, ebo, element_count;
        GLuint width, height, xpos, ypos;
        GLuint margin[4], border[4];
        glm::vec4 foreground, background;

        int get_size(GLuint, void *);
        void set_size(GLuint, void *);
        int get_border(GLuint, void *);
        void set_border(GLuint, void *);
        int get_margin(GLuint, void *);
        void set_margin(GLuint, void *);
        int get_color(GLuint, void *);
        void set_color(GLuint, void *);

        virtual void populate_buffers(void);

      public:
        panel(context *, GLuint, GLuint);
        virtual ~panel();

        virtual int get(GLuint, GLuint, void *);
        virtual void set(GLuint, GLuint, void *);
        void set_va(GLuint, GLuint, void *, ...);

        virtual void draw(void);

        void close(void);

        /* The context may change sizes, so it needs to be able to
         * call populate_buffers() in the event of a window resize.
         */
        friend class context;
    };
}

#endif /* __INC_R9_PANEL_H__ */
