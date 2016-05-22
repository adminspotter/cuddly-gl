/* font.h                                                  -*- C++ -*-
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 21 May 2016, 12:26:41 tquirk
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

#ifndef __INC_R9_FONT_H__
#define __INC_R9_FONT_H__

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_TYPES_H

#include "../cache.h"

struct Glyph
{
    int x_min, x_max, x_advance, x_offset, width;
    int y_min, y_max, y_advance, y_offset, height;
    int top, left;
    unsigned char *bitmap;
}

class Font
{
  private:
    static struct glyph_cleanup
    {
        void operator()(struct Glyph& g)
            {
                if (g.bitmap != NULL)
                    delete[] g.bitmap;
            }
    };

    FT_Face face;
    BasicCache<struct Glyph, glyph_cleanup, FT_ULong> glyphs;

    std::string search_path(std::string&);

    void load_glyph(FT_ULong);

  public:
    Font(std::string&, int);
    ~Font();

    struct Glyph& operator[](FT_ULong);
};

#endif /* __INC_R9_FONT_H__ */
