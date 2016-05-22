/* font.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 22 May 2016, 09:18:29 tquirk
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
 * Things to do
 *
 */

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <stdexcept>

#include "../configdata.h"
#include "../l10n.h"

/* We'll keep a single library handle, and a refcount.  This will all
 * be handled within the same thread, so no multi-thread weirdness
 * should intrude.
 */
static FT_Library *ft_lib = NULL;
static int ft_lib_count = 0;

static FT_Library *init_freetype(void)
{
    if (ft_lib == NULL)
        FT_Init_FreeType(ft_lib);

    ++ft_lib_count;
    return ft_lib;
}

static void cleanup_freetype(void)
{
    if (ft_lib != NULL)
        if (--ft_lib_count == 0)
        {
            FT_Done_FreeType(*ft_lib);
            ft_lib = NULL;
        }
}

std::string Font::search_path(std::string& font_name)
{
    std::vector<std::string>::iterator i;
    struct stat st;

    for (i = config.font_paths.begin(); i != config.font_paths.end(); ++i)
    {
        std::string path = *i;

        /* Make sure the path doesn't have a ~, which stat won't
         * understand
         */
        if ((std::string::size_type pos = path.find('~')) != std::string::npos)
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

Font::Font(std::string& font_name, int pixel_size)
    : glyphs(font_name + " glyphs");
{
    FT_Library *lib = init_freetype();
    std::string font_path = this->search_path(font_name);

    if (FT_New_Face(*lib, font_path.c_str(), 0, this->face))
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
