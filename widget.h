/* widget.h                                                -*- C++ -*-
 *   by Trinity Quirk <tquirk@ymb.net>
 *
 * CuddlyGL OpenGL widget toolkit
 * Copyright (C) 2016-2020  Trinity Annabelle Quirk
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

#ifndef __INC_CUDDLY_WIDGET_H__
#define __INC_CUDDLY_WIDGET_H__

#include <vector>

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

        std::vector<float> vertex;
        std::vector<GLuint> element;

        vertex_buffer();
        ~vertex_buffer();

        void generate_box(glm::vec2, glm::vec2, const glm::vec4&);
        void generate_ellipse(glm::vec2, glm::vec2, float, int,
                              const glm::vec4&);
        void generate_ellipse_divider(glm::vec2, glm::vec2, float, float,
                                      const glm::vec4&);

        const float *vertex_data(void);
        size_t vertex_size(void);
        const GLuint *element_data(void);
        size_t element_size(void);
        GLuint element_count(void);
    };

    class widget : public virtual active
    {
      protected:
        composite *parent;
        glm::ivec2 pos, relative_pos;
        glm::mat4 pos_transform;
        GLuint vao, vbo, ebo, element_count;
        GLuint border[4], margin[4];
        glm::vec4 foreground, background;
        bool visible;

        int get_position(GLuint, int *) const;
        int get_position(GLuint, glm::ivec2 *) const;
        void set_position(GLuint, int);
        void set_position(GLuint, const glm::ivec2&);
        virtual int get_state(GLuint, bool *) const;
        virtual void set_state(GLuint, bool);
        virtual int get_border(GLuint, GLuint *) const;
        virtual void set_border(GLuint, GLuint);
        virtual int get_margin(GLuint, GLuint *) const;
        virtual void set_margin(GLuint, GLuint);
        int get_color(GLuint, glm::vec4 *) const;
        void set_color(GLuint, const glm::vec4&);
        virtual void set_size(GLuint, GLuint) override;
        virtual void set_size(GLuint, const glm::ivec2&) override;

        static void reposition(active *, void *, void *);

        virtual void recalculate_absolute_pos(void);
        virtual void recalculate_transformation_matrix(void);
        virtual vertex_buffer *generate_points(void);
        virtual void populate_buffers(void);

        void init(composite *);

      public:
        explicit widget(composite *);
        template<typename... Args>
        widget(composite *c, Args... args)
            : rect(0, 0), active(0, 0), pos(0, 0), pos_transform(),
              foreground(1.0f, 1.0f, 1.0f, 1.0f),
              background(0.5f, 0.5f, 0.5f, 1.0f)
            {
                this->init(c);
                this->set(args...);
            };
        virtual ~widget();

        using ui::rect::get;
        virtual int get(GLuint, GLuint, GLuint *) const override;
        virtual int get(GLuint, GLuint, glm::ivec2 *) const override;
        virtual int get(GLuint, GLuint, int *) const;
        virtual int get(GLuint, GLuint, bool *) const;
        virtual int get(GLuint, GLuint, glm::vec4 *) const;
        using ui::rect::set;
        virtual void set(GLuint, GLuint, GLuint) override;
        virtual void set(GLuint, GLuint, const glm::ivec2&) override;
        virtual void set(GLuint, GLuint, int) override;
        virtual void set(GLuint, GLuint, bool);
        virtual void set(GLuint, GLuint, const glm::vec4&);

        GET_VA;
        SET_VA;

        virtual void draw(GLuint, const glm::mat4&);

        void close(void);

        friend class composite;
    };
}

#endif /* __INC_CUDDLY_WIDGET_H__ */
