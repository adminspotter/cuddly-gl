/* label.h                                                 -*- C++ -*-
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 28 Oct 2016, 07:55:12 tquirk
 *
 * CuddlyGL OpenGL widget toolkit
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

#include <string.h>

#include <string>

#include "widget.h"
#include "font.h"

namespace ui
{
    class label : public widget
    {
      protected:
        bool use_text, shared_font;
        std::u32string str;
        image img;
        ui::font *font;
        GLuint tex;

        int get_font(GLuint, void *);
        virtual void set_font(GLuint, void *);
        int get_string(GLuint, void *);
        virtual void set_string(GLuint, void *);
        int get_image(GLuint, void *);
        virtual void set_image(GLuint, void *);
        virtual void set_border(GLuint, void *) override;
        virtual void set_margin(GLuint, void *) override;

        static std::u32string utf8tou32str(const std::string&);
        static std::string u32strtoutf8(const std::u32string&);

        virtual void generate_string_image(void);
        virtual void calculate_widget_size(void);
        virtual vertex_buffer *generate_points(void) override;
        virtual void populate_buffers(void) override;

      public:
        label(composite *, GLuint = 0, GLuint = 0);
        virtual ~label();

        virtual int get(GLuint, GLuint, void *) override;
        virtual void set(GLuint, GLuint, void *) override;

        virtual void draw(GLuint, const glm::mat4&) override;
    };
}

#endif /* __INC_R9_LABEL_H__ */
