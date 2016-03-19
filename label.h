/* label.h                                                 -*- C++ -*-
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 19 Mar 2016, 08:59:52 tquirk
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
 * This file contains the basic label object declaration.  We derive
 * from the panel, which will take care of the box part.  In this
 * class, we'll add font handling via freetype, and we'll get our text
 * on the screen via a 2D GL texture.
 *
 * Things to do
 *
 */

#ifndef __INC_R9_LABEL_H__
#define __INC_R9_LABEL_H__

#include "panel.h"

namespace ui
{
    class label : public panel
    {
      protected:
        GLuint tex;

        int get_font(GLuint, void *);
        void set_font(GLuint, void *);
        int get_bgimage(GLuint, void *);
        void set_bgimage(GLuint, void *);

        virtual void populate_buffers(void);

      public:
        label(context *, GLuint, GLuint);
        virtual ~label();

        virtual int get(GLuint, GLuint, void *);
        virtual void set(GLuint, GLuint, void *);

        virtual void draw(void);
    };
}

#endif /* __INC_R9_LABEL_H__ */
