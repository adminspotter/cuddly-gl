/* ui.h                                                    -*- C++ -*-
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 06 Nov 2016, 10:10:24 tquirk
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
 * This file contains the declarations for the UI context, which loads
 * and manages the GLSL program which the rest of the UI toolkit uses
 * to draw its widgets.  It is a descendent of the composite object,
 * which will allow the context to manage the top-level widgets.
 *
 * Things to do
 *
 */

#ifndef __INC_R9_UI_H__
#define __INC_R9_UI_H__

#include "ui_defs.h"
#include "composite.h"

namespace ui
{
    class context : public composite
    {
      private:
        GLuint vert_shader, frag_shader, shader_pgm;
        GLuint pos_attr, norm_attr, color_attr, texture_attr;
        GLuint use_text_uniform, text_bgnd_uniform;
        GLuint translate_uniform;

        std::list<widget *> to_close;

      protected:
        int get_attribute(GLuint, void *);

      public:
        context(GLuint, GLuint);
        ~context();

        virtual int get(GLuint, GLuint, void *) override;

        virtual void close_child(widget *) override;

        void draw(void);
    };
}

#endif /* __INC_R9_UI_H__ */
