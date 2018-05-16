/* bidi.h                                                  -*- C++ -*-
 *   by Trinity Quirk <tquirk@ymb.net>
 *   last updated 16 May 2018, 08:22:25 tquirk
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
#include <unordered_map>
#include <deque>
#include <stack>
#include <utility>

typedef enum {
    class_AL, class_AN, class_B, class_BN, class_CS, class_EN, class_ES,
    class_ET, class_FSI, class_L, class_LRE, class_LRI, class_LRO, class_NSM,
    class_ON, class_PDF, class_PDI, class_R, class_RLE, class_RLI, class_RLO,
    class_S, class_WS
}
char_class_t;

extern const std::unordered_set<char32_t> AL, AN, B, BN, CS, EN, ES;
extern const std::unordered_set<char32_t> ET, NSM, ON, R, S, WS;

extern const char32_t LRM, RLM, ALM, LRE, RLE, PDF, LRO, RLO;
extern const char32_t LRI, RLI, FSI, PDI;

typedef struct
{
    enum {o, c} which;
    char32_t mate;
}
bracket_t;

extern const std::unordered_map<char32_t, bracket_t> bracket_chars;

class bidi
{
  protected:
    typedef struct
    {
        int embed;
        enum { LTR, RTL, NEUTRAL } override;
        bool isolate;
    }
    direction_rec;

    typedef struct
    {
        char32_t c;
        char_class_t c_class;
        int embed;
    }
    character_rec;
    typedef std::deque<character_rec> char_container;
    typedef char_container::iterator char_cont_it_t;
    typedef std::pair<char_cont_it_t, char_cont_it_t> char_pair_t;

    typedef struct
    {
        char_cont_it_t start, end;
        char_class_t sos, eos;
        std::deque<char_pair_t> bracket_pairs;
    }
    run_sequence;
    typedef std::deque<run_sequence> sequences;

    typedef struct
    {
        char_cont_it_t start;
        char32_t c;
        bracket_t b;
    }
    stack_entry_t;

    static const int MAX_DEPTH, MAX_STACK_SIZE, MAX_PAIR_STACK_SIZE;

    static char_class_t char_type(char32_t);

    std::stack<direction_rec> direction_stack;
    int overflow_isolate, overflow_embed, valid_isolate;

    char_class_t reset_direction_class(char_class_t);

    std::vector<std::u32string> rule_p1(const std::u32string&);
    virtual int rule_p2_p3(const std::u32string&);

    char_container rule_x1(int, const std::u32string&);
    character_rec& rule_x2(character_rec&);
    character_rec& rule_x3(character_rec&);
    character_rec& rule_x4(character_rec&);
    character_rec& rule_x5(character_rec&);
    virtual character_rec& rule_x5a(character_rec&);
    virtual character_rec& rule_x5b(character_rec&);
    character_rec& rule_x5c(character_rec&, const std::u32string&);
    character_rec& rule_x6a(character_rec&);
    character_rec& rule_x7(character_rec&);
    character_rec& rule_x8(character_rec&);

    void rule_x9(char_container&);

    virtual bidi::sequences bd13(int, char_container&);
    void rule_x10(int, char_container&);
    virtual void compute_sos_eos(char_container&, int, run_sequence&);

    virtual void rule_w1(run_sequence&);
    virtual void rule_w2(run_sequence&);
    virtual void rule_w3(run_sequence&);
    virtual void rule_w4(run_sequence&);
    virtual void rule_w5(run_sequence&);
    virtual void rule_w6(run_sequence&);
    virtual void rule_w7(run_sequence&);

    void bd16(run_sequence&);
    virtual void rule_n0(run_sequence&);
    void set_paired_brackets(char_pair_t&, char_class_t, run_sequence&);
    virtual void rule_n1(run_sequence&);
    virtual void rule_n2(run_sequence&);

    virtual void rule_i1(run_sequence&);
    virtual void rule_i2(run_sequence&);

    void rule_l1(int, char_container&);
    void rule_l2(int, char_container&);

  public:
    bidi();
    virtual ~bidi();

    std::vector<std::u32string> reorder(const std::u32string&);
};

#endif /* __INC_CUDDLY_BIDI_H__ */
