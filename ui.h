/* ui.h                                                    -*- C++ -*-
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 31 Jul 2016, 15:42:07 tquirk
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

#include <glm/vec2.hpp>

#include "ui_defs.h"
#include "quadtree.h"

namespace ui
{
    /* Forward declarations for multi-include problems */
    class quadtree;
    class panel;

    /* Mouse motion callback routines */
    typedef struct mouse_callback_call
    {
        glm::ivec2 location;
    }
    mouse_call_data;

    /* Button press/release callback routines */
    typedef struct btn_callback_call
    {
        glm::ivec2 location;
        GLuint button;
        GLuint state;
    }
    btn_call_data;

    /* Key press/release callback routines */
    typedef struct key_callback_call
    {
        glm::ivec2 location;
        uint32_t character;
        GLuint key;
        GLuint state;
        GLuint mods;
    }
    key_call_data;

    class context
    {
      private:
        GLuint vert_shader, frag_shader, shader_pgm;
        GLuint pos_attr, norm_attr, color_attr, texture_attr;
        GLuint use_text_uniform;
        std::list<panel *> children;

        quadtree *tree;
        /* Previous mouse position and pointed-to child */
        glm::ivec2 old_mouse;
        panel *old_child;

        const static int tree_max_depth;

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
        context& move_child(panel *);

        void mouse_pos_callback(int, int);
        void mouse_btn_callback(int, int);
        void key_callback(int, uint32_t, int, int);
    };
}

#endif /* __INC_R9_UI_H__ */
