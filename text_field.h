/* text_field.h                                            -*- C++ -*-
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 26 Jul 2016, 07:49:03 tquirk
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
 * This file contains the text field declaration.  We're subclassing
 * the label, since it already has font handling built in.  We'll be
 * adding keyboard handling and a moving cursor.
 *
 * Things to do
 *   - Determine if we want to add a 'password' flag here, to display
 *     things as asterisks, or have that in a subclass.
 *
 */

#ifndef __INC_R9_TEXT_FIELD_H__
#define __INC_R9_TEXT_FIELD_H__

#include <chrono>

#include "label.h"

namespace ui
{
    class text_field : public label
    {
      protected:
        GLuint cursor_pos, blink, max_length;
        std::chrono::high_resolution_clock::time_point cursor_clock;
        bool cursor_visible;

        int get_cursor_pos(GLuint, void *);
        void set_cursor_pos(GLuint, void *);
        int get_cursor_blink(GLuint, void *);
        void set_cursor_blink(GLuint, void *);
        int get_max_size(GLuint, void *);
        void set_max_size(GLuint, void *);
        void set_bgimage(GLuint, void *);

        void first_char(void);
        void previous_char(void);
        void next_char(void);
        void last_char(void);
        void insert_char(uint32_t);
        void remove_previous_char(void);
        void remove_next_char(void);

      public:
        text_field(context *, GLuint = 0, GLuint = 0);
        virtual ~text_field();

        virtual int get(GLuint, GLuint, void *);
        virtual void set(GLuint, GLuint, void *);

        virtual void draw(void);
    };
}

#endif /* __INC_R9_TEXT_FIELD_H__ */
