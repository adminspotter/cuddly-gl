/* text_field.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *
 * CuddlyGL OpenGL widget toolkit
 * Copyright (C) 2016-2026  Trinity Annabelle Quirk
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

#include "text_field.h"
#include "util.h"

void (*ui::text_field::focus_hook)(bool) = NULL;

int ui::text_field::get_size(GLuint t, GLuint *v) const
{
    if (t == ui::size::max_width)
    {
        *v = this->max_length;
        return 0;
    }
    return this->ui::label::get_size(t, v);
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
        this->ui::label::set_size(t, v);
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
    this->ui::label::set_font(t, v);

    this->calculate_widget_size();
    this->generate_cursor();
    this->calculate_positions();
    this->generate_string_image();
    this->reset_cursor();
}

int ui::text_field::get_string(GLuint t, std::string *v) const
{
    if (t == ui::string::selection)
    {
        *v = ui::u32strtoutf8(this->selection);
        return 0;
    }
    return this->ui::label::get_string(t, v);
}

void ui::text_field::set_string(GLuint t, const std::string& v)
{
    if (t == ui::string::selection)
        return;

    this->ui::label::set_string(t, v);
    this->cursor_pos = this->str.size();
    this->selection = std::u32string();
    this->selecting = false;
    this->calculate_positions();
    this->generate_string_image();
    this->reset_cursor();
}

void ui::text_field::set_image(GLuint t, const ui::image& v)
{
    /* Don't do anything; this doesn't make sense in this widget. */
}

void ui::text_field::set_selection(GLuint t, const glm::uvec2& v)
{
    this->select_start = std::min((GLuint)this->str.size(), v.x);
    this->cursor_pos = std::min((GLuint)this->str.size(), v.y);
    this->set_selection_string();
    this->set_cursor_transform(this->positions[this->cursor_pos]
                               - this->img_offset);
    this->populate_buffers();
}

void ui::text_field::focus_callback(ui::active *a, void *call, void *client)
{
    ui::text_field *t = dynamic_cast<ui::text_field *>(a);

    if (t != NULL)
    {
        if (((ui::focus_call_data *)call)->focus == true)
        {
            if (ui::text_field::focus_hook != NULL)
                (*ui::text_field::focus_hook)(true);
            t->activate_cursor();
        }
        else
        {
            t->deactivate_cursor();
            if (ui::text_field::focus_hook != NULL)
                (*ui::text_field::focus_hook)(false);
        }
    }
}

void ui::text_field::btn_down_callback(ui::active *a, void *call, void *client)
{
    ui::text_field *t = dynamic_cast<ui::text_field *>(a);
    ui::btn_call_data *bcd = (ui::btn_call_data *)call;

    if (t != NULL && bcd->button == ui::mouse::button0)
    {
        t->cursor_mouse_position(bcd->location);

        t->selecting = bcd->state == ui::mouse::down;
        t->select_start = t->cursor_pos;
        t->selection = std::u32string();
    }
}

void ui::text_field::btn_up_callback(ui::active *a, void *call, void *client)
{
    ui::text_field *t = dynamic_cast<ui::text_field *>(a);
    ui::btn_call_data *bcd = (ui::btn_call_data *)call;

    if (t != NULL && bcd->button == ui::mouse::button0)
        t->selecting = bcd->state == ui::mouse::down;
}

void ui::text_field::motion_callback(ui::active *a, void *call, void *client)
{
    ui::text_field *t = dynamic_cast<ui::text_field *>(a);
    ui::mouse_call_data *mcd = (ui::mouse_call_data *)call;

    if (t != NULL && t->selecting)
    {
        t->cursor_mouse_position(mcd->location);
        t->set_selection_string();
    }
}

void ui::text_field::key_down_callback(ui::active *a, void *call, void *client)
{
    ui::text_field *t = dynamic_cast<ui::text_field *>(a);
    ui::key_call_data *kcd = (ui::key_call_data *)call;

    if (t != NULL)
    {
        t->apply_key(kcd);
        ui::key_call_data *nkcd = new ui::key_call_data;
        memcpy(nkcd, call, sizeof(ui::key_call_data));
        t->add_timeout(std::chrono::milliseconds(t->repeat_initial),
                       ui::text_field::key_timeout,
                       nkcd);
    }
}

void ui::text_field::key_up_callback(ui::active *a, void *call, void *client)
{
    ui::text_field *t = dynamic_cast<ui::text_field *>(a);

    if (t != NULL)
    {
        std::lock_guard<std::mutex> lock(t->repeat_mutex);
        if (t->timeout_arg != NULL)
        {
            ui::key_call_data *kcd = (ui::key_call_data *)t->timeout_arg;
            t->remove_timeout();
            delete kcd;
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

        ui::key_call_data *kcd = (ui::key_call_data *)client;
        t->apply_key(kcd);
        t->add_timeout(std::chrono::milliseconds(t->repeat_delay),
                       ui::text_field::key_timeout,
                       kcd);
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

void ui::text_field::set_selection_string(void)
{
    this->selection = this->str.substr(std::min(this->cursor_pos,
                                                this->select_start),
                                       std::abs((int)this->select_start
                                                - (int)this->cursor_pos));
}

void ui::text_field::apply_key(const ui::key_call_data *c)
{
    if (c->character != 0)
        this->insert_char(c->character);
    else
    {
        bool select = c->mods & ui::key_mod::shift;

        switch (c->key)
        {
          case ui::key::l_arrow:  this->previous_char(select);   break;
          case ui::key::r_arrow:  this->next_char(select);       break;
          case ui::key::home:     this->first_char(select);      break;
          case ui::key::end:      this->last_char(select);       break;
          case ui::key::bkspc:    this->remove_previous_char();  break;
          case ui::key::del:      this->remove_next_char();      break;
        }
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

void ui::text_field::first_char(bool select)
{
    this->cursor_pos = 0;
    if (!select)
        this->select_start = this->cursor_pos;
    this->generate_string_image();
    this->populate_buffers();
}

void ui::text_field::previous_char(bool select)
{
    if (this->cursor_pos > 0)
    {
        --this->cursor_pos;
        if (!select)
            this->select_start = this->cursor_pos;
        this->generate_string_image();
        this->populate_buffers();
    }
}

void ui::text_field::next_char(bool select)
{
    if (this->cursor_pos < this->str.size())
    {
        ++this->cursor_pos;
        if (!select)
            this->select_start = this->cursor_pos;
        this->generate_string_image();
        this->populate_buffers();
    }
}

void ui::text_field::last_char(bool select)
{
    this->cursor_pos = this->str.size();
    if (!select)
        this->select_start = this->cursor_pos;
    this->generate_string_image();
    this->populate_buffers();
}

void ui::text_field::insert_char(uint32_t c)
{
    this->str.insert(this->cursor_pos++, 1, c);
    this->selecting = false;
    this->select_start = this->cursor_pos;
    this->calculate_positions();
    this->generate_string_image();
    this->populate_buffers();
}

void ui::text_field::remove_previous_char(void)
{
    if (this->cursor_pos > 0)
    {
        this->str.erase(--this->cursor_pos, 1);
        this->selecting = false;
        this->select_start = this->cursor_pos;
        this->calculate_positions();
        this->generate_string_image();
        this->populate_buffers();
    }
}

void ui::text_field::remove_next_char(void)
{
    if (this->cursor_pos < this->str.size())
    {
        this->str.erase(this->cursor_pos, 1);
        this->selecting = false;
        this->select_start = this->cursor_pos;
        this->calculate_positions();
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

    if (this->positions.size() > this->cursor_pos)
        ret = this->positions[this->cursor_pos];
    return ret;
}

void ui::text_field::cursor_mouse_position(glm::ivec2& loc)
{
    int i;

    loc.x += this->img_offset;
    for (i = 0; i <= this->str.size(); ++i)
        if (this->positions[i] > loc.x)
            break;

    if (i > this->str.size())
        this->last_char(this->selecting);
    else
    {
        if (loc.x - this->positions[i - 1] <= this->positions[i] - loc.x)
            this->cursor_pos = i - 1;
        else
            this->cursor_pos = i;
        this->set_cursor_transform(this->positions[this->cursor_pos]
                                   - this->img_offset);
        this->populate_buffers();
    }
}

void ui::text_field::set_cursor_transform(int pixel_pos)
{
    glm::vec3 dest;
    glm::mat4 new_trans(1.0);

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

void ui::text_field::calculate_positions(void)
{
    GLuint w, a, d;

    this->positions.clear();
    for (int i = 0; i <= this->str.size(); ++i)
    {
        this->get_string_size(this->str.substr(0, i), w, a, d);
        this->positions.push_back(w);
    }
}

void ui::text_field::generate_string_image(void)
{
    this->label::generate_string_image();

    int pixel_pos = this->get_raw_cursor_pos();
    int field_len = this->calculate_field_length();

    if (this->img.width > field_len)
    {
        /* The full string is too big to be displayed in its entirety.
         * We'll chunk the image into half-widget-size pieces, and try
         * to pick a starting chunk such that the cursor is in the
         * second half of the widget.
         */
        ui::image tmp_img;
        int chunk = field_len / 2;
        int which = std::max((pixel_pos / chunk) - 1, 0);
        this->img_offset = chunk * which;
        GLuint width = std::min(field_len,
                                (int)this->img.width - this->img_offset);

        this->img = ui::image(this->img, width, this->img_offset);

        /* Fix the cursor's position */
        pixel_pos -= this->img_offset;
    }

    this->set_cursor_transform(pixel_pos);
}

void ui::text_field::calculate_widget_size(void)
{
    int max_width, max_height;
    glm::ivec2 size;

    this->font->max_cell_size(max_width, max_height);
    size.x = (max_width * this->max_length)
        + this->border[1] + this->border[2]
        + this->margin[1] + this->margin[2] + 2;
    size.y = max_height
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
    int max_width, max_asc, max_desc;
    GLuint w, a, d;
    float ph;

    if (this->img.data == NULL)
        return vb;

    this->font->max_cell_size(max_width, max_asc, max_desc);
    this->get_string_size(this->str, w, a, d);

    ph = 1.0f / (float)this->img.height;

    vb->vertex[7] = 1.0f + ((this->margin[0] + this->border[0] + 1
                             + max_asc - a) * ph);
    vb->vertex[15] = vb->vertex[7];
    vb->vertex[23] = 0.0f - ((this->margin[3] + this->border[3] + 1
                              + max_desc - d) * ph);
    vb->vertex[31] = vb->vertex[23];

    return vb;
}

void ui::text_field::init(ui::composite *c)
{
    GLuint pos_attr, color_attr, texture_attr;

    this->cursor_pos = 0;
    this->blink = 250;
    this->max_length = 20;
    this->img_offset = 0;
    this->cursor_clock = std::chrono::high_resolution_clock::now();
    this->cursor_visible = true;
    this->cursor_active = false;
    this->cursor_element_count = 0;
    this->repeat_initial = 350;
    this->repeat_delay = 150;
    this->selecting = false;

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

    this->add_callback(ui::callback::focus,
                       ui::text_field::focus_callback,
                       NULL);
    this->add_callback(ui::callback::key_down,
                       ui::text_field::key_down_callback,
                       NULL);
    this->add_callback(ui::callback::key_up,
                       ui::text_field::key_up_callback,
                       NULL);
    this->add_callback(ui::callback::btn_down,
                       ui::text_field::btn_down_callback,
                       NULL);
    this->add_callback(ui::callback::btn_up,
                       ui::text_field::btn_up_callback,
                       NULL);
    this->add_callback(ui::callback::motion,
                       ui::text_field::motion_callback,
                       NULL);

    this->populate_buffers();
}

ui::text_field::text_field(ui::composite *c)
    : ui::label::label(c), ui::active::active(0, 0), ui::rect::rect(0, 0),
      positions(), cursor_transform(), repeat_mutex(), selection()
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

void ui::text_field::set(GLuint e, GLuint t, const glm::uvec2& v)
{
    if (e == ui::element::string)
        this->set_selection(t, v);
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
