/* multi_label.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 22 Sep 2016, 23:47:25 tquirk
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
 * This file contains the multi-line label object definitions.  We
 * derive from the label, which will give us a basic string rendering
 * framework.  In this class, we'll add calculation of line breaks and
 * a desired dimension, not unlike the row-column manager widget.
 *
 * Things to do
 *
 */

#include "multi_label.h"

/* The "string" which contains all the whitespace characters that
 * we're interested in.
 */
std::u32string ui::multi_label::whitespace =
{
    0x0020, 0x1680, 0x180e, 0x2000, 0x2001, 0x2002, 0x2003, 0x2004, 0x2005,
    0x2006, 0x2007, 0x2008, 0x2009, 0x200a, 0x200b, 0x205f, 0x3000
};

/* The "string" which contains all line-break characters.  We also
 * include the vertical tab and form-feed.
 */
std::u32string ui::multi_label::newline =
{
    0x000a, 0x000b, 0x000c, 0x000d
};

void ui::multi_label::set_image(GLuint t, void *v)
{
    /* Don't do anything; this doesn't make sense in this widget. */
}

/* We need to be able to break on any arbitrary space when doing
 * multiline strings, so let's get a list of all the valid whitespace
 * that we can break on, in one place.  Found a nice table at
 * https://www.cs.tut.fi/~jkorpela/chars/spaces.html
 */
bool ui::multi_label::is_whitespace(uint32_t c)
{
    if ((c >= 0x000a && c <= 0x000d)
        || c == 0x0020 || c == 0x1680 || c == 0x180e
        || (c >= 0x2000 && c <= 0x200b)
        || c == 0x205f || c == 0x3000)
        return true;
    return false;
}

/* This function will consider 0x000d and 0x000a separately, so if the
 * system newline is the combination of the two, we'll get
 * double-spacing.
 */
void ui::multi_label::split_by_newlines(std::u32string s,
                                        std::list<std::u32string>& strs)
{
    std::u32string tmp_str = s;
    std::u32string::size_type pos;

    while ((pos = tmp_str.find_first_of(ui::multi_label::newline))
           != std::u32string::npos)
    {
        if (pos > 0)
            strs.push_back(tmp_str.substr(0, pos - 1));
        else
            strs.push_back(std::u32string());
        tmp_str = tmp_str.substr(pos + 1);
    }
    strs.push_back(tmp_str);
}

/* There is no whitespace in our string which will break close enough
 * to the beginning to give us the width we want.  So we'll break at a
 * point which gives us the size we need.  This is the function which
 * breaks up the proverbial "page-widening posts".
 *
 * We'll use a binary search through the string to reduce the number
 * of string-size calculations we need to do.
 */
void ui::multi_label::hard_split_string(GLuint width,
                                        std::vector<int>& sz,
                                        std::list<std::u32string>& strs,
                                        std::list<std::u32string>::iterator& i)
{
    std::u32string str = *i;
    std::u32string::size_type first = 0, last = str.size() - 1, pos = last / 2;
    auto new_iter = i;

    while (last - first > 1)
    {
        this->font->get_string_size(str.substr(0, pos), sz);
        if (sz[0] <= width)
            last = pos;
        else
            first = pos;
        pos = first + ((last - first) / 2);
    }
    *i = str.substr(0, pos);
    strs.insert(++new_iter, str.substr(pos + 1));
}

ui::multi_label::multi_label(ui::composite *p, GLuint w, GLuint h)
    : ui::label::label(p, w, h)
{
}

ui::multi_label::~multi_label()
{
}
