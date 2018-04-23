/* bidi.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 23 Apr 2018, 16:50:01 tquirk
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

std::u32string PARA_SEP = { 0x0a, 0x2029 };
std::u32string CRLF = { 0x0d, 0x0a };

/* Rule P1:  split text into paragraphs.  We will consider LF (0x0a),
 * Paragraph Separator (0x2029), and CRLF (0x0d 0x0a) to be the
 * separators.
 */
std::vector<std::u32string> bidi_p1(const std::u32string& s)
{
    std::u32string tmp_str(s);
    std::u32string::size_type pos;
    std::u32string empty;
    std::vector<std::u32string> ret;

    while ((pos = tmp_str.find(CRLF)) != std::u32string::npos)
        tmp_str.replace(pos, 2, 1, 0x0a);

    while ((pos = tmp_str.find_first_of(PARA_SEP)) != std::u32string::npos)
    {
        ret.push_back(tmp_str.substr(0, pos));
        tmp_str.replace(0, pos + 1, empty);
    }
    ret.push_back(tmp_str);
    return ret;
}

/* Rule P2:  find the first L, AL, or R character that isn't part of
 * an isolate block.
 * Rule P3:  if the found character is AL or R, set paragraph
 * embedding to 1, otherwise 0.
 */
int bidi_p2_p3(const std::u32string& s)
{
    int embedding = 0;

    return embedding;
}
