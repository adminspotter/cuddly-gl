/* font.h                                                  -*- C++ -*-
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 18 Nov 2017, 13:04:36 tquirk
 *
 * CuddlyGL OpenGL widget toolkit
 * Copyright (C) 2017  Trinity Annabelle Quirk
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
 * This file contains the declarations for our Freetype font handling
 * object.
 *
 * Strategy here is to keep a rendered glyph in a given size (via a
 * constructor arg, perhaps) and the relevant ancillary data (size,
 * etc.) in a cache object as we've used before.  We'll convert the
 * Unicode point into the index; Freetype uses an unsigned long
 * (FT_ULong), so that seems reasonable for us too.
 *
 * Regular cache evacuation, as with geometries and textures, seems
 * reasonable - if a glyph hasn't been used for several minutes, we
 * shouldn't assume that we'll use it again any time soon, and might
 * as well reclaim the memory.  Demand-loading of glyphs also seems
 * more than reasonable.  For large character sets (e.g. some of the
 * Asian languages), the memory savings could be significant.
 *
 * It might also be interesting to make a streambuf of some sort, with
 * some additional flags (color, etc.), so we can use the regular C++
 * stream injection operators.  We can model it off the usage of the
 * server log object, and the way we inject syslog priorities.
 *
 * Things to do
 *
 */

#ifndef __INC_CUDDLY_FONT_H__
#define __INC_CUDDLY_FONT_H__

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_TYPES_H

#include <string>
#include <vector>

#include "cache.h"
#include "image.h"

namespace ui
{
    struct glyph
    {
        FT_Face face;
        uint32_t code_point;
        int x_advance, y_advance, width, height;
        int top, left;
        int pitch;
        unsigned char *bitmap;

        bool is_l_to_r(void);
    };

    class base_font
    {
      protected:
        struct glyph_cleanup
        {
            void operator()(struct glyph& g)
                {
                    if (g.bitmap != NULL)
                        delete[] g.bitmap;
                }
        };
        BasicCache<struct glyph, glyph_cleanup, FT_ULong> glyphs;

        std::string search_path(std::string&, std::vector<std::string>&);

        void load_glyph(FT_Face, FT_ULong);
        void kern(FT_ULong, FT_ULong, FT_Vector *);

      public:
        base_font(std::string&);
        virtual ~base_font();
    };

    class font : public base_font
    {
      private:
        FT_Face face;
        int bbox_w, bbox_a, bbox_d;

        void get_max_glyph_box(void);

      public:
        font(std::string&, int, std::vector<std::string>&);
        virtual ~font();

        void max_cell_size(std::vector<int>&);

        struct glyph& operator[](FT_ULong);

        void get_string_size(const std::u32string&, std::vector<int>&);
        void render_string(const std::u32string&, image&);
        void render_multiline_string(const std::vector<std::u32string>&,
                                     image&);
    };
}

#endif /* __INC_CUDDLY_FONT_H__ */
