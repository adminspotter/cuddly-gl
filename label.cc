/* label.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 20 Jun 2016, 17:01:26 tquirk
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

#include <stdlib.h>

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
    *((std::string *)v) = u32strtoutf8(this->str);
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
    this->str = utf8tou32str(*((std::string *)v));
    if (this->font != NULL)
        this->image = this->font->render_string(this->str,
                                                this->width,
                                                this->height);
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
    this->str.clear();
    if (this->image != NULL)
        delete[] this->image;
    this->image = new unsigned char[this->width * this->height * 4];
    memcpy(this->image, v, this->width * this->height * 4);
}

/* We need to be able to convert from UTF-8 representation to
 * actual Unicode code points and back.  All network traffic
 * should be in UTF-8, and we'll of course need to display things
 * in whatever native font the user needs.
 *
 * Ref: http://www.cprogramming.com/tutorial/unicode.html
 * Ref: https://www.cl.cam.ac.uk/~mgk25/unicode.html
 */

std::u32string ui::label::utf8tou32str(const std::string& str)
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

std::string ui::label::u32strtoutf8(const std::u32string& str)
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

void ui::label::populate_buffers(void)
{
    float vertex[160];
    GLuint element[60], text;

    if (this->image != NULL)
    {
        this->panel::generate_points(vertex, element);
        vertex[6]  = 0.0; vertex[7]  = 1.0;
        vertex[14] = 1.0; vertex[15] = 1.0;
        vertex[22] = 0.0; vertex[23] = 0.0;
        vertex[30] = 1.0; vertex[31] = 0.0;
        glBindVertexArray(this->vao);
        glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
        glBufferData(GL_ARRAY_BUFFER,
                     sizeof(float) * this->vertex_count, vertex,
                     GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     sizeof(GLuint) * this->element_count, element,
                     GL_DYNAMIC_DRAW);
        glBindTexture(GL_TEXTURE_2D, this->tex);
        this->parent->get(ui::element::attribute,
                          ui::attribute::use_text,
                          &text);

        if (this->use_text)
        {
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
                         this->width, this->height, 0, GL_RED,
                         GL_UNSIGNED_BYTE, this->image);
            glUniform1i(text, 1);
        }
        else
        {
            glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                         this->width, this->height, 0, GL_RGBA,
                         GL_UNSIGNED_BYTE, this->image);
            glUniform1i(text, 0);
        }
    }
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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    this->populate_buffers();
}

ui::label::~label()
{
    glDeleteTextures(1, &this->tex);
    if (this->image != NULL)
        delete[] this->image;
    if (this->font != NULL)
        delete this->font;
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
