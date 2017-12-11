/* font.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 10 Dec 2017, 09:17:48 tquirk
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
 * We want to seamlessly handle mixed L-to-R and R-to-L text.  There
 * may be cases in an L-to-R language, in which R-to-L text is
 * included, or vice versa, that we want to handle.  It seems like
 * most of the vertical languages have horizontal forms, so we're
 * going to make a command decision and render everything in
 * horizontal mode.  It would be nice to handle verticals natively,
 * but it's going to be a nightmare to implement in a reasonable way.
 *
 * There do not appear to be any horizontal scripts which flow their
 * lines B-to-T, so we can at least be safe with a T-to-B line flow
 * assumption.
 *
 * Ref: http://www.omniglot.com/writing/direction.htm
 *
 * Things to do
 *
 */

#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <set>
#include <stdexcept>
#include <algorithm>

#include "font.h"
#ifdef TT_CONFIG_OPTION_EMBEDDED_BITMAPS
#include FT_TRUETYPE_TABLES_H
#include FT_TRUETYPE_TAGS_H
#endif /* TT_CONFIG_OPTION_EMBEDDED_BITMAPS */
#include FT_GLYPH_H

/* We'll keep a single library handle, and a refcount.  This will all
 * be handled within the same thread, so no multi-thread weirdness
 * should intrude.
 */
static FT_Library ft_lib;
static int ft_lib_count = 0;

/* Instead of having a giant if-condition, we'll do some sensible
 * searching with a set.  There are a handful of single code points
 * which are r-to-l, and a number of ranges.  We'll make ranges out of
 * the single points, with the same beginning and end, and our
 * comparisons should still hold.
 */
struct range
{
    uint32_t start, end;

    range(uint32_t a, uint32_t b) { this->start = a; this->end = b; };
};

static bool operator<(const range& a, const range& b)
{
    if (a.end < b.start)
        return true;
    return false;
}

static std::set<range> r_to_l_ranges =
{
    {0x05be, 0x05be}, {0x05c0, 0x05c0}, {0x05c3, 0x05c3}, {0x05c6, 0x05c6},
    {0x05d0, 0x05f4}, {0x0608, 0x0608}, {0x060b, 0x060b}, {0x060d, 0x060d},
    {0x061b, 0x064a}, {0x066d, 0x066f}, {0x0671, 0x06d5}, {0x06e5, 0x06e6},
    {0x06ee, 0x06ef}, {0x06fa, 0x0710}, {0x0712, 0x072f}, {0x074d, 0x07a5},
    {0x07b1, 0x07ea}, {0x07f4, 0x07f5}, {0x07fa, 0x0815}, {0x081a, 0x081a},
    {0x0824, 0x0824}, {0x0828, 0x0828}, {0x0830, 0x0858}, {0x085e, 0x08ac},
    {0x200f, 0x200f}, {0xfb1d, 0xfb1d}, {0xfb1f, 0xfb28}, {0xfb2a, 0xfd3d},
    {0xfd50, 0xfdfc}, {0xfe70, 0xfefc}, {0x10800, 0x1091b}, {0x10920, 0x10a00},
    {0x10a10, 0x10a33}, {0x10a40, 0x10b35}, {0x10b40, 0x10c48},
    {0x1ee00, 0x1eebb}
};

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

/* The TTF format, or at least Freetype, doesn't distinguish between
 * L-to-R and R-to-L characters.  Seems that they could provide a
 * negative advance value for R-to-L, but whatever.  Our R-to-L set
 * should provide the ranges which are R-to-L based on Unicode code
 * point.
 */
bool ui::glyph::is_l_to_r(void)
{
    if (r_to_l_ranges.find(range(this->code_point, this->code_point))
        == r_to_l_ranges.end())
        return true;
    return false;
}

/* We need to be able to convert from UTF-8 representation to
 * actual Unicode code points and back.  All network traffic
 * should be in UTF-8, and we'll of course need to display things
 * in whatever native font the user needs.
 *
 * Ref: http://www.cprogramming.com/tutorial/unicode.html
 * Ref: https://www.cl.cam.ac.uk/~mgk25/unicode.html
 */
std::u32string ui::utf8tou32str(const std::string& str)
{
    std::string::const_iterator i = str.begin();
    std::u32string newstr;
    uint32_t ch;

    while (i != str.end())
    {
        if ((*i & 0xfe) == 0xfc)
        {
            ch = (*i & 0x01) << 30;
            ch |= (*(++i) & 0x3f) << 24;
            ch |= (*(++i) & 0x3f) << 18;
            ch |= (*(++i) & 0x3f) << 12;
            ch |= (*(++i) & 0x3f) << 6;
            ch |= (*(++i) & 0x3f);
        }
        else if ((*i & 0xfc) == 0xf8)
        {
            ch = (*i & 0x03) << 24;
            ch |= (*(++i) & 0x3f) << 18;
            ch |= (*(++i) & 0x3f) << 12;
            ch |= (*(++i) & 0x3f) << 6;
            ch |= (*(++i) & 0x3f);
        }
        else if ((*i & 0xf8) == 0xf0)
        {
            ch = (*i & 0x07) << 18;
            ch |= (*(++i) & 0x3f) << 12;
            ch |= (*(++i) & 0x3f) << 6;
            ch |= (*(++i) & 0x3f);
        }
        else if ((*i & 0xf0) == 0xe0)
        {
            ch = (*i & 0x0f) << 12;
            ch |= (*(++i) & 0x3f) << 6;
            ch |= (*(++i) & 0x3f);
        }
        else if ((*i & 0xe0) == 0xc0)
        {
            ch = (*i & 0x1f) << 6;
            ch |= (*(++i) & 0x3f);
        }
        else if ((*i & 0x80) == 0x00)
            ch = *i;
        else
            ch = '.';

        newstr.push_back(ch);
        ++i;
    }
    return newstr;
}

std::string ui::u32strtoutf8(const std::u32string& str)
{
    std::u32string::const_iterator i = str.begin();
    std::string newstr;

    while (i != str.end())
    {
        if (*i & 0x7c000000)
        {
            newstr.push_back(0xfc | ((*i & 0x40000000) >> 30));
            newstr.push_back(0x80 | ((*i & 0x3f000000) >> 24));
            newstr.push_back(0x80 | ((*i & 0xfc0000) >> 18));
            newstr.push_back(0x80 | ((*i & 0x3f000) >> 12));
            newstr.push_back(0x80 | ((*i & 0xfc0) >> 6));
            newstr.push_back(0x80 | (*i & 0x3f));
        }
        else if (*i & 0x3e00000)
        {
            newstr.push_back(0xf8 | ((*i & 0x3000000) >> 24));
            newstr.push_back(0x80 | ((*i & 0xfc0000) >> 18));
            newstr.push_back(0x80 | ((*i & 0x3f000) >> 12));
            newstr.push_back(0x80 | ((*i & 0xfc0) >> 6));
            newstr.push_back(0x80 | (*i & 0x3f));
        }
        else if (*i & 0x1f0000)
        {
            newstr.push_back(0xf0 | ((*i & 0x1c0000) >> 18));
            newstr.push_back(0x80 | ((*i & 0x3f000) >> 12));
            newstr.push_back(0x80 | ((*i & 0xfc0) >> 6));
            newstr.push_back(0x80 | (*i & 0x3f));
        }
        else if (*i & 0xf800)
        {
            newstr.push_back(0xe0 | ((*i & 0x1f000) >> 12));
            newstr.push_back(0x80 | ((*i & 0xfc0) >> 6));
            newstr.push_back(0x80 | (*i & 0x3f));
        }
        else if (*i & 0x780)
        {
            newstr.push_back(0xc0 | ((*i & 0x7c0) >> 6));
            newstr.push_back(0x80 | (*i & 0x3f));
        }
        else
        {
            newstr.push_back(*i & 0x7f);
        }
        ++i;
    }

    return newstr;
}

std::string ui::base_font::search_path(std::string& font_name,
                                       ui::search_paths& paths)
{
    ui::search_paths::iterator i;
    struct stat st;

    for (i = paths.begin(); i != paths.end(); ++i)
    {
        std::string path = *i;
        std::string::size_type pos;

        /* Make sure the path doesn't have a ~, which stat won't
         * understand
         */
        if ((pos = path.find('~')) != std::string::npos)
        {
            char *home;
            std::string home_str;

            if ((home = getenv("HOME")) == NULL)
                throw std::runtime_error("Could not find home directory");
            home_str = home;
            path.replace(pos, 1, home_str);
        }
        path += '/' + font_name;
        if (stat(path.c_str(), &st) != -1)
            return path;
    }
    throw std::runtime_error("Could not find font " + font_name);
}

#ifdef TT_CONFIG_OPTION_EMBEDDED_BITMAPS
void ui::base_font::setup_bitmap_face(FT_Face face, int pixel_size)
{
    int best_match = 0;
    int diff = abs(pixel_size - face->available_sizes[0].height);

    for (int i = 1; i < face->num_fixed_sizes; ++i)
    {
        int ndiff = abs(pixel_size - face->available_sizes[i].height);
        if (ndiff < diff)
        {
            best_match = i;
            diff = ndiff;
        }
    }
    FT_Select_Size(face, best_match);
}
#endif /* TT_CONFIG_OPTION_EMBEDDED_BITMAPS */

FT_Face ui::base_font::init_face(std::string& fname,
                                 int pixel_size,
                                 ui::search_paths& paths)
{
    FT_Library *lib = init_freetype();
    FT_Face face;
    std::string font_path = this->search_path(fname, paths);

    if (FT_New_Face(*lib, font_path.c_str(), 0, &face))
        throw std::runtime_error("Could not load font " + fname);

#ifdef TT_CONFIG_OPTION_EMBEDDED_BITMAPS
    FT_ULong length = 0;

    if (!FT_Load_Sfnt_Table(face, TTAG_CBDT, 0, NULL, &length) && length)
        this->setup_bitmap_face(face, pixel_size);
    else
#endif /* TT_CONFIG_OPTION_EMBEDDED_BITMAPS */
        FT_Set_Pixel_Sizes(face, 0, pixel_size);
    return face;
}

void ui::base_font::cleanup_face(FT_Face face)
{
    FT_Done_Face(face);
    cleanup_freetype();
}

void ui::base_font::load_glyph(FT_Face face, FT_ULong code)
{
    FT_UInt index = FT_Get_Char_Index(face, code);
    if (index == 0)
        return;

    if (FT_Load_Char(face, code, FT_LOAD_RENDER)
        && FT_Load_Glyph(face, index, FT_LOAD_COLOR))
        return;

    ui::glyph& g = this->glyphs[code];
    FT_GlyphSlot slot = face->glyph;

    g.face = face;
    g.code_point = code;
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

void ui::base_font::kern(FT_ULong a, FT_ULong b, FT_Vector *k)
{
    if (this->glyphs[a].face != this->glyphs[b].face
        || FT_Get_Kerning(this->glyphs[a].face, a, b, FT_KERNING_DEFAULT, k))
        k->x = k->y = 0;
    /* Kerning in default mode is 26.6 format */
    k->x >>= 6;
    k->y >>= 6;
}

void ui::base_font::get_max_glyph_box(FT_Face face,
                                      int *box_w, int *box_a, int *box_d)
{
    *box_w = face->size->metrics.max_advance >> 6;
    *box_a = face->size->metrics.ascender >> 6;
    *box_d = -(face->size->metrics.descender >> 6);
}

ui::base_font::base_font(std::string& name)
    : glyphs(name + " glyphs")
{
}

ui::base_font::~base_font()
{
}

void ui::base_font::max_cell_size(std::vector<int>& v)
{
    v[0] = this->bbox_w;
    v[1] = this->bbox_a;
    v[2] = this->bbox_d;
}

/* This gets a little complicated, because a glyph which has no
 * descender could have an overall height that is equal to a shorter
 * glyph that has a descender.  They would evaluate as equal, but the
 * descender would expand the line height, and we wouldn't be able to
 * account for that with a single value.  So we need two values for
 * the vertical axis; the values for vertical size are the ascender
 * and descender respectively.
 *
 * Return values come back in the req_size argument.  First element is
 * width, second is ascender, third is descender.
 *
 * It would be nice if we could support vertical lines, but it's going
 * to be way too difficult, and most of the currently-used vertical
 * languages have horizontal usage nowadays.
 */
void ui::base_font::get_string_size(const std::u32string& str,
                                    std::vector<int>& req_size)
{
    std::u32string::const_iterator i;

    req_size[0] = req_size[1] = req_size[2] = 0;
    for (i = str.begin(); i != str.end(); ++i)
    {
        FT_Vector kerning = {0, 0};
        ui::glyph& g = (*this)[*i];

        if (g.x_advance == 0 && g.y_advance != 0)
            throw std::runtime_error("This font is not supported.");
        if (i != str.begin())
            this->kern(*(i - 1), *i, &kerning);

        /* We're only going to do horizontal text */
        req_size[0] += kerning.x;
        if (i != str.begin())
            req_size[0] += g.left;
        if (i + 1 == str.end() && g.width != 0)
            req_size[0] += g.width;
        else
            req_size[0] += g.x_advance;
        req_size[1] = std::max(req_size[1], g.top);
        req_size[2] = std::max(req_size[2], g.height - g.top);
    }
}

void ui::base_font::render_string(const std::u32string& str, ui::image& img)
{
    std::vector<int> req_size = {0, 0, 0};
    std::u32string::const_iterator i = str.begin();
    bool l_to_r = (*this)[*i].is_l_to_r();
    int pos, save_pos;

    this->get_string_size(str, req_size);
    img.reset();
    img.width = req_size[0];
    img.height = req_size[1] + req_size[2];
    img.per_pixel = 1;
    img.data = new unsigned char[img.width * img.height];
    memset(img.data, 0, img.width * img.height);
    save_pos = pos = (l_to_r ? 0 : img.width - 1);

    /* GL does positive y as up, so it makes more sense to just draw
     * the buffer upside-down.  All the glyphs are already
     * upside-down.
     */
    while (i != str.end())
    {
        ui::glyph& g = (*this)[*i];
        int j, k, bottom_row = req_size[2] + g.top - g.height;
        int row_offset, glyph_offset;
        FT_Vector kerning = {0, 0};
        bool same_dir = (l_to_r == g.is_l_to_r());

        /* Kerning, if available */
        if (i != str.begin())
            this->kern(*(i - 1), *i, &kerning);

        /* If we're dealing with wrong-direction text, scoot things over */
        if (!same_dir && pos != save_pos)
        {
            int x_move = abs(pos - save_pos);
            int x_distance = g.x_advance + kerning.x;
            int start = std::min(pos, save_pos);
            for (j = 0; j < img.height; ++j)
            {
                row_offset = (img.width * j) + start;
                memmove(&img.data[row_offset
                                  + (!l_to_r ? -x_distance : x_distance)],
                        &img.data[row_offset],
                        x_move);
                memset(&img.data[row_offset - start + save_pos
                                 - (!l_to_r ? x_distance : 0)],
                       0,
                       x_distance);
            }
        }
        else
            save_pos = pos;
        if (!l_to_r)
        {
            pos -= g.x_advance + kerning.x;
            if (i != str.begin())
                pos -= g.left;
            if (same_dir)
                save_pos = pos;
        }
        for (j = 0; j < g.height; ++j)
        {
            row_offset = (bottom_row + j + kerning.y) * img.width
                + save_pos + kerning.x
                - (!l_to_r && !same_dir ? g.x_advance : 0);
            if (i != str.begin())
                row_offset += g.left;
            glyph_offset = (g.height - 1 - j) * g.width;
            for (k = 0; k < g.width; ++k)
                img.data[row_offset + k] |= g.bitmap[glyph_offset + k];
        }
        if (l_to_r)
        {
            pos += g.x_advance + kerning.x;
            if (i != str.begin())
                pos += g.left;
            if (same_dir)
                save_pos = pos;
        }

        ++i;
    }
}

void ui::base_font::render_multiline_string(const std::vector<std::u32string>& strs,
                                            ui::image& img)
{
    std::vector<int> req_size = {0, 0, 0};
    ui::image *imgs = new ui::image[strs.size()];
    int str_count = 0, line_height = this->line_height();

    img.reset();
    img.width = 0;
    for (auto i = strs.begin(); i != strs.end(); ++i, ++str_count)
    {
        this->render_string(*i, imgs[str_count]);
        img.width = std::max(img.width, imgs[str_count].width);
    }

    /* We'll keep our line spacing consistent to what is contained in
     * the font records, so we need n - 1 line-spacings, plus the
     * ascender of the top string, and the descender of the bottom
     * string.
     */
    img.height = (str_count - 1) * line_height;
    this->get_string_size(strs.back(), req_size);
    img.height += req_size[2];
    if (str_count > 1)
        this->get_string_size(strs.front(), req_size);
    img.height += req_size[1];

    img.per_pixel = 1;
    img.data = new unsigned char[img.width * img.height];
    memset(img.data, 0, sizeof(unsigned char) * img.width * img.height);

    /* Now copy everything into place in the main image.  We're still
     * producing upside-down images, so we'll copy bottom-to-top.
     */
    GLuint row_num = img.height - 1;
    GLuint row_offset, prev_descender;

    for (int i = 0; i < str_count; ++i)
    {
        this->get_string_size(strs[i], req_size);
        if (i != 0)
            row_num -= line_height - prev_descender - req_size[1];
        row_offset = row_num * img.width;
        for (int j = imgs[i].height - 1;
             j >= 0;
             --j, --row_num, row_offset -= img.width)
            memcpy(&(img.data[row_offset]),
                   &(imgs[i].data[j * imgs[i].width]),
                   imgs[i].width);
        prev_descender = req_size[2];
    }

    delete[] imgs;
}

int ui::font::line_height(void)
{
    return this->face->size->metrics.height >> 6;
}

ui::font::font(std::string& font_name, int pixel_size, ui::search_paths& paths)
    : ui::base_font(font_name)
{
    this->face = this->init_face(font_name, pixel_size, paths);
    this->get_max_glyph_box(this->face,
                            &this->bbox_w, &this->bbox_a, &this->bbox_d);
}

ui::font::~font()
{
    this->cleanup_face(this->face);
}

struct ui::glyph& ui::font::operator[](FT_ULong code)
{
    ui::glyph& g = this->glyphs[code];

    if (g.bitmap == NULL)
        this->load_glyph(this->face, code);
    return g;
}

int ui::font_set::line_height(void)
{
    return this->faces[0]->size->metrics.height >> 6;
}

ui::font_set::font_set(std::string& set_name)
    : ui::base_font(set_name), faces()
{
}

ui::font_set::~font_set()
{
    for (auto i = this->faces.begin(); i != this->faces.end(); ++i)
        this->cleanup_face(*i);
}

ui::font_set& ui::font_set::operator<<(ui::font_set::font_spec& fs)
{
    this->faces.push_back(this->init_face(std::get<0>(fs),
                                          std::get<1>(fs),
                                          std::get<2>(fs)));
    int w, a, d;

    this->get_max_glyph_box(this->faces[this->faces.size() - 1], &w, &a, &d);

    this->bbox_w = std::max(this->bbox_w, w);
    this->bbox_a = std::max(this->bbox_a, a);
    this->bbox_d = std::max(this->bbox_d, d);

    return *this;
}

struct ui::glyph& ui::font_set::operator[](FT_ULong code)
{
    ui::glyph& g = this->glyphs[code];

    auto i = this->faces.begin();
    while (g.bitmap == NULL && i != this->faces.end())
    {
        this->load_glyph(*i, code);
        ++i;
    }
    return g;
}
