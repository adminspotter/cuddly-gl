/* multi_label.h                                           -*- C++ -*-
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 23 Sep 2016, 08:10:20 tquirk
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
 * This file contains the multi-line label object declaration.  We
 * derive from the regular label, and add handling for breaking the
 * string into multiple lines by whitespace, or by characters if there
 * are no spaces (no "page-widening posts").
 *
 * Things to do
 *
 */

#ifndef __INC_R9_MULTI_LABEL_H__
#define __INC_R9_MULTI_LABEL_H__

#include "label.h"

namespace ui
{
    class multi_label : public label
    {
      protected:
        static std::u32string whitespace, newline;

        virtual void set_image(GLuint, void *);

        void split_by_newlines(std::u32string, std::list<std::u32string>&);
        std::u32string::size_type hard_split_string(GLuint, std::u32string&);
        void split_string_to_width(GLuint, std::list<std::u32string>&);

      public:
        multi_label(composite *, GLuint, GLuint);
        virtual ~multi_label();
    };
}

#endif /* __INC_R9_MULTI_LABEL_H__ */
