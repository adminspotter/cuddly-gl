/* font.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 04 Jun 2016, 08:03:03 tquirk
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
 * This file contains the implementation for the font handling, via
 * the Freetype library.
 *
 * We include a static Freetype library singleton with reference
 * counting in here, so that we don't have to think about the library
 * anywhere else.
 *
 * Since we have kerning data available to us in the font, and it
 * would be difficult to represent that to an external caller, we'll
 * go ahead and render strings here.
 *
 * We want to seamlessly handle L-to-R, R-to-L, T-to-B, and B-to-T, so
 * when trying to decide what the order is, we'll key off the first
 * character in the string.  I'm not sure whether there are any fonts
 * which render vertically by default, but it's possible - traditional
 * Japanese renders T-to-B, R-to-L.  It's not clear how (or even
 * whether) the line-by-line flow is represented.  Are there any
 * languages or scripts out there which render lines B-to-T?
 *
 * Things to do
 *
 */

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <stdexcept>
#include <algorithm>

#include "font.h"

#include "../configdata.h"
#include "../l10n.h"

/* We'll keep a single library handle, and a refcount.  This will all
 * be handled within the same thread, so no multi-thread weirdness
 * should intrude.
 */
static FT_Library ft_lib;
static int ft_lib_count = 0;

static FT_Library *init_freetype(void)
{
    if (ft_lib_count++ == 0)
        FT_Init_FreeType(&ft_lib);

    return &ft_lib;
}

static void cleanup_freetype(void)
{
    if (--ft_lib_count == 0)
        FT_Done_FreeType(ft_lib);
}

std::string Font::search_path(std::string& font_name)
{
    std::vector<std::string>::iterator i;
    struct stat st;

    for (i = config.font_paths.begin(); i != config.font_paths.end(); ++i)
    {
        std::string path = *i;
        std::string::size_type pos;

        /* Make sure the path doesn't have a ~, which stat won't
         * understand
         */
        if ((pos = path.find('~')) != std::string::npos)
        {
            std::string home = getenv("HOME");

            if (home.size() == 0)
                throw std::runtime_error(_("Could not find home directory"));
            path.replace(pos, 1, home);
        }
        path += '/' + font_name;
        if (stat(path.c_str(), &st) != -1)
            return path;
    }
    throw std::runtime_error(_("Could not find font ") + font_name);
}

void Font::load_glyph(FT_ULong code)
{
    FT_GlyphSlot slot = this->face->glyph;

    FT_Load_Char(this->face, code, FT_LOAD_RENDER);
    Glyph& g = this->glyphs[code];
    /* Advance is represented in 26.6 format, so throw away the
     * lowest-order 6 bits.
     */
    g.x_advance = slot->advance.x >> 6;
    g.y_advance = slot->advance.y >> 6;
    g.width = slot->bitmap.width;
    g.height = slot->bitmap.rows;
    g.left = slot->bitmap_left;
    g.top = slot->bitmap_top;
    g.pitch = slot->bitmap.pitch;
    g.bitmap = new unsigned char[abs(g.pitch) * g.height];
    memcpy(g.bitmap, slot->bitmap.buffer, abs(g.pitch) * g.height);
}

/* This gets a little complicated, because a glyph which has no
 * descender could have an overall height that is equal to a shorter
 * glyph that has a descender.  They would evaluate as equal, but the
 * descender would expand the line height, and we wouldn't be able to
 * account for that with a single value.  So it sounds like we need
 * two values for each axis; one of the values for horizontal size
 * with horizontal text should be zero, and the values for vertical
 * size are the ascender and descender respectively.
 *
 * Vertical text may have similar problems with the vertical baseline,
 * which we'll have to account for.  We'll make this as fully general
 * as possible, so we never have to change it.
 */
void Font::get_string_size(const std::u32string& str,
                           unsigned int& w,
                           unsigned int& h)
{
    std::vector<int> req_size = {0, 0, 0, 0};
    std::u32string::const_iterator i;
    FT_Vector kern;

    for (i = str.begin(); i != str.end(); ++i)
    {
        Glyph& g = this->glyphs[*i];
        if (g.y_advance == 0)
        {
            /* Horizontal text */
            req_size[0] += abs(g.x_advance) + g.left;
            if (i + 1 != str.end()
                && FT_Get_Kerning(this->face,
                                  *i,
                                  *(i + 1),
                                  FT_KERNING_DEFAULT,
                                  &kern) == 0)
                req_size[1] += kern.x;
            req_size[2] = std::max(req_size[1], g.top);
            req_size[3] = std::min(req_size[2], g.top - g.height);
        }
        else if (g.x_advance == 0)
        {
            /* Vertical text */
            req_size[0] = std::max(req_size[0], g.left);
            req_size[1] = std::min(req_size[1], g.left - g.width);
            req_size[2] += abs(g.y_advance) + g.top;
            /* FT doesn't supply kerning info for vertical layouts */
        }
    }
    w = req_size[0] + req_size[1];
    h = req_size[2] + req_size[3];
}

Font::Font(std::string& font_name, int pixel_size)
    : glyphs(font_name + " glyphs")
{
    FT_Library *lib = init_freetype();
    std::string font_path = this->search_path(font_name);

    if (FT_New_Face(*lib, font_path.c_str(), 0, &this->face))
        throw std::runtime_error(_("Could not load font ") + font_name);
    FT_Set_Pixel_Sizes(this->face, 0, pixel_size);
}

Font::~Font()
{
    FT_Done_Face(this->face);
    cleanup_freetype();
}

struct Glyph& Font::operator[](FT_ULong code)
{
    Glyph& g = this->glyphs[code];

    if (g.bitmap == NULL)
        this->load_glyph(code);
    return g;
}

unsigned char *Font::render_string(const std::u32string& str,
                                   unsigned int& w,
                                   unsigned int& h)
{
    unsigned char *img = NULL;

    this->get_string_size(str, w, h);
    img = new unsigned char[w * h];
    return img;
}
