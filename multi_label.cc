/* multi_label.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 26 Sep 2016, 09:32:30 tquirk
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
 * we're interested in.  Found a nice table at
 * https://www.cs.tut.fi/~jkorpela/chars/spaces.html
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
            strs.push_back(tmp_str.substr(0, pos));
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
std::u32string::size_type ui::multi_label::hard_split_string(GLuint width,
                                                             std::u32string& s)
{
    std::vector<int> sz = {0, 0, 0};
    std::u32string::size_type first = 0, last = s.size() - 1, pos = last / 2;

    while (last - first > 1)
    {
        this->font->get_string_size(s.substr(0, pos), sz);
        if (sz[0] <= width)
            first = pos;
        else
            last = pos;
        pos = first + ((last - first) / 2);
    }
    return pos;
}

void ui::multi_label::split_string_to_width(GLuint width,
                                            std::list<std::u32string>& strs)
{
    std::vector<int> sz = {0, 0, 0};
    std::u32string::size_type pos;

    /* Split on newlines, push everything into the list */
    this->split_by_newlines(this->str, strs);

    /* For each list element:
     * 1) Get string size
     * 2) While size is too long, find character before last chunk of whitespace
     * 3) If we have a chunk that fits, insert the remainder next in the list,
     *    and shorten ourselves to be the leading substring
     */
    for (auto i = strs.begin(); i != strs.end(); ++i)
    {
        std::u32string tmp_str = *i;
        auto next = i;
        ++next;

        this->font->get_string_size(tmp_str, sz);
        if (sz[0] <= width)
            continue;
        while (sz[0] > width)
        {
            pos = tmp_str.find_last_of(ui::multi_label::whitespace);
            if (pos == std::u32string::npos)
                /* There isn't any whitespace that we can break on,
                 * that lets our string be short enough.  We'll chop
                 * things apart at the closest character.
                 */
                pos = this->hard_split_string(width, tmp_str);

            tmp_str = tmp_str.substr(0, pos);
            this->font->get_string_size(tmp_str, sz);
        }

        /* Skip past any whitespace in the second half of our string */
        std::u32string second = (*i).substr(tmp_str.size());
        strs.insert(next,
                    second.substr(
                        second.find_first_not_of(ui::multi_label::whitespace)));
        *i = tmp_str;
    }
}

void ui::multi_label::generate_string_image(void)
{
    if (this->font != NULL)
    {
        std::list<std::u32string> strs;
        std::vector<std::u32string> str_vec;
        GLuint width = this->size.x - this->margin[1] - this->border[1]
            - this->border[2] - this->margin[2] - 2;

        this->split_string_to_width(width, strs);
        str_vec.insert(str_vec.begin(), strs.begin(), strs.end());
        this->font->render_multiline_string(str_vec, this->img);
    }
}

void ui::multi_label::calculate_widget_size(int w, int h)
{
    /* We'll leave the x size as-is, since it's what we used to
     * generate the string image.
     */
    this->size.y = h
        + this->margin[0] + this->border[0]
        + this->border[3] + this->margin[3] + 2;
    this->parent->move_child(this);
}

ui::multi_label::multi_label(ui::composite *p, GLuint w, GLuint h)
    : ui::label::label(p, w, h)
{
}

ui::multi_label::~multi_label()
{
}
