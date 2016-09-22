/* multi_label.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 22 Sep 2016, 08:46:45 tquirk
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

ui::multi_label::multi_label(ui::composite *p, GLuint w, GLuint h)
    : ui::label::label(p, w, h)
{
}

ui::multi_label::~multi_label()
{
}
