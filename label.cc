/* label.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 12 Nov 2016, 07:19:17 tquirk
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
 * from the widget, which will take care of the box part.  In this
 * class, we'll add font handling via freetype, and we'll get our text
 * on the screen via a 2D GL texture.  We will handle either text
 * rendering, or image rendering.
 *
 * Things to do
 *
 */

#include <stdlib.h>

#include <glm/gtc/type_ptr.hpp>

#include "ui_defs.h"
#include "label.h"

/* ARGSUSED */
int ui::label::get_font(GLuint t, void *v)
{
    v = (void *)this->font;
    return 0;
}

/* ARGSUSED */
void ui::label::set_font(GLuint t, void *v)
{
    if (t == ui::ownership::shared)
        this->shared_font = true;
    else
        this->shared_font = false;
    this->font = (ui::font *)v;
    if (this->str.size() > 0)
        this->generate_string_image();
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
    this->str = utf8tou32str(*((std::string *)v));
    if (this->font != NULL)
        this->generate_string_image();
}

/* ARGSUSED */
int ui::label::get_image(GLuint t, void *v)
{
    v = (void *)&this->img;
    return 0;
}

/* ARGSUSED */
void ui::label::set_image(GLuint t, void *v)
{
    this->use_text = false;
    this->str.clear();
    this->img = *(ui::image *)v;
    this->calculate_widget_size();
}

void ui::label::set_border(GLuint t, void *v)
{
    this->widget::set_border(t, v);
    this->calculate_widget_size();
}

void ui::label::set_margin(GLuint t, void *v)
{
    this->widget::set_margin(t, v);
    this->calculate_widget_size();
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

void ui::label::generate_string_image(void)
{
    if (this->use_text == true && this->font != NULL)
        this->font->render_string(this->str, this->img);
    this->calculate_widget_size();
}

void ui::label::calculate_widget_size(void)
{
    glm::ivec2 size;

    /* We want an extra pixel of space between the string and each
     * side, even if there is no border or margin, thus the
     * literal 2s.
     */
    size.x = this->img.width
        + this->margin[1] + this->margin[2]
        + this->border[1] + this->border[2] + 2;
    size.y = this->img.height
        + this->margin[0] + this->margin[3]
        + this->border[0] + this->border[3] + 2;
    this->set_size(ui::size::all, &size);
}

ui::vertex_buffer *ui::label::generate_points(void)
{
    ui::vertex_buffer *vb = this->widget::generate_points();
    float pw, ph, m[4], b[4];

    if (this->img.data == NULL)
        return vb;

    pw = 1.0f / (float)this->img.width;
    ph = 1.0f / (float)this->img.height;
    m[0] = this->margin[0] * ph;  b[0] = this->border[0] * ph;
    m[1] = this->margin[1] * pw;  b[1] = this->border[1] * pw;
    m[2] = this->margin[2] * pw;  b[2] = this->border[2] * pw;
    m[3] = this->margin[3] * ph;  b[3] = this->border[3] * ph;

    vb->vertex[6]  = 0.0f - m[1] - b[1] - pw;
    vb->vertex[7]  = 1.0f + m[0] + b[0] + ph
        + ((this->dim.y - this->margin[0] - this->margin[3]
            - this->border[0] - this->border[3] - 2 - this->img.height)
           * ph);

    vb->vertex[14] = 1.0f + m[2] + b[2] + pw
        + ((this->dim.x - this->margin[1] - this->margin[2]
            - this->border[1] - this->border[2] - 2 - this->img.width)
           * pw);
    vb->vertex[15] = vb->vertex[7];

    vb->vertex[22] = vb->vertex[6];
    vb->vertex[23] = 0.0f - m[3] - b[3] - ph;

    vb->vertex[30] = vb->vertex[14];
    vb->vertex[31] = vb->vertex[23];

    memcpy(&(vb->vertex[2]),
           glm::value_ptr(this->foreground), sizeof(float) * 4);
    memcpy(&(vb->vertex[10]),
           glm::value_ptr(this->foreground), sizeof(float) * 4);
    memcpy(&(vb->vertex[18]),
           glm::value_ptr(this->foreground), sizeof(float) * 4);
    memcpy(&(vb->vertex[26]),
           glm::value_ptr(this->foreground), sizeof(float) * 4);

    return vb;
}

void ui::label::populate_buffers(void)
{
    this->widget::populate_buffers();
    if (this->img.data != NULL)
    {
        glBindTexture(GL_TEXTURE_2D, this->tex);

        if (this->use_text)
        {
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
                         this->img.width, this->img.height, 0, GL_RED,
                         GL_UNSIGNED_BYTE, this->img.data);
        }
        else
        {
            glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                         this->img.width, this->img.height, 0, GL_RGBA,
                         GL_UNSIGNED_BYTE, this->img.data);
        }
    }
}

ui::label::label(ui::composite *c, GLuint w, GLuint h)
    : ui::widget::widget(c, w, h), ui::rect::rect(w, h), str(), img()
{
    float black[4] = {0.0, 0.0, 0.0, 0.0};

    this->use_text = true;
    this->shared_font = false;
    this->font = NULL;
    glGenTextures(1, &this->tex);
    glBindTexture(GL_TEXTURE_2D, this->tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, black);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

ui::label::~label()
{
    glDeleteTextures(1, &this->tex);
    if (this->font != NULL && this->shared_font == false)
        delete this->font;
}

int ui::label::get(GLuint e, GLuint t, void *v)
{
    switch (e)
    {
      case ui::element::font:    return this->get_font(t, v);
      case ui::element::string:  return this->get_string(t, v);
      case ui::element::image:   return this->get_image(t, v);
      default:                   return ui::widget::get(e, t, v);
    }
}

void ui::label::set(GLuint e, GLuint t, void *v)
{
    switch (e)
    {
      case ui::element::font:    this->set_font(t, v);      break;
      case ui::element::string:  this->set_string(t, v);    break;
      case ui::element::image:   this->set_image(t, v);     break;
      default:                   ui::widget::set(e, t, v);  break;
    }
}

void ui::label::draw(GLuint trans_uniform, const glm::mat4& parent_trans)
{
    GLuint text, bgnd, val = (this->use_text ? 1 : 0);

    this->parent->get_va(ui::element::attribute,
                         ui::attribute::use_text, &text,
                         ui::element::attribute,
                         ui::attribute::text_bgnd, &bgnd, 0);
    glUniform1ui(text, val);
    glUniform4f(bgnd,
                this->background.x, this->background.y,
                this->background.z, this->background.a);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, this->tex);
    ui::widget::draw(trans_uniform, parent_trans);
    glDisable(GL_TEXTURE_2D);
    glUniform1ui(text, 0);
}
