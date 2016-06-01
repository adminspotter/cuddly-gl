/* label.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 01 Jun 2016, 10:14:12 tquirk
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
 * This file contains the basic label object definitions.  We derive
 * from the panel, which will take care of the box part.  In this
 * class, we'll add font handling via freetype, and we'll get our text
 * on the screen via a 2D GL texture.  We will handle either text
 * rendering, or image rendering.
 *
 * Things to do
 *
 */

#include <stdexcept>

#include <glm/gtc/type_ptr.hpp>

#include "label.h"

#include "../l10n.h"

/* ARGSUSED */
int ui::label::get_font(GLuint t, void *v)
{
    v = (void *)this->font;
    return 0;
}

/* ARGSUSED */
void ui::label::set_font(GLuint t, void *v)
{
    this->font = (Font *)v;
}

/* ARGSUSED */
int ui::label::get_string(GLuint t, void *v)
{
    *((std::string *)v) = this->str;
    return 0;
}

/* ARGSUSED */
void ui::label::set_string(GLuint t, void *v)
{
    this->use_text = true;
    if (this->image != NULL)
    {
        delete[] this->image;
        this->image = NULL;
    }
    this->str = *((std::string *)v);
}

/* ARGSUSED */
int ui::label::get_bgimage(GLuint t, void *v)
{
    v = (void *)this->image;
    return 0;
}

/* ARGSUSED */
void ui::label::set_bgimage(GLuint t, void *v)
{
    if (this->width == 0 || this->height == 0)
        throw std::runtime_error(_("Width and height must be set before assigning a background image"));

    this->use_text = false;
    this->str = "";
    if (this->image != NULL)
        delete[] this->image;
    this->image = new unsigned char[this->width * this->height];
    memcpy(this->image, v, this->width * this->height);
}

/* We need to be able to convert from UTF-8 representation to
 * actual Unicode code points and back.  All network traffic
 * should be in UTF-8, and we'll of course need to display things
 * in whatever native font the user needs.
 *
 * Ref: http://www.cprogramming.com/tutorial/unicode.html
 */

std::u32string& ui::label::utf8tou32str(const std::string& str)
{
    std::string::iterator i = str.begin();
    std::u32string newstr();
    uint32_t ch;

    while (i != str.end())
    {
        if (*i & 0xf0 == 0xf0)
        {
            ch = (*i & 0x07) << 18;
            ch += (*(++i) & 0x3f) << 12;
            ch += (*(++i) & 0x3f) << 6;
            ch += (*(++i) & 0x3f);
        }
        else if (*i & 0xe0 == 0xe0)
        {
            ch = (*i & 0x0f) << 12;
            ch += (*(++i) & 0x3f) << 6;
            ch += (*(++i) & 0x3f);
        }
        else if (*i & 0xc0 == 0xc0)
        {
            ch = (*i & 0x1f) << 6;
            ch += (*(++i) & 0x3f);
        }
        else
            ch = *i;

        newstr.push_back(ch);
        ++i;
    }
    return newstr;
}

std::string& ui::label::u32strtoutf8(const std::u32string& str)
{
    std::u32string::iterator i = str.begin();
    std::string newstr;

    while (i != str.end())
    {
        if (*i >= 0x10000)
        {
            newstr.push_back(0xf0 & ((*i & 0x1c0000) >> 18));
            newstr.push_back(0x80 & ((*i & 0x3f000) >> 12));
            newstr.push_back(0x80 & ((*i & 0xfc0) >> 6));
            newstr.push_back(0x80 & (*i & 0x3f));
        }
        else if (*i >= 0x800)
        {
            newstr.push_back(0xe0 & ((*i & 0x1f000) >> 12));
            newstr.push_back(0x80 & ((*i & 0xfc0) >> 6));
            newstr.push_back(0x80 & (*i & 0x3f));
        }
        else if (*i >= 0x80)
        {
            newstr.push_back(0xc0 & ((*i & 0x7c0) >> 6));
            newstr.push_back(0x80 & (*i & 0x3f));
        }
        else
        {
            newstr.push_back(*i & 0x7f);
        }
        ++i;
    }

    return newstr;
}

void ui::label::populate_buffers(void)
{
    if (this->use_text)
    {
    }
    glBindTexture(GL_TEXTURE_2D, this->tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA,
                 this->width, this->height, 0, GL_ALPHA,
                 GL_UNSIGNED_BYTE, this->image);
}

ui::label::label(ui::context *c, GLuint w, GLuint h)
    : ui::panel::panel(c, w, h), str()
{
    this->use_text = true;
    this->image = NULL;
    this->font = NULL;
    glGenTextures(1, &this->tex);
    glBindTexture(GL_TEXTURE_2D, this->tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameterfv(GL_TEXTURE_2D,
                     GL_TEXTURE_BORDER_COLOR,
                     glm::value_ptr(this->background));
    this->populate_buffers();
}

ui::label::~label()
{
    glDeleteTextures(1, &this->tex);
    if (this->image != NULL)
        delete[] this->image;
}

int ui::label::get(GLuint e, GLuint t, void *v)
{
    switch (e)
    {
      case ui::element::font:     return this->get_font(t, v);
      case ui::element::string:   return this->get_string(t, v);
      case ui::element::bgimage:  return this->get_bgimage(t, v);
      default:                    return ui::panel::get(e, t, v);
    }
}

void ui::label::set(GLuint e, GLuint t, void *v)
{
    switch (e)
    {
      case ui::element::font:
        this->set_font(t, v);
        this->populate_buffers();
        break;

      case ui::element::string:
        this->set_string(t, v);
        this->populate_buffers();
        break;

      case ui::element::bgimage:
        this->set_bgimage(t, v);
        this->populate_buffers();
        break;

      default:
        /* This already calls populate_buffers, so no need to call it again */
        ui::panel::set(e, t, v);
        break;
    }
}

void ui::label::draw(void)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, this->tex);
    ui::panel::draw();
    glDisable(GL_TEXTURE_2D);
}
