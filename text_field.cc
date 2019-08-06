/* text_field.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 05 Aug 2019, 08:39:14 tquirk
 *
 * CuddlyGL OpenGL widget toolkit
 * Copyright (C) 2019  Trinity Annabelle Quirk
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
 * This file contains the text field method definitions for the
 * CuddlyGL UI widget set.
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

int ui::text_field::get_size(GLuint t, GLuint *v) const
{
    if (t == ui::size::max_width)
    {
        *v = this->max_length;
        return 0;
    }
    return this->label::get_size(t, v);
}

void ui::text_field::set_size(GLuint t, GLuint v)
{
    if (t == ui::size::max_width)
    {
        this->max_length = v;
        this->calculate_widget_size();
        this->populate_buffers();
        this->reset_cursor();
    }
    else
        this->label::set_size(t, v);
}

int ui::text_field::get_cursor(GLuint t, GLuint *v) const
{
    switch (t)
    {
      case ui::cursor::position:  return this->get_cursor_pos(v);
      case ui::cursor::blink:     return this->get_cursor_blink(v);
      default:                    return 1;
    }
}

void ui::text_field::set_cursor(GLuint t, GLuint v)
{
    switch (t)
    {
      case ui::cursor::position:  this->set_cursor_pos(v);    break;
      case ui::cursor::blink:     this->set_cursor_blink(v);  break;
      default:                                                return;
    }
    this->generate_string_image();
    this->reset_cursor();
}

int ui::text_field::get_repeat(GLuint t, GLuint *v) const
{
    switch (t)
    {
      case ui::repeat::initial:    return this->get_initial_repeat(v);
      case ui::repeat::secondary:  return this->get_secondary_repeat(v);
      default:                     return 1;
    }
}

void ui::text_field::set_repeat(GLuint t, GLuint v)
{
    switch (t)
    {
      case ui::repeat::initial:    this->set_initial_repeat(v);    break;
      case ui::repeat::secondary:  this->set_secondary_repeat(v);  break;
      default:                                                     return;
    }
}

void ui::text_field::set_font(GLuint t, ui::base_font *v)
{
    this->label::set_font(t, v);

    this->calculate_widget_size();
    this->generate_cursor();
    this->generate_string_image();
    this->reset_cursor();
}

void ui::text_field::set_string(GLuint t, const std::string& v)
{
    this->label::set_string(t, v);
    this->cursor_pos = this->str.size();
    this->generate_string_image();
    this->reset_cursor();
}

void ui::text_field::set_image(GLuint t, const ui::image& v)
{
    /* Don't do anything; this doesn't make sense in this widget. */
}

void ui::text_field::enter_callback(ui::active *a, void *call, void *client)
{
    ui::text_field *t = dynamic_cast<ui::text_field *>(a);

    if (t != NULL)
        t->activate_cursor();
}

void ui::text_field::leave_callback(ui::active *a, void *call, void *client)
{
    ui::text_field *t = dynamic_cast<ui::text_field *>(a);

    if (t != NULL)
        t->deactivate_cursor();
}

void ui::text_field::key_down_callback(ui::active *a, void *call, void *client)
{
    ui::text_field *t = dynamic_cast<ui::text_field *>(a);
    ui::key_call_data *c = (ui::key_call_data *)call;

    if (t != NULL)
    {
        t->apply_key(c);
        ui::key_call_data *k = new ui::key_call_data;
        memcpy(k, call, sizeof(ui::key_call_data));
        t->add_timeout(std::chrono::milliseconds(t->repeat_initial),
                       ui::text_field::key_timeout,
                       k);
    }
}

void ui::text_field::key_up_callback(ui::active *a, void *call, void *client)
{
    ui::text_field *t = dynamic_cast<ui::text_field *>(a);
    ui::key_call_data *c = (ui::key_call_data *)call;

    if (t != NULL)
    {
        std::lock_guard<std::mutex> lock(t->repeat_mutex);
        if (t->timeout_arg != NULL)
        {
            ui::key_call_data *k = (ui::key_call_data *)t->timeout_arg;
            t->remove_timeout();
            delete k;
        }
    }
}

void ui::text_field::key_timeout(ui::active *a, void *client)
{
    ui::text_field *t = dynamic_cast<ui::text_field *>(a);

    if (t != NULL && client != NULL)
    {
        if (!t->repeat_mutex.try_lock())
            return;

        ui::key_call_data *c = (ui::key_call_data *)client;
        t->apply_key(c);
        t->add_timeout(std::chrono::milliseconds(t->repeat_delay),
                       ui::text_field::key_timeout,
                       c);
        t->repeat_mutex.unlock();
    }
}

int ui::text_field::get_cursor_pos(GLuint *v) const
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
int ui::text_field::get_cursor_blink(GLuint *v) const
{
    *v = this->blink;
    return 0;
}

void ui::text_field::set_cursor_blink(GLuint v)
{
    this->blink = v;
    this->reset_cursor();
}

/* Repeat rates are also in milliseconds. */
int ui::text_field::get_initial_repeat(GLuint *v) const
{
    *v = this->repeat_initial;
    return 0;
}

void ui::text_field::set_initial_repeat(GLuint v)
{
    this->repeat_initial = v;
}

int ui::text_field::get_secondary_repeat(GLuint *v) const
{
    *v = this->repeat_delay;
    return 0;
}

void ui::text_field::set_secondary_repeat(GLuint v)
{
    this->repeat_delay = v;
}

void ui::text_field::apply_key(const ui::key_call_data *c)
{
    if (c->key == ui::key::no_key && c->character != 0)
        this->insert_char(c->character);
    else
        switch (c->key)
        {
          case ui::key::l_arrow:  this->previous_char();         break;
          case ui::key::r_arrow:  this->next_char();             break;
          case ui::key::home:     this->first_char();            break;
          case ui::key::end:      this->last_char();             break;
          case ui::key::bkspc:    this->remove_previous_char();  break;
          case ui::key::del:      this->remove_next_char();      break;
        }
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
    if (this->cursor_pos < this->str.size())
    {
        ++this->cursor_pos;
        this->generate_string_image();
        this->populate_buffers();
    }
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
                                     GLuint& w, GLuint& a, GLuint& d)
{
    if (this->font != NULL)
        this->font->get_string_size(str, w, a, d);
}

int ui::text_field::get_raw_cursor_pos(void)
{
    int ret = 0;

    if (this->font != NULL)
    {
        GLuint w, a, d;

        this->get_string_size(this->str.substr(0, this->cursor_pos), w, a, d);
        ret = w;
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

    GLuint w, a, d;
    int pixel_pos = this->get_raw_cursor_pos();
    int field_len = this->calculate_field_length();

    this->get_string_size(this->str, w, a, d);
    if (w > field_len)
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
        tmp_img.width = std::min(field_len, (int)w - start);
        tmp_img.height = this->img.height;
        tmp_img.per_pixel = this->img.per_pixel;
        tmp_img.data = new unsigned char[tmp_img.width
                                         * tmp_img.height
                                         * tmp_img.per_pixel];
        for (int r = 0; r < tmp_img.height; ++r)
            memcpy(&tmp_img.data[r * tmp_img.width * tmp_img.per_pixel],
                   &this->img.data[r * this->img.width * tmp_img.per_pixel
                                   + start],
                   tmp_img.width * tmp_img.per_pixel);
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
    this->set_size(ui::size::all, size);
}

void ui::text_field::generate_cursor(void)
{
    if (this->font != NULL)
    {
        ui::vertex_buffer *vb = new ui::vertex_buffer();
        float h, m[2], b[2];
        glm::vec3 psz;

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

        this->cursor_element_count = vb->element_count();
        glBindVertexArray(this->cursor_vao);
        glBindBuffer(GL_ARRAY_BUFFER, this->cursor_vbo);
        glBufferData(GL_ARRAY_BUFFER,
                     vb->vertex_size(), vb->vertex_data(),
                     GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->cursor_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     vb->element_size(), vb->element_data(),
                     GL_DYNAMIC_DRAW);
        delete vb;
    }
}

/* The ui::label's generate_points() assumes that we want to wrap our
 * images as tightly as possible.  In this widget we use the cell
 * size, which is constant for any font, as the sizing for our widget,
 * and we want the baselines for our strings to always be in the same
 * place in the widget.  We only need to adjust the image s/t values
 * along the y-axis.
 */
ui::vertex_buffer *ui::text_field::generate_points(void)
{
    ui::vertex_buffer *vb = this->label::generate_points();
    std::vector<int> font_max = {0, 0, 0};
    GLuint w, a, d;
    float ph;

    if (this->img.data == NULL)
        return vb;

    this->font->max_cell_size(font_max);
    this->get_string_size(this->str, w, a, d);

    ph = 1.0f / (float)this->img.height;

    vb->vertex[7] = 1.0f + ((this->margin[0] + this->border[0] + 1
                             + font_max[1] - a) * ph);
    vb->vertex[15] = vb->vertex[7];
    vb->vertex[23] = 0.0f - ((this->margin[3] + this->border[3] + 1
                              + font_max[2] - d) * ph);
    vb->vertex[31] = vb->vertex[23];

    return vb;
}

void ui::text_field::init(ui::composite *c)
{
    GLuint pos_attr, color_attr, texture_attr;

    this->cursor_pos = 0;
    this->blink = 250;
    this->max_length = 20;
    this->cursor_clock = std::chrono::high_resolution_clock::now();
    this->cursor_visible = true;
    this->cursor_active = false;
    this->cursor_element_count = 0;
    this->repeat_initial = 350;
    this->repeat_delay = 150;

    this->parent->get(ui::element::attribute,
                      ui::attribute::position,
                      &pos_attr,
                      ui::element::attribute,
                      ui::attribute::color,
                      &color_attr,
                      ui::element::attribute,
                      ui::attribute::texture,
                      &texture_attr);

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
                       ui::text_field::key_down_callback,
                       NULL);
    this->add_callback(ui::callback::key_up,
                       ui::text_field::key_up_callback,
                       NULL);

    this->populate_buffers();
}

ui::text_field::text_field(ui::composite *c)
    : ui::label::label(c), ui::active::active(0, 0), ui::rect::rect(0, 0),
      cursor_transform(), repeat_mutex()
{
    this->init(c);
}

ui::text_field::~text_field()
{
    glDeleteBuffers(1, &this->cursor_ebo);
    glDeleteBuffers(1, &this->cursor_vbo);
    glDeleteVertexArrays(1, &this->cursor_vao);
}

int ui::text_field::get(GLuint e, GLuint t, GLuint *v) const
{
    switch (e)
    {
      case ui::element::cursor:  return this->get_cursor(t, v);
      case ui::element::repeat:  return this->get_repeat(t, v);
      default:                   return this->label::get(e, t, v);
    }
}

void ui::text_field::set(GLuint e, GLuint t, GLuint v)
{
    switch (e)
    {
      case ui::element::cursor:  this->set_cursor(t, v);     break;
      case ui::element::repeat:  this->set_repeat(t, v);     break;
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
