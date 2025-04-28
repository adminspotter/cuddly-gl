/* label.h                                                 -*- C++ -*-
 *   by Trinity Quirk <tquirk@ymb.net>
 *
 * CuddlyGL OpenGL widget toolkit
 * Copyright (C) 2016-2018  Trinity Annabelle Quirk
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

#ifndef __INC_CUDDLY_LABEL_H__
#define __INC_CUDDLY_LABEL_H__

#include <string.h>

#include <string>

#include "widget.h"
#include "font.h"

namespace ui
{
    class label : public widget
    {
      protected:
        bool shared_font;
        std::u32string str;
        ui::image img;
        ui::base_font *font;
        GLuint tex;

        int get_font(GLuint, ui::base_font **) const;
        virtual void set_font(GLuint, ui::base_font *);
        int get_string(GLuint, std::string *) const;
        virtual void set_string(GLuint, const std::string&);
        int get_image(GLuint, ui::image *) const;
        virtual void set_image(GLuint, const ui::image&);
        virtual void set_border(GLuint, GLuint) override;
        virtual void set_margin(GLuint, GLuint) override;

        virtual void generate_string_image(void);
        virtual void calculate_widget_size(void);
        virtual vertex_buffer *generate_points(void) override;
        virtual void populate_buffers(void) override;

        void init(composite *);

      public:
        explicit label(composite *);
        template<typename... Args>
        label(composite *c, Args... args)
            : rect(0, 0), active(0, 0), widget(c), str(), img()
            {
                this->init(c);
                this->set(args...);
            };
        virtual ~label();

        using ui::widget::get;
        int get(GLuint, GLuint, ui::base_font **) const;
        int get(GLuint, GLuint, std::string *) const;
        int get(GLuint, GLuint, ui::image *) const;
        using ui::widget::set;
        virtual void set(GLuint, GLuint, ui::base_font *);
        virtual void set(GLuint a, GLuint b, ui::font *c)
            { this->set(a, b, (ui::base_font *)c); }
        virtual void set(GLuint a, GLuint b, ui::font_set *c)
            { this->set(a, b, (ui::base_font *)c); }
        virtual void set(GLuint, GLuint, const std::string&);
        virtual void set(GLuint, GLuint, const ui::image&);

        GET_VA;
        SET_VA;

        virtual void draw(GLuint, const glm::mat4&) override;
    };
}

#endif /* __INC_CUDDLY_LABEL_H__ */
