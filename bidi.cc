/* bidi.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 28 Apr 2018, 11:20:14 tquirk
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

const int PARA_SEP = 0x2029;

std::u32string CRLF = { 0x0d, 0x0a };

char_class_t bidi::char_type(char32_t c)
{
    if (c == LRE) return class_LRE;
    if (c == RLE) return class_RLE;
    if (c == PDF) return class_PDF;
    if (c == LRO) return class_LRO;
    if (c == RLO) return class_RLO;
    if (c == LRI) return class_LRI;
    if (c == RLI) return class_RLI;
    if (c == FSI) return class_FSI;
    if (c == PDI) return class_PDI;
    if (c == ALM || AL.find(c) != AL.end()) return class_AL;
    if (c == RLM || R.find(c) != R.end()) return class_R;
    if (AN.find(c) != AN.end()) return class_AN;
    if (B.find(c) != B.end()) return class_B;
    if (BN.find(c) != BN.end()) return class_BN;
    if (CS.find(c) != CS.end()) return class_CS;
    if (EN.find(c) != EN.end()) return class_EN;
    if (ES.find(c) != ES.end()) return class_ES;
    if (ET.find(c) != ET.end()) return class_ET;
    if (NSM.find(c) != NSM.end()) return class_NSM;
    if (ON.find(c) != ON.end()) return class_ON;
    if (S.find(c) != S.end()) return class_S;
    if (WS.find(c) != WS.end()) return class_WS;
    return class_L;
}

char_class_t bidi::reset_direction_class(char_class_t cc)
{
    if (this->direction_stack.top().override == direction_rec::LTR)
        return class_L;
    else if (this->direction_stack.top().override == direction_rec::RTL)
        return class_R;
    return cc;
}

/* Rule P1:  split text into paragraphs.  Along with the contents of
 * character class B, we will also transform CRLF (0x0d 0x0a) to be a
 * valid separator.
 */
std::vector<std::u32string> bidi::rule_p1(const std::u32string& s)
{
    std::u32string tmp_str(s);
    std::u32string::size_type pos, start;
    std::u32string empty;
    std::vector<std::u32string> ret;

    while ((pos = tmp_str.find(CRLF)) != std::u32string::npos)
        tmp_str.replace(pos, 2, 1, PARA_SEP);

    start = pos = 0;
    for (auto c : tmp_str)
    {
        if (B.find(c) != B.end())
        {
            ret.push_back(tmp_str.substr(start, pos - start + 1));
            start = pos + 1;
        }
        ++pos;
    }
    ret.push_back(tmp_str.substr(start));
    return ret;
}

/* Rule P2:  find the first L, AL, or R character that isn't part of
 * an isolate block.
 * Rule P3:  if the found character is AL or R, set paragraph
 * embedding to 1, otherwise 0.
 */
int bidi::rule_p2_p3(const std::u32string& s)
{
    int embedding = 0, isolate_level = 0;

    for (auto c : s)
        if (c >= LRI && c <= FSI)
            ++isolate_level;
        else if (c == PDI)
        {
            if (--isolate_level < 0)
                break;
        }
        else if (isolate_level == 0)
        {
            char_class_t type = bidi::char_type(c);
            if (type == class_R || type == class_AL)
            {
                embedding = 1;
                break;
            }
            else if (type == class_L)
                break;
        }

    return embedding;
}

bidi::bidi()
    : direction_stack()
{
    this->overflow_isolate = this->overflow_embed = this->valid_isolate = 0;
}

bidi::~bidi()
{
}
