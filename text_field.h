/* text_field.h                                            -*- C++ -*-
 *   by Trinity Quirk <tquirk@ymb.net>
 *
 * CuddlyGL OpenGL widget toolkit
 * Copyright (C) 2016-2021  Trinity Annabelle Quirk
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
 * This file contains the text field declaration.  We're subclassing
 * the label, since it already has font handling built in.  We'll be
 * adding keyboard handling and a moving cursor.
 *
 * Things to do
 *
 */

#ifndef __INC_CUDDLY_TEXT_FIELD_H__
#define __INC_CUDDLY_TEXT_FIELD_H__

#include <chrono>
#include <mutex>

#include "label.h"

namespace ui
{
    class text_field : public label
    {
      protected:
        GLuint cursor_pos, blink, max_length;
        GLuint repeat_initial, repeat_delay;
        GLuint cursor_vao, cursor_vbo, cursor_ebo, cursor_element_count;
        glm::mat4 cursor_transform;
        std::chrono::high_resolution_clock::time_point cursor_clock;
        bool cursor_visible, cursor_active;
        std::mutex repeat_mutex;

        using ui::label::get_size;
        virtual int get_size(GLuint, GLuint *) const override;
        using ui::label::set_size;
        virtual void set_size(GLuint, GLuint) override;
        virtual int get_cursor(GLuint, GLuint *) const;
        virtual void set_cursor(GLuint, GLuint);
        virtual int get_repeat(GLuint, GLuint *) const;
        virtual void set_repeat(GLuint, GLuint);
        virtual void set_font(GLuint, ui::base_font *) override;
        virtual void set_string(GLuint, const std::string&) override;
        virtual void set_image(GLuint, const ui::image&) final;

        static void focus_callback(active *, void *, void *);
        static void key_down_callback(active *, void *, void *);
        static void key_up_callback(active *, void *, void *);
        static void key_timeout(active *, void *);

        int get_cursor_pos(GLuint *) const;
        void set_cursor_pos(GLuint);
        int get_cursor_blink(GLuint *) const;
        void set_cursor_blink(GLuint);
        int get_initial_repeat(GLuint *) const;
        void set_initial_repeat(GLuint);
        int get_secondary_repeat(GLuint *) const;
        void set_secondary_repeat(GLuint);
        void apply_key(const ui::key_call_data *);
        void reset_cursor(void);
        void activate_cursor(void);
        void deactivate_cursor(void);
        void first_char(void);
        void previous_char(void);
        void next_char(void);
        void last_char(void);
        void insert_char(uint32_t);
        void remove_previous_char(void);
        void remove_next_char(void);

        virtual void get_string_size(const std::u32string&,
                                     GLuint&, GLuint&, GLuint&);
        virtual int get_raw_cursor_pos(void);
        void set_cursor_transform(int);
        int calculate_field_length(void);

        virtual void generate_string_image(void) override;
        virtual void calculate_widget_size(void) override;
        void generate_cursor(void);
        virtual vertex_buffer *generate_points(void) override;

        void init(composite *);

      public:
        explicit text_field(composite *);
        template<typename... Args>
        text_field(composite *c, Args... args)
            : rect(0, 0), active(0, 0), label(c), cursor_transform()
            {
                this->init(c);
                this->set(args...);
            };
        virtual ~text_field();

        using ui::label::get;
        virtual int get(GLuint, GLuint, GLuint *) const override;
        using ui::label::set;
        virtual void set(GLuint, GLuint, GLuint) override;

        GET_VA;
        SET_VA;

        virtual void draw(GLuint, const glm::mat4&) override;

        static void (*focus_hook)(bool);
    };
}

#endif /* __INC_CUDDLY_TEXT_FIELD_H__ */
