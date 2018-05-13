/* bidi.cc
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 13 May 2018, 09:33:15 tquirk
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
 * Implementation of the Unicode BIDI algorithm, revision 37.
 *
 * http://www.unicode.org/reports/tr9/tr9-37.html
 *
 * Things to do
 *
 */

#include "bidi.h"

const int PARA_SEP = 0x2029;

std::u32string CRLF = { 0x0d, 0x0a };

const int bidi::MAX_DEPTH = 125;
const int bidi::MAX_STACK_SIZE = bidi::MAX_DEPTH + 2;
const int bidi::MAX_PAIR_STACK_SIZE = 63;

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

bidi::char_container bidi::rule_x1(int base, const std::u32string& str)
{
    while (this->direction_stack.size())
        this->direction_stack.pop();
    this->direction_stack.push({base, direction_rec::NEUTRAL, false});
    this->overflow_isolate = this->overflow_embed = this->valid_isolate = 0;

    bidi::char_container s;
    std::u32string::size_type pos = 0;

    for (auto c : str)
    {
        bidi::character_rec cr =
            {c, bidi::char_type(c), this->direction_stack.top().embed};

        switch (cr.c_class)
        {
          case class_RLE:  s.push_back(this->rule_x2(cr));   break;
          case class_LRE:  s.push_back(this->rule_x3(cr));   break;
          case class_RLO:  s.push_back(this->rule_x4(cr));   break;
          case class_LRO:  s.push_back(this->rule_x5(cr));   break;
          case class_RLI:  s.push_back(this->rule_x5a(cr));  break;
          case class_LRI:  s.push_back(this->rule_x5b(cr));  break;
          case class_FSI:
            /* If FSI is *the* last character, we have a degenerate
             * string, and should be able to ignore the FSI.
             */
            if (pos < str.size() - 1)
                s.push_back(this->rule_x5c(cr, str.substr(pos + 1)));
            break;
          case class_PDI:  s.push_back(this->rule_x6a(cr));  break;
          case class_PDF:  s.push_back(this->rule_x7(cr));   break;
          case class_B:    s.push_back(this->rule_x8(cr));   break;
          default:
            /* Rule X6 */
            if (cr.c_class != class_BN)
                cr.c_class = this->reset_direction_class(cr.c_class);
            s.push_back(cr);
            break;
        }
        ++pos;
    }
    return s;
}

bidi::character_rec& bidi::rule_x2(bidi::character_rec& cr)
{
    int embed = this->direction_stack.top().embed;

    if (this->overflow_isolate == 0)
    {
        if (this->overflow_embed == 0
            && this->direction_stack.size() < bidi::MAX_STACK_SIZE)
        {
            if (++embed % 2 == 0)
                ++embed;
            this->direction_stack.push({embed, direction_rec::NEUTRAL, false});
        }
        else
            ++this->overflow_embed;
    }
    cr.embed = embed;
    return cr;
}

bidi::character_rec& bidi::rule_x3(bidi::character_rec& cr)
{
    int embed = this->direction_stack.top().embed;

    if (this->overflow_isolate == 0)
    {
        if (this->overflow_embed == 0
            && this->direction_stack.size() < bidi::MAX_STACK_SIZE)
        {
            if (++embed % 2 == 1)
                ++embed;
            this->direction_stack.push({embed, direction_rec::NEUTRAL, false});
        }
        else
            ++this->overflow_embed;
    }
    cr.embed = embed;
    return cr;
}

bidi::character_rec& bidi::rule_x4(bidi::character_rec& cr)
{
    int embed = this->direction_stack.top().embed;

    if (this->overflow_isolate == 0)
    {
        if (this->overflow_embed == 0
            && this->direction_stack.size() < bidi::MAX_STACK_SIZE)
        {
            if (++embed % 2 == 0)
                ++embed;
            this->direction_stack.push({embed, direction_rec::RTL, false});
        }
        else
            ++this->overflow_embed;
    }
    cr.embed = embed;
    return cr;
}

bidi::character_rec& bidi::rule_x5(bidi::character_rec& cr)
{
    int embed = this->direction_stack.top().embed;

    if (this->overflow_isolate == 0)
    {
        if (this->overflow_embed == 0
            && this->direction_stack.size() < bidi::MAX_STACK_SIZE)
        {
            if (++embed % 2 == 1)
                ++embed;
            this->direction_stack.push({embed, direction_rec::LTR, false});
        }
        else
            ++this->overflow_embed;
    }
    cr.c_class = this->reset_direction_class(cr.c_class);
    cr.embed = embed;
    return cr;
}

bidi::character_rec& bidi::rule_x5a(bidi::character_rec& cr)
{
    int embed = this->direction_stack.top().embed;

    cr.c_class = this->reset_direction_class(cr.c_class);
    cr.embed = embed;
    if (this->overflow_isolate == 0
        && this->overflow_embed == 0
        && this->direction_stack.size() < bidi::MAX_STACK_SIZE)
    {
        if (++embed % 2 == 0)
            ++embed;
        this->direction_stack.push({embed, direction_rec::NEUTRAL, true});
        ++this->valid_isolate;
    }
    else
        ++this->overflow_isolate;
    return cr;
}

bidi::character_rec& bidi::rule_x5b(bidi::character_rec& cr)
{
    int embed = this->direction_stack.top().embed;

    cr.c_class = this->reset_direction_class(cr.c_class);
    cr.embed = embed;
    if (this->overflow_isolate == 0
        && this->overflow_embed == 0
        && this->direction_stack.size() < bidi::MAX_STACK_SIZE)
    {
        if (++embed % 2 == 1)
            ++embed;
        this->direction_stack.push({embed, direction_rec::NEUTRAL, true});
        ++this->valid_isolate;
    }
    else
        ++this->overflow_isolate;
    return cr;
}

bidi::character_rec& bidi::rule_x5c(bidi::character_rec& cr,
                                    const std::u32string& s)
{
    int embed = this->rule_p2_p3(s);

    if (embed == 1)
        return this->rule_x5a(cr);
    return this->rule_x5b(cr);
}

bidi::character_rec& bidi::rule_x6a(bidi::character_rec& cr)
{
    if (this->overflow_isolate > 0)
        --this->overflow_isolate;
    else if (this->valid_isolate > 0)
    {
        this->overflow_embed = 0;
        while (this->direction_stack.top().isolate == false)
            this->direction_stack.pop();
        this->direction_stack.pop();
        --this->valid_isolate;
    }

    cr.c_class = this->reset_direction_class(cr.c_class);
    cr.embed = this->direction_stack.top().embed;
    return cr;
}

bidi::character_rec& bidi::rule_x7(bidi::character_rec& cr)
{
    if (this->overflow_isolate > 0)
        return cr;
    else if (this->overflow_embed > 0)
        --this->overflow_embed;
    else if (this->direction_stack.size() >= 2
             && this->direction_stack.top().isolate == false)
        this->direction_stack.pop();
    cr.embed = this->direction_stack.top().embed;
    return cr;
}

bidi::character_rec& bidi::rule_x8(bidi::character_rec& cr)
{
    while (this->direction_stack.size() > 1)
        this->direction_stack.pop();
    this->overflow_isolate = this->overflow_embed = this->valid_isolate = 0;
    cr.embed = this->direction_stack.top().embed;
    return cr;
}

void bidi::rule_x9(bidi::char_container& str)
{
    auto pos = str.begin();

    while (pos != str.end())
    {
        if ((*pos).c == RLE || (*pos).c == LRE
            || (*pos).c == RLO || (*pos).c == LRO
            || (*pos).c == PDF || bidi::char_type((*pos).c) == class_BN)
            str.erase(--pos + 1);
        ++pos;
    }
}

void bidi::rule_w1(bidi::run_sequence& seq)
{
    auto i = seq.start;

    do
        if (i->c_class == class_NSM)
        {
            if (i == seq.start)
                i->c_class = seq.sos;
            else
            {
                auto prev = (*(i - 1)).c_class;
                if (prev == class_LRI || prev == class_RLI || prev == class_PDI)
                    i->c_class = class_ON;
                else
                    i->c_class = prev;
            }
        }
    while (i++ != seq.end);
}

void bidi::rule_w2(bidi::run_sequence& seq)
{
    auto i = seq.end;

    do
        if (i->c_class == class_EN)
        {
            auto j = i - 1;
            do
            {
                if (j->c_class == class_R
                    || j->c_class == class_L
                    || j->c_class == class_AL)
                {
                    if (j->c_class == class_AL)
                        i->c_class = class_AN;
                    break;
                }
                if (j == seq.start && seq.sos == class_AL)
                    i->c_class = class_AN;
            }
            while (j-- != seq.start);
        }
    while (i-- != seq.start);
}

void bidi::rule_w3(bidi::run_sequence& seq)
{
    auto i = seq.start;

    do
        if (i->c_class == class_AL)
            i->c_class = class_R;
    while (i++ != seq.end);
}

void bidi::rule_w4(bidi::run_sequence& seq)
{
    auto i = seq.start + 1;

    do
        if (i->c_class == class_ES
            && (*(i - 1)).c_class == class_EN
            && (*(i + 1)).c_class == class_EN)
        {
            i->c_class = class_EN;
        }
        else if (i->c_class == class_CS
                 && ((*(i - 1)).c_class == class_EN
                     || (*(i - 1)).c_class == class_AN)
                 && (*(i - 1)).c_class == (*(i + 1)).c_class)
        {
            i->c_class = (*(i - 1)).c_class;
        }
    while (++i != seq.end);
}

void bidi::rule_w5(bidi::run_sequence& seq)
{
    auto i = seq.start;

    do
        if (i->c_class == class_ET)
        {
            if (i != seq.start && (*(i - 1)).c_class == class_EN)
                i->c_class = class_EN;
            else
            {
                auto j = i + 1;
                while (j != seq.end && j->c_class == class_ET)
                    ++j;
                if (j->c_class == class_EN)
                {
                    while (i != j)
                    {
                        i->c_class = class_EN;
                        ++i;
                    }
                }
            }
        }
    while (i++ != seq.end);
}

void bidi::rule_w6(bidi::run_sequence& seq)
{
    auto i = seq.start;

    do
        if (i->c_class == class_ET
            || i->c_class == class_ES
            || i->c_class == class_CS)
        {
            i->c_class = class_ON;
        }
    while (i++ != seq.end);
}

void bidi::rule_w7(bidi::run_sequence& seq)
{
    auto i = seq.end;

    do
        if (i->c_class == class_EN)
        {
            auto j = i - 1;
            do
            {
                if (j->c_class == class_R
                    || j->c_class == class_L
                    || j->c_class == class_AL)
                {
                    if (j->c_class == class_L)
                        i->c_class = class_L;
                    break;
                }
                if (j == seq.start && seq.sos == class_L)
                    i->c_class = class_L;
            }
            while (j-- != seq.start);
        }
    while (i-- != seq.start);
}

void bidi::bd16(bidi::run_sequence& seq)
{
    std::deque<bidi::stack_entry_t> bracket_stack;
    auto i = seq.start;

    do
    {
        auto j = bracket_chars.find(i->c);
        if (j != bracket_chars.end())
        {
            if (j->second.which == bracket_t::o)
            {
                if (bracket_stack.size() < bidi::MAX_PAIR_STACK_SIZE)
                    bracket_stack.push_back({i, i->c, j->second});
                else
                    i = seq.end;
            }
            else
            {
                auto k = bracket_stack.end() - 1;
                while (k >= bracket_stack.begin())
                {
                    if (k->b.mate == i->c)
                    {
                        seq.bracket_pairs.push_back({k->start, i});
                        bracket_stack.erase(k, bracket_stack.end());
                        break;
                    }
                    --k;
                }
            }
        }
    }
    while (i++ != seq.end);
}

void bidi::rule_n0(bidi::run_sequence& seq)
{
    this->bd16(seq);

    for (auto& i : seq.bracket_pairs)
    {
        auto j = i.first;
        do
            if (j->c_class == class_R || j->c_class == class_L)
            {
                if ((j->c_class == class_R && j->embed % 2 == 1)
                    || (j->c_class == class_L && j->embed % 2 == 0))
                {
                    this->set_paired_brackets(i, j->c_class, seq);
                }
                else if (i.first != seq.start)
                {
                    auto k = i.first;
                    do
                        if (k->c_class == j->c_class)
                        {
                            this->set_paired_brackets(i, j->c_class, seq);
                            break;
                        }
                    while (k-- != seq.start);
                    if (k == seq.start - 1)
                    {
                        if (seq.sos == j->c_class)
                            this->set_paired_brackets(i, seq.sos, seq);
                        else
                        {
                            auto dir = i.first->embed % 2 ? class_R : class_L;
                            this->set_paired_brackets(i, dir, seq);
                        }
                    }
                }
                break;
            }
        while (j++ != i.second);
    }
}

void bidi::set_paired_brackets(char_pair_t& brackets,
                               char_class_t type,
                               bidi::run_sequence& seq)
{
    brackets.first->c_class = brackets.second->c_class = type;
    auto i = brackets.first + 1;
    while (bidi::char_type(i->c) == class_NSM)
    {
        i->c_class = type;
        ++i;
    }
    i = brackets.second;
    while (++i != seq.end + 1 && bidi::char_type(i->c) == class_NSM)
        i->c_class = type;
}

#define is_NI(x) ((x)->c_class == class_B || (x)->c_class == class_S \
                  || (x)->c_class == class_WS || (x)->c_class == class_ON \
                  || (x)->c == FSI || (x)->c == LRI || (x)->c == RLI \
                  || (x)->c == PDI)
#define N1_class(x) ((x)->c_class == class_AN || (x)->c_class == class_EN \
                     ? class_R : (x)->c_class)

void bidi::rule_n1(bidi::run_sequence& seq)
{
    auto i = seq.start + 1;

    do
        if (is_NI(i)
            && ((i - 1)->c_class == class_L
                || (i - 1)->c_class == class_R
                || (i - 1)->c_class == class_AN
                || (i - 1)->c_class == class_EN))
        {
            char_class_t prev = N1_class(i - 1);
            auto j = i + 1;

            while (j != seq.end && is_NI(j))
                ++j;

            if (N1_class(j) == prev)
                do
                    i->c_class = prev;
                while (++i != j);

            if (i == seq.end)
                break;
        }
    while (++i != seq.end);
}

bidi::bidi()
    : direction_stack()
{
    this->overflow_isolate = this->overflow_embed = this->valid_isolate = 0;
}

bidi::~bidi()
{
}
