/* widget.h                                                -*- C++ -*-
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 06 Nov 2016, 09:57:20 tquirk
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
 * This file contains the widget object declaration.  The widget adds
 * a parent, the draw method, some GL entity handling, and is the
 * simplest thing that will actually draw on the screen.
 *
 * Things to do
 *
 */

#ifndef __INC_R9_WIDGET_H__
#define __INC_R9_WIDGET_H__

#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include "active.h"
#include "composite.h"

namespace ui
{
    class composite;

    struct vertex_buffer
    {
        const static float no_texture;

        float *vertex;
        GLuint *element;
        GLuint vertex_index, vertex_count, element_index;

        vertex_buffer(GLuint verts, GLuint elts);
        ~vertex_buffer();

        void generate_box(glm::vec2, glm::vec2, const glm::vec4&);
        void generate_ellipse(glm::vec2, glm::vec2, float, int,
                              const glm::vec4&);
        void generate_ellipse_divider(glm::vec2, glm::vec2, float, float,
                                      const glm::vec4&);

        size_t vertex_size(void);
        size_t element_size(void);
    };

    class widget : public active
    {
      protected:
        composite *parent;
        glm::ivec2 pos;
        glm::mat4 pos_transform;
        GLuint vao, vbo, ebo, element_count;
        GLuint border[4], margin[4];
        glm::vec4 foreground, background;
        bool visible;

        int get_position(GLuint, void *);
        void set_position(GLuint, void *);
        virtual int get_state(GLuint, void *);
        virtual void set_state(GLuint, void *);
        virtual int get_border(GLuint, void *);
        virtual void set_border(GLuint, void *);
        virtual int get_margin(GLuint, void *);
        virtual void set_margin(GLuint, void *);
        int get_color(GLuint, void *);
        void set_color(GLuint, void *);
        virtual void set_size(GLuint, void *) override;

        virtual void recalculate_transformation_matrix(void);
        virtual vertex_buffer *generate_points(void);
        virtual void populate_buffers(void);

      public:
        widget(composite *, GLuint, GLuint);
        virtual ~widget();

        virtual int get(GLuint, GLuint, void *) override;
        virtual void set(GLuint, GLuint, void *) override;

        virtual void draw(GLuint, const glm::mat4&);

        void close(void);

        friend class composite;
    };
}

#endif /* __INC_R9_WIDGET_H__ */
