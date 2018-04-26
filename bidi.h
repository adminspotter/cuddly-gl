/* bidi.h                                                  -*- C++ -*-
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 25 Apr 2018, 18:29:59 tquirk
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
 * Prototypes, types, and defines for the Unicode BIDI algorithm.
 * See:  http://unicode.org/reports/tr9/
 *
 * Things to do
 *
 */

#ifndef __INC_CUDDLY_BIDI_H__
#define __INC_CUDDLY_BIDI_H__

#include <string>
#include <vector>
#include <unordered_set>
#include <stack>

typedef enum {
    class_AL, class_AN, class_B, class_BN, class_CS, class_EN, class_ES,
    class_ET, class_FSI, class_L, class_LRE, class_LRI, class_LRO, class_NSM,
    class_ON, class_PDF, class_PDI, class_R, class_RLE, class_RLI, class_RLO,
    class_S, class_WS
}
char_class_t;

extern const std::unordered_set<char32_t> AL, AN, B, BN, CS, EN, ES;
extern const std::unordered_set<char32_t> ET, NSM, ON, R, S, WS;

extern const int LRM, RLM, ALM, LRE, RLE, PDF, LRO, RLO, LRI, RLI, FSI, PDI;

class unicode_bidi
{
  protected:
    typedef struct
    {
        int embed;
        enum { LTR, RTL, NEUTRAL } override;
        bool isolate;
    }
    direction_rec;

    std::stack<direction_rec> direction_stack;
    int overflow_isolate, overflow_embed, valid_isolate;

    static char_class_t char_type(char32_t);

    std::vector<std::u32string> rule_p1(const std::u32string&);
    int rule_p2_p3(const std::u32string&);

  public:
    unicode_bidi();
    ~unicode_bidi();

};

#endif /* __INC_CUDDLY_BIDI_H__ */
