/* image.h                                                 -*- C++ -*-
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 24 Sep 2016, 09:25:09 tquirk
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
 * Things to do
 *
 */

#ifndef __INC_R9_IMAGE_H__
#define __INC_R9_IMAGE_H__

#include <string.h>

#include <GL/gl.h>

namespace ui
{
    struct image
    {
        unsigned char *data;
        GLuint width, height, per_pixel;

        image()
            {
                this->data = NULL;
                this->reset();
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

#endif /* __INC_R9_IMAGE_H__ */
