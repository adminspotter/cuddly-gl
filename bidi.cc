/* bidi.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 22 Apr 2018, 09:53:20 tquirk
 *
 * CuddlyGL OpenGL widget toolkit
 * Copyright (C) 2018  Trinity Annabelle Quirk
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
 * Implementation of the Unicode BIDI algorithm.
 *
 * Things to do
 *
 */

#include "bidi.h"

/* Rule P1:  split text into paragraphs.  We will consider LF (0x0a)
 * and CRLF (0x0d 0x0a) to be the separators.
 */
std::vector<std::u32string> bidi_p1(const std::u32string& s)
{
    std::u32string tmp_str(s);
    std::u32string::size_type pos;
    std::u32string empty;
    std::vector<std::u32string> ret;

    while ((pos = tmp_str.find_first_of(0x0a)) != std::u32string::npos)
    {
        ret.push_back(tmp_str.substr(0, pos));
        tmp_str.replace(0, pos + 1, empty);
    }
    ret.push_back(tmp_str);
    return ret;
}
