#include <stdexcept>

#include "../l10n.h"

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
    memcpy(g.bitmap, slot->bitmap.buffer, g.pitch * g.height);
}

void Font::cleanup_glyph(FT_ULong code)
{
    delete[] this->glyphs[code].bitmap;
}

Font::Font(FT_Library *lib, std::string& font_name, int pixel_size)
    : glyphs(font_name + " glyphs");
{
    if (FT_New_Face(*lib, font_name.c_str(), 0, this->face))
        throw std::runtime_error(_("Could not load font ") + font_name);
    FT_Set_Pixel_Sizes(this->face, 0, pixel_size);
}

Font::~Font()
{
}

struct Glyph& Font::operator[](FT_ULong code)
{
    return this->glyphs[code];
}
