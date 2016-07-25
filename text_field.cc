/* text_field.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 25 Jul 2016, 07:55:58 tquirk
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
    if (this->font != NULL)
    {
        int mw, mh;

        this->font->max_cell_size(&mw, &mh);

        if (t == ui::size::width)
        {
            this->max_length = *(int *)v;
            this->width = mw * this->max_length;
            this->height = mh;
        }
    }
}

void ui::text_field::set_bgimage(GLuint t, void *v)
{
    /* Don't do anything; this doesn't make sense in this widget. */
}

void ui::text_field::first_char(void)
{
    this->cursor_pos = 0;
}

void ui::text_field::previous_char(void)
{
    this->cursor_pos = std::max(0u, this->cursor_pos - 1);
}

void ui::text_field::next_char(void)
{
    this->cursor_pos = std::min((GLuint)this->str.size(), this->cursor_pos + 1);
}

void ui::text_field::last_char(void)
{
    this->cursor_pos = this->str.size();
}

void ui::text_field::insert_char(uint32_t c)
{
    this->str.insert(this->cursor_pos++, 1, c);
}

void ui::text_field::remove_char(void)
{
    this->str.erase(this->cursor_pos--, 1);
}

ui::text_field::text_field(ui::context *c, GLuint w, GLuint h)
    : ui::label::label(c, w, h)
{
    this->cursor_pos = 0;
    this->blink = 250;
    this->max_length = 20;
}

ui::text_field::~text_field()
{
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
        break;

      case ui::element::max_size:
        this->set_max_size(t, v);
        break;

      default:
        ui::label::set(e, t, v);
        break;
    }
}

void ui::text_field::draw(void)
{
    ui::label::draw();
}
