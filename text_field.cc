/* text_field.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 31 Oct 2016, 08:03:00 tquirk
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

#include <algorithm>
#include <ratio>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ui_defs.h"
#include "text_field.h"

int ui::text_field::get_size(GLuint t, void *v)
{
    switch (t)
    {
      case ui::size::max_width:  *((GLuint *)v) = this->max_length;  return 0;
      default:                   return this->label::get_size(t, v);
    }
}

void ui::text_field::set_size(GLuint t, void *v)
{
    switch (t)
    {
      case ui::size::max_width:  this->max_length = *(int *)v;  break;
      default:                   this->label::set_size(t, v);   break;
    }
}

int ui::text_field::get_cursor(GLuint t, void *v)
{
    GLuint *val = (GLuint *)v;

    switch (t)
    {
      case ui::cursor::position:  return this->get_cursor_pos(val);
      case ui::cursor::blink:     return this->get_cursor_blink(val);
      default:                    return 1;
    }
}

void ui::text_field::set_cursor(GLuint t, void *v)
{
    GLuint val = *(GLuint *)v;

    switch (t)
    {
      case ui::cursor::position:  this->set_cursor_pos(val);    break;
      case ui::cursor::blink:     this->set_cursor_blink(val);  break;
    }
}

void ui::text_field::set_font(GLuint t, void *v)
{
    this->label::set_font(t, v);

    this->calculate_widget_size();
    this->generate_cursor();
    this->reset_cursor();
}

void ui::text_field::set_string(GLuint t, void *v)
{
    this->label::set_string(t, v);
    this->cursor_pos = this->str.size();
    this->generate_string_image();
    this->reset_cursor();
}

void ui::text_field::set_image(GLuint t, void *v)
{
    /* Don't do anything; this doesn't make sense in this widget. */
}

void ui::text_field::enter_callback(ui::active *a,
                                    void *call,
                                    void *client)
{
    ui::text_field *t = dynamic_cast<ui::text_field *>(a);

    if (t != NULL)
        t->activate_cursor();
}

void ui::text_field::leave_callback(ui::active *a,
                                    void *call,
                                    void *client)
{
    ui::text_field *t = dynamic_cast<ui::text_field *>(a);

    if (t != NULL)
        t->deactivate_cursor();
}

void ui::text_field::key_callback(ui::active *a,
                                  void *call,
                                  void *client)
{
    ui::text_field *t = dynamic_cast<ui::text_field *>(a);
    ui::key_call_data *c = (ui::key_call_data *)call;

    if (t != NULL)
    {
        if (c->key == ui::key::no_key && c->character != 0)
            t->insert_char(c->character);
        else
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
}

int ui::text_field::get_cursor_pos(GLuint *v)
{
    *v = this->cursor_pos;
    return 0;
}

void ui::text_field::set_cursor_pos(GLuint v)
{
    if (v > this->str.size())
        v = this->str.size();
    this->cursor_pos = v;
    this->reset_cursor();
}

/* The cursor blink rate is in milliseconds.  Zero will turn blinking off. */
int ui::text_field::get_cursor_blink(GLuint *v)
{
    *v = this->blink;
    return 0;
}

void ui::text_field::set_cursor_blink(GLuint v)
{
    this->blink = v;
    this->reset_cursor();
}

void ui::text_field::reset_cursor(void)
{
    this->cursor_visible = true;
    this->cursor_clock = std::chrono::high_resolution_clock::now();
}

void ui::text_field::activate_cursor(void)
{
    this->cursor_active = true;
    this->reset_cursor();
}

void ui::text_field::deactivate_cursor(void)
{
    this->cursor_active = false;
}

void ui::text_field::first_char(void)
{
    this->cursor_pos = 0;
    this->generate_string_image();
    this->populate_buffers();
}

void ui::text_field::previous_char(void)
{
    if (this->cursor_pos > 0)
    {
        --this->cursor_pos;
        this->generate_string_image();
        this->populate_buffers();
    }
}

void ui::text_field::next_char(void)
{
    this->cursor_pos = std::min((GLuint)this->str.size(), this->cursor_pos + 1);
    this->generate_string_image();
    this->populate_buffers();
}

void ui::text_field::last_char(void)
{
    this->cursor_pos = this->str.size();
    this->generate_string_image();
    this->populate_buffers();
}

void ui::text_field::insert_char(uint32_t c)
{
    this->str.insert(this->cursor_pos++, 1, c);
    this->generate_string_image();
    this->populate_buffers();
}

void ui::text_field::remove_previous_char(void)
{
    if (this->cursor_pos > 0)
    {
        this->str.erase(--this->cursor_pos, 1);
        this->generate_string_image();
        this->populate_buffers();
    }
}

void ui::text_field::remove_next_char(void)
{
    if (this->cursor_pos < this->str.size())
    {
        this->str.erase(this->cursor_pos, 1);
        this->generate_string_image();
        this->populate_buffers();
    }
}

void ui::text_field::get_string_size(const std::u32string& str,
                                     std::vector<int>& sz)
{
    if (this->font != NULL)
        this->font->get_string_size(str, sz);
}

int ui::text_field::get_raw_cursor_pos(void)
{
    int ret = 0;

    if (this->font != NULL)
    {
        std::vector<int> req_size = {0, 0, 0};

        this->get_string_size(this->str.substr(0, this->cursor_pos), req_size);
        ret = req_size[0];
    }
    return ret;
}

void ui::text_field::set_cursor_transform(int pixel_pos)
{
    glm::vec3 dest;
    glm::mat4 new_trans;

    this->parent->get(ui::element::pixel_size, ui::size::all, &dest);
    dest.x *= this->margin[1] + this->border[1] + 1 + pixel_pos;
    dest.y = -(dest.y * (this->margin[0] + this->border[0] + 1));
    this->cursor_transform = glm::translate(new_trans, dest);
}

int ui::text_field::calculate_field_length(void)
{
    /* We have an extra pixel on each side of the field, per
     * ui::label::calculate_widget_size, thus the literal 2.
     */
    return this->dim.x - this->margin[1] - this->margin[2]
        - this->border[1] - this->border[2] - 2;
}

void ui::text_field::generate_string_image(void)
{
    this->label::generate_string_image();

    std::vector<int> string_max = {0, 0, 0};
    int pixel_pos = this->get_raw_cursor_pos();
    int field_len = this->calculate_field_length();

    this->get_string_size(this->str, string_max);
    if (string_max[0] > field_len)
    {
        /* The full string is too big to be displayed in its entirety.
         * We'll chunk the image into half-widget-size pieces, and try
         * to pick a starting chunk such that the cursor is in the
         * second half of the widget.
         */
        ui::image tmp_img;
        int chunk = field_len / 2;
        int which = std::max((pixel_pos / chunk) - 1, 0);
        int start = chunk * which;

        /* Take the appropriate portion of the string image */
        tmp_img.width = std::min(field_len, string_max[0] - start);
        tmp_img.height = this->img.height;
        tmp_img.per_pixel = this->img.per_pixel;
        tmp_img.data = new unsigned char[tmp_img.width
                                         * tmp_img.height
                                         * tmp_img.per_pixel];
        for (int r = 0; r < tmp_img.height; ++r)
            memcpy(&tmp_img.data[r * tmp_img.width],
                   &this->img.data[r * this->img.width + start],
                   tmp_img.width);
        this->img = tmp_img;

        /* Fix the cursor's position */
        pixel_pos -= start;
    }

    this->set_cursor_transform(pixel_pos);
}

void ui::text_field::calculate_widget_size(void)
{
    std::vector<int> font_max = {0, 0, 0};
    glm::ivec2 size;

    this->font->max_cell_size(font_max);
    font_max[0] *= this->max_length;
    size.x = font_max[0]
        + this->border[1] + this->border[2]
        + this->margin[1] + this->margin[2] + 2;
    size.y = font_max[1] + font_max[2]
        + this->border[0] + this->border[3]
        + this->margin[0] + this->margin[3] + 2;
    this->set_size(ui::size::all, &size);
}

void ui::text_field::generate_cursor(void)
{
    if (this->font != NULL)
    {
        ui::vertex_buffer *vb = new ui::vertex_buffer(32, 6);
        float h, m[2], b[2];
        glm::vec2 psz;

        this->parent->get(ui::element::pixel_size, ui::size::all, &psz);
        psz.y = -psz.y;
        h = ((float)this->dim.y) * psz.y;
        m[0] = this->margin[0] * psz.y;  m[1] = this->margin[3] * psz.y;
        b[0] = this->border[0] * psz.y;  b[1] = this->border[3] * psz.y;

        vb->generate_box(glm::vec2(-1.0f, 1.0f),
                         glm::vec2(-1.0f + psz.x,
                                   1.0f + h - m[0] - b[0] - m[1]
                                   - b[1] - psz.y - psz.y),
                         this->foreground);

        this->cursor_element_count = vb->element_index;
        glBindVertexArray(this->cursor_vao);
        glBindBuffer(GL_ARRAY_BUFFER, this->cursor_vbo);
        glBufferData(GL_ARRAY_BUFFER,
                     vb->vertex_size(), vb->vertex,
                     GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->cursor_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     vb->element_size(), vb->element,
                     GL_DYNAMIC_DRAW);
        delete vb;
    }
}

ui::text_field::text_field(ui::composite *c, GLuint w, GLuint h)
    : ui::label::label(c, w, h), ui::rect::rect(w, h), cursor_transform()
{
    GLuint pos_attr, color_attr, texture_attr;

    this->cursor_pos = 0;
    this->blink = 250;
    this->max_length = 20;
    this->cursor_clock = std::chrono::high_resolution_clock::now();
    this->cursor_visible = true;
    this->cursor_active = false;
    this->cursor_element_count = 0;

    this->parent->get_va(ui::element::attribute,
                         ui::attribute::position,
                         &pos_attr,
                         ui::element::attribute,
                         ui::attribute::color,
                         &color_attr,
                         ui::element::attribute,
                         ui::attribute::texture,
                         &texture_attr, 0);

    glGenVertexArrays(1, &this->cursor_vao);
    glBindVertexArray(this->cursor_vao);
    glGenBuffers(1, &this->cursor_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, this->cursor_vbo);
    glGenBuffers(1, &this->cursor_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->cursor_ebo);
    glEnableVertexAttribArray(pos_attr);
    glVertexAttribPointer(pos_attr, 2, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 8, (void *)0);
    glEnableVertexAttribArray(color_attr);
    glVertexAttribPointer(color_attr, 4, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 8, (void *)(sizeof(float) * 2));
    glEnableVertexAttribArray(texture_attr);
    glVertexAttribPointer(texture_attr, 2, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 8, (void *)(sizeof(float) * 6));

    this->add_callback(ui::callback::enter,
                       ui::text_field::enter_callback,
                       NULL);
    this->add_callback(ui::callback::leave,
                       ui::text_field::leave_callback,
                       NULL);
    this->add_callback(ui::callback::key_down,
                       ui::text_field::key_callback,
                       NULL);

    this->populate_buffers();
}

ui::text_field::~text_field()
{
    glDeleteBuffers(1, &this->cursor_ebo);
    glDeleteBuffers(1, &this->cursor_vbo);
    glDeleteVertexArrays(1, &this->cursor_vao);
}

int ui::text_field::get(GLuint e, GLuint t, void *v)
{
    int ret = 0;

    switch (e)
    {
      case ui::element::cursor:  return this->get_cursor(t, v);
      default:                   return this->label::get(e, t, v);
    }
}

void ui::text_field::set(GLuint e, GLuint t, void *v)
{
    switch (e)
    {
      case ui::element::cursor:  this->set_cursor(t, v);     break;
      default:                   this->label::set(e, t, v);  break;
    }
}

void ui::text_field::draw(GLuint trans_uniform, const glm::mat4& parent_trans)
{
    this->label::draw(trans_uniform, parent_trans);
    if (this->cursor_active)
    {
        if (this->blink > 0)
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
        }
        if (this->cursor_visible)
        {
            glm::mat4 trans
                = this->cursor_transform * this->pos_transform * parent_trans;

            glBindVertexArray(this->cursor_vao);
            glBindBuffer(GL_ARRAY_BUFFER, this->cursor_vbo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->cursor_ebo);
            glUniformMatrix4fv(trans_uniform, 1, GL_FALSE,
                               glm::value_ptr(trans));
            glDrawElements(GL_TRIANGLES, this->cursor_element_count,
                           GL_UNSIGNED_INT, 0);
        }
    }
}
