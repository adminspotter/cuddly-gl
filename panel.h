/* panel.h                                                 -*- C++ -*-
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 14 Jul 2016, 19:15:57 tquirk
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

#include <glm/vec4.hpp>

#include <list>

#include "ui.h"

namespace ui
{
    /* Forward declarations for multi-include problems */
    class context;
    class panel;

    /* Callback function pointer */
    typedef void (*cb_fptr)(panel *, void *, void *);

    class panel
    {
      protected:
        const static float no_texture;

        typedef struct cb_list_tag
        {
            cb_fptr ptr;
            void *client_data;

            bool operator==(const struct cb_list_tag& p) const
                {
                    return (this->ptr == p.ptr
                            && this->client_data == p.client_data);
                };
            void operator()(panel *p, void *call_data)
                {
                    this->ptr(p, call_data, this->client_data);
                };
        }
        cb_list_elem;
        std::list<cb_list_elem> enter_cb, leave_cb, motion_cb;
        std::list<cb_list_elem> btn_down_cb, btn_up_cb;
        std::list<cb_list_elem> key_down_cb, key_up_cb;

        std::list<cb_list_elem>& which_cb_list(GLuint);

        context *parent;
        GLuint vao, vbo, ebo, vertex_count, element_count;
        GLuint width, height, xpos, ypos;
        GLuint margin[4], border[4];
        glm::vec4 foreground, background;

        int get_position(GLuint, void *);
        void set_position(GLuint, void *);
        int get_size(GLuint, void *);
        void set_size(GLuint, void *);
        int get_border(GLuint, void *);
        void set_border(GLuint, void *);
        virtual int get_margin(GLuint, void *);
        virtual void set_margin(GLuint, void *);
        int get_color(GLuint, void *);
        void set_color(GLuint, void *);

        virtual void generate_points(float *, GLuint *);
        virtual void populate_buffers(void);

      public:
        panel(context *, GLuint, GLuint);
        virtual ~panel();

        virtual int get(GLuint, GLuint, void *);
        virtual void set(GLuint, GLuint, void *);
        void get_va(GLuint, GLuint, void *, ...);
        void set_va(GLuint, GLuint, void *, ...);

        virtual void draw(void);

        void close(void);

        virtual void add_callback(GLuint, cb_fptr, void *);
        virtual void remove_callback(GLuint, cb_fptr, void *);
        virtual void call_callbacks(GLuint);

        /* The context may change sizes, so it needs to be able to
         * call populate_buffers() in the event of a window resize.
         */
        friend class context;
    };
}

#endif /* __INC_R9_PANEL_H__ */
