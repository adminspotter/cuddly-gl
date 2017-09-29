/* password.h                                              -*- C++ -*-
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 29 Oct 2016, 15:54:00 tquirk
 *
 * CuddlyGL OpenGL widget toolkit
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
 * This file contains the password field declaration.  We're
 * subclassing the text_field, since it already does most of what we
 * want here.  We'll display a set of asterisks, rather than the
 * actual string.
 *
 * Things to do
 *
 */

#ifndef __INC_R9_PASSWORD_H__
#define __INC_R9_PASSWORD_H__

#include "text_field.h"

namespace ui
{
    class password : public text_field
    {
      protected:
        virtual void generate_string_image(void) override;

        virtual void get_string_size(const std::u32string&,
                                     std::vector<int>&) override;
        virtual int get_raw_cursor_pos(void) override;

      public:
        password(composite *, GLuint, GLuint);
        virtual ~password();
    };
}

#endif /* __INC_R9_PASSWORD_H__ */
