/* text_field.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 28 Jul 2016, 08:09:45 tquirk
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
 * This file contains the text field method definitions for the R9 UI
 * widget set.
 *
 * Things to do
 *
 */

#include <ratio>

#include <glm/gtc/type_ptr.hpp>

#include "text_field.h"

int ui::text_field::get_cursor_pos(GLuint t, void *v)
{
    *((GLuint *)v) = this->cursor_pos;
    return 1;
}

void ui::text_field::set_cursor_pos(GLuint t, void *v)
{
    GLuint new_v = *((GLuint *)v);

    if (new_v > this->str.size())
        new_v = this->str.size();
    this->cursor_pos = new_v;
}

/* The cursor blink rate is in milliseconds.  Zero will turn blinking off. */
int ui::text_field::get_cursor_blink(GLuint t, void *v)
{
    *((GLuint *)v) = this->blink;
    return 1;
}

void ui::text_field::set_cursor_blink(GLuint t, void *v)
{
    this->blink = *((GLuint *)v);
}

int ui::text_field::get_max_size(GLuint t, void *v)
{
    int ret = 0;

    if (t == ui::size::width)
    {
        *((GLuint *)v) = this->max_length;
        ret = 1;
    }
    return ret;
}

void ui::text_field::set_max_size(GLuint t, void *v)
{
    if (t == ui::size::width)
        this->max_length = *(int *)v;
}

void ui::text_field::set_bgimage(GLuint t, void *v)
{
    /* Don't do anything; this doesn't make sense in this widget. */
}

void ui::text_field::key_callback(ui::panel *p, void *call, void *client)
{
    ui::text_field *t = dynamic_cast<ui::text_field *>(p);
    ui::key_call_data *c = (ui::key_call_data *)call;

    if (t == NULL)
        return;
    switch (c->key)
    {
      case ui::key::l_arrow:  t->previous_char();         break;
      case ui::key::r_arrow:  t->next_char();             break;
      case ui::key::home:     t->first_char();            break;
      case ui::key::end:      t->last_char();             break;
      case ui::key::bkspc:    t->remove_previous_char();  break;
      case ui::key::del:      t->remove_next_char();      break;
    }
}

void ui::text_field::first_char(void)
{
    this->cursor_pos = 0;
    this->generate_cursor();
}

void ui::text_field::previous_char(void)
{
    this->cursor_pos = std::max(0u, this->cursor_pos - 1);
    this->generate_cursor();
}

void ui::text_field::next_char(void)
{
    this->cursor_pos = std::min((GLuint)this->str.size(), this->cursor_pos + 1);
    this->generate_cursor();
}

void ui::text_field::last_char(void)
{
    this->cursor_pos = this->str.size();
    this->generate_cursor();
}

void ui::text_field::insert_char(uint32_t c)
{
    this->str.insert(this->cursor_pos++, 1, c);
    this->populate_buffers();
}

void ui::text_field::remove_previous_char(void)
{
    if (this->cursor_pos > 0)
    {
        this->str.erase(--this->cursor_pos, 1);
        this->populate_buffers();
    }
}

void ui::text_field::remove_next_char(void)
{
    if (this->cursor_pos < this->str.size())
    {
        this->str.erase(this->cursor_pos, 1);
        this->generate_string();
    }
}

void ui::text_field::generate_cursor(void)
{
}

void ui::text_field::generate_string(void)
{
    if (this->font != NULL && this->img.data != NULL)
    {
        float vertex[160], pw, ph, m[4], b[4];
        GLuint element[60];
        int mw, mh;

        this->font->max_cell_size(&mw, &mh);
        mw *= this->max_length;

        /* The literal 2s are to provide an extra pixel of space
         * between the edges of the border/margin/widget and the
         * actual string.
         */
        this->width = mw
            + this->margin[1] + this->margin[2]
            + this->border[1] + this->border[2] + 2;
        this->height = mh
            + this->margin[0] + this->margin[3]
            + this->border[0] + this->border[3] + 2;
        this->panel::generate_points(vertex, element);

        pw = 1.0f / (float)this->img.width;
        ph = 1.0f / (float)this->img.height;
        m[0] = this->margin[0] * ph;  b[0] = this->border[0] * ph;
        m[1] = this->margin[1] * pw;  b[1] = this->border[1] * pw;
        m[2] = this->margin[2] * pw;  b[2] = this->border[2] * pw;
        m[3] = this->margin[3] * ph;  b[3] = this->border[3] * ph;

        /* TODO:  the displayed string may be too long to fit in the
         * panel that we have available.  We'll need to check for that
         * and take a sub-image for display.
         */

        vertex[6]  = 0.0f - m[1] - b[1] - pw;
        vertex[7]  = 1.0f + m[0] + b[0] + ph;

        vertex[14] = 1.0f
            + ((mw - this->img.width) * pw)
            + m[2] + b[2] + pw;
        vertex[15] = vertex[7];

        vertex[22] = vertex[6];
        vertex[23] = 0.0f - m[3] - b[3] - ph;

        vertex[30] = vertex[14];
        vertex[31] = vertex[23];

        memcpy(&vertex[2],
               glm::value_ptr(this->foreground), sizeof(float) * 4);
        memcpy(&vertex[10],
               glm::value_ptr(this->foreground), sizeof(float) * 4);
        memcpy(&vertex[18],
               glm::value_ptr(this->foreground), sizeof(float) * 4);
        memcpy(&vertex[26],
               glm::value_ptr(this->foreground), sizeof(float) * 4);
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
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
                     this->img.width, this->img.height, 0, GL_RED,
                     GL_UNSIGNED_BYTE, this->img.data);
    }
}

void ui::text_field::populate_buffers(void)
{
    this->generate_string();
    this->generate_cursor();
}

ui::text_field::text_field(ui::context *c, GLuint w, GLuint h)
    : ui::label::label(c, w, h)
{
    GLuint pos_attr, color_attr;

    this->cursor_pos = 0;
    this->blink = 250;
    this->max_length = 20;
    this->cursor_clock = std::chrono::high_resolution_clock::now();
    this->cursor_visible = true;
    this->add_callback(ui::callback::key_down,
                       ui::text_field::key_callback,
                       NULL);

    this->parent->get(ui::element::attribute,
                      ui::attribute::position,
                      &pos_attr);
    this->parent->get(ui::element::attribute,
                      ui::attribute::color,
                      &color_attr);

    glGenVertexArrays(1, &this->cursor_vao);
    glBindVertexArray(this->cursor_vao);
    glGenBuffers(1, &this->cursor_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, this->cursor_vbo);
    glEnableVertexAttribArray(pos_attr);
    glVertexAttribPointer(pos_attr, 2, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 6, (void *)0);
    glEnableVertexAttribArray(color_attr);
    glVertexAttribPointer(color_attr, 4, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 6, (void *)(sizeof(float) * 2));

    this->populate_buffers();
}

ui::text_field::~text_field()
{
    glDeleteBuffers(1, &this->cursor_vbo);
    glDeleteVertexArrays(1, &this->cursor_vao);
}

int ui::text_field::get(GLuint e, GLuint t, void *v)
{
    int ret = 0;

    switch (e)
    {
      case ui::element::cursor:
        switch (t)
        {
          case ui::cursor::position:  return this->get_cursor_pos(t, v);
          case ui::cursor::blink:     return this->get_cursor_blink(t, v);
        }
      case ui::element::max_size:     return this->get_max_size(t, v);
      default:                        return ui::label::get(e, t, v);
    }
}

void ui::text_field::set(GLuint e, GLuint t, void *v)
{
    switch (e)
    {
      case ui::element::cursor:
        switch (t)
        {
          case ui::cursor::position:  this->set_cursor_pos(t, v);   break;
          case ui::cursor::blink:     this->set_cursor_blink(t, v); break;
        }
        this->generate_cursor();
        break;

      case ui::element::max_size:
        this->set_max_size(t, v);
        this->populate_buffers();
        break;

      default:
        ui::label::set(e, t, v);
        break;
    }
}

void ui::text_field::draw(void)
{
    std::chrono::high_resolution_clock::time_point now
        = std::chrono::high_resolution_clock::now();
    std::chrono::duration<GLuint, std::milli> ms
        = std::chrono::duration_cast<std::chrono::milliseconds>
        (now - this->cursor_clock);

    if (ms.count() >= this->blink)
    {
        this->cursor_visible = !this->cursor_visible;
        this->cursor_clock = now;
    }
    ui::label::draw();
}
