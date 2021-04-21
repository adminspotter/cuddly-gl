/* font.h                                                  -*- C++ -*-
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 29 Nov 2019, 15:19:26 tquirk
 *
 * CuddlyGL OpenGL widget toolkit
 * Copyright (C) 2019  Trinity Annabelle Quirk
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
#include FT_CONFIG_OPTIONS_H
#include FT_FREETYPE_H
#include FT_TYPES_H

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include <string>
#include <vector>
#include <tuple>

#include "cache.h"
#include "image.h"
#include "bidi.h"

namespace ui
{
    struct glyph
    {
        FT_Face face;
        uint32_t code_point;
        int x_advance, y_advance, width, height;
        int top, left;
        int pitch;
        int per_pixel;
        union
        {
            unsigned char *bitmap;
            cell *cells;
        };

        void copy_to_image(image&, const glm::ivec2&, const glm::vec4&, bool);
    };

    typedef std::vector<std::string> search_paths;

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
        int bbox_w, bbox_a, bbox_d;

        static std::string search_path(std::string&, search_paths&);

#ifdef TT_CONFIG_OPTION_EMBEDDED_BITMAPS
        static void setup_bitmap_face(FT_Face, int);
#endif /* TT_CONFIG_OPTION_EMBEDDED_BITMAPS */

        static FT_Face init_face(std::string&, int, search_paths&);
        static void cleanup_face(FT_Face);

        void load_glyph(FT_Face, FT_ULong);
        void kern(FT_ULong, FT_ULong, FT_Vector *);
        virtual int line_height(void) = 0;

        static void get_max_glyph_box(FT_Face, int *, int *, int *);

        image render(const std::vector<bidi::mirror_t>&,
                     const glm::vec4&, const glm::vec4&);

      public:
        explicit base_font(std::string&);
        virtual ~base_font();

        void max_cell_size(int&, int&);
        void max_cell_size(int&, int&, int&);

        virtual struct glyph& operator[](FT_ULong) = 0;

        void get_string_size(const std::u32string&, GLuint&, GLuint&);
        void get_string_size(const std::u32string&, GLuint&, GLuint&, GLuint&);
        void get_string_size(const std::vector<bidi::mirror_t>&,
                             GLuint&, GLuint&, GLuint&);
        image render_string(const std::u32string&,
                            const glm::vec4&,
                            const glm::vec4&);
    };

    class font : public base_font
    {
      private:
        FT_Face face;

        virtual int line_height(void) override;

      public:
        font(std::string&, int, search_paths&);
        virtual ~font();

        virtual struct glyph& operator[](FT_ULong) override;
    };

    class font_set : public base_font
    {
      public:
        typedef std::tuple<std::string&, int, search_paths&> font_spec;

      private:
        std::vector<FT_Face> faces;

        virtual int line_height(void) override;

      public:
        explicit font_set(std::string&);
        virtual ~font_set();

        font_set& operator<<(font_spec&);
        virtual struct glyph& operator[](FT_ULong) override;
    };
}

#endif /* __INC_CUDDLY_FONT_H__ */
