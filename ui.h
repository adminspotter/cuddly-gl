/* ui.h                                                    -*- C++ -*-
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 24 Jun 2016, 07:18:51 tquirk
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
 * This file contains the basic ui declarations and the base ui
 * utility namespace definitions for the R9 UI widget set.
 *
 * Things to do
 *
 */

#ifndef __INC_R9_UI_H__
#define __INC_R9_UI_H__

#include <GL/gl.h>

#include <list>

#include "ui_defs.h"

namespace ui
{
    /* Forward declaration for our child list */
    class panel;

    class context
    {
      private:
        GLuint vert_shader, frag_shader, shader_pgm;
        GLuint pos_attr, norm_attr, color_attr, texture_attr;
        GLuint use_text_uniform;
        std::list<panel *> children;

      protected:
        GLuint width, height;

        int get_size(GLuint, void *);
        void set_size(GLuint, void *);
        int get_attribute(GLuint, void *);

      public:
        context(GLuint, GLuint);
        ~context();

        int get(GLuint, GLuint, void *);
        void set(GLuint, GLuint, void *);

        void draw(void);

        context& add_child(panel *);
        context& remove_child(panel *);

        void cursor_pos_callback(int, int);
        void cursor_btn_callback(int, int);
    };
}

#endif /* __INC_R9_UI_H__ */
