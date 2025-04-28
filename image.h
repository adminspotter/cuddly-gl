/* image.h                                                 -*- C++ -*-
 *   by Trinity Quirk <tquirk@ymb.net>
 *
 * CuddlyGL OpenGL widget toolkit
 * Copyright (C) 2016-2019  Trinity Annabelle Quirk
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
 * Things to do
 *
 */

#ifndef __INC_CUDDLY_IMAGE_H__
#define __INC_CUDDLY_IMAGE_H__

#include <math.h>
#include <string.h>

#include <GL/gl.h>

#include <algorithm>

#include <glm/common.hpp>
#include <glm/vec4.hpp>

namespace ui
{
    typedef struct cell_tag
    {
        unsigned char r, g, b, a;

        explicit cell_tag(const glm::vec4& v)
            {
                this->r = truncf(glm::clamp(v.r, 0.0f, 1.0f) * 255);
                this->g = truncf(glm::clamp(v.g, 0.0f, 1.0f) * 255);
                this->b = truncf(glm::clamp(v.b, 0.0f, 1.0f) * 255);
                this->a = truncf(glm::clamp(v.a, 0.0f, 1.0f) * 255);
            }

        struct cell_tag& operator|=(const struct cell_tag& c)
            {
                this->r |= c.r;
                this->g |= c.g;
                this->b |= c.b;
                this->a |= c.a;
                return *this;
            }

        struct cell_tag& operator|=(const glm::vec4& v)
            {
                struct cell_tag t(v);
                return *this |= t;
            }
    } __attribute__ ((__packed__))
    cell;

    struct image
    {
        union {
            unsigned char *data;
            cell *cells;
        };

        GLuint width, height, per_pixel;

        image()
            {
                this->data = NULL;
                this->reset();
            };
        image(GLuint w, GLuint h, GLuint pp)
            {
                this->width = w;
                this->height = h;
                this->per_pixel = pp;
                this->data = new unsigned char[w * h * pp];
                memset(this->data, 0, w * h * pp);
            };
        image(const image& i)
            {
                this->data = NULL;
                *this = i;
            };
        ~image()
            {
                if (this->data != NULL)
                    delete[] this->data;
            };
        image& operator=(const image& i)
            {
                if (&i == this)
                    return *this;

                GLuint new_size = i.width * i.height * i.per_pixel;

                if (this->data != NULL)
                    delete[] this->data;
                this->width = i.width;
                this->height = i.height;
                this->per_pixel = i.per_pixel;
                if (new_size > 0)
                {
                    this->data = new unsigned char[new_size];
                    memcpy(this->data, i.data, new_size);
                }
                return *this;
            };
        void reset(void)
            {
                this->width = 0;
                this->height = 0;
                this->per_pixel = 0;
                if (this->data != NULL)
                    delete[] this->data;
                this->data = NULL;
            };
    };

}

#endif /* __INC_CUDDLY_IMAGE_H__ */
