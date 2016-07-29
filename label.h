/* label.h                                                 -*- C++ -*-
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 29 Jul 2016, 06:35:17 tquirk
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

#include <string.h>

#include <string>

#include "panel.h"
#include "font.h"

namespace ui
{
    struct image
    {
        unsigned char *data;
        GLuint width, height, per_pixel;

        image& operator=(const image& i)
            {
                GLuint new_size = i.width * i.height * i.per_pixel;

                if (this->data != NULL)
                    delete[] this->data;
                this->width = i.width;
                this->height = i.height;
                this->per_pixel = i.per_pixel;
                this->data = new unsigned char[new_size];
                memcpy(this->data, i.data, new_size);
                return *this;
            };
    };

    class label : public panel
    {
      protected:
        bool use_text;
        std::u32string str;
        image img;
        ui::font *font;
        GLuint tex;

        int get_font(GLuint, void *);
        void set_font(GLuint, void *);
        int get_string(GLuint, void *);
        virtual void set_string(GLuint, void *);
        int get_bgimage(GLuint, void *);
        virtual void set_bgimage(GLuint, void *);

        static std::u32string utf8tou32str(const std::string&);
        static std::string u32strtoutf8(const std::u32string&);

        virtual void populate_buffers(void);

      public:
        label(context *, GLuint = 0, GLuint = 0);
        virtual ~label();

        virtual int get(GLuint, GLuint, void *);
        virtual void set(GLuint, GLuint, void *);

        virtual void draw(void);
    };
}

#endif /* __INC_R9_LABEL_H__ */
