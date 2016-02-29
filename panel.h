/* panel.h                                                 -*- C++ -*-
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 28 Feb 2016, 11:35:30 tquirk
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

namespace ui
{
    namespace element
    {
        const GLuint size = 1, border = 2, margin = 3, radius = 4;
    }

    namespace corner
    {
        const GLuint top_left = 1, top_right = 2,
            bottom_left = 4, bottom_right = 8,
            top = 3, left = 5, right = 10, bottom = 12, all = 15;
    }
    namespace side
    {
        const GLuint top = 1, left = 2, right = 4, bottom = 8, all = 15;
    }

    class panel
    {
      protected:
        GLuint width, height;
        GLuint margin[4], border[4], radius[4];

        void set_size(GLuint, GLuint);
        void set_border(GLuint, GLuint);
        void set_margin(GLuint, GLuint);
        void set_radius(GLuint, GLuint);

      public:
        panel(GLuint, GLuint);

        void set(GLuint, GLuint, GLuint);
        void set_va(GLuint, GLuint, GLuint, ...);

        void draw(void)
    };

}

#endif /* __INC_R9_PANEL_H__ */
