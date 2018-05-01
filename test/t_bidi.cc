#include <tap++.h>

using namespace TAP;

#include "../bidi.h"
#include "../font.h"

class fake_bidi : public bidi
{
  public:
    fake_bidi() : bidi() {};
    ~fake_bidi() {};

    using bidi::MAX_DEPTH;
    using bidi::MAX_STACK_SIZE;

    using bidi::direction_rec;
    using bidi::character_rec;

    using bidi::direction_stack;
    using bidi::overflow_isolate;
    using bidi::overflow_embed;
    using bidi::valid_isolate;

    using bidi::char_type;
    using bidi::reset_direction_class;
    using bidi::rule_p1;
    using bidi::rule_p2_p3;
    using bidi::rule_x1;
    using bidi::rule_x2;
    using bidi::rule_x3;
    using bidi::rule_x4;
    using bidi::rule_x5;
    using bidi::rule_x5a;
    using bidi::rule_x5b;
    using bidi::rule_x6a;
    using bidi::rule_x7;
    using bidi::rule_x8;
};

class mock_x5c_bidi : public bidi
{
  public:
    int fake_embed;

    mock_x5c_bidi() : bidi() {};
    ~mock_x5c_bidi() {};

    using bidi::character_rec;

    virtual int rule_p2_p3(const std::u32string& s)
        {
            return fake_embed;
        };
    virtual character_rec& rule_x5a(character_rec& cr)
        {
            cr = {'a', class_BN, 99};
            return cr;
        };
    virtual character_rec& rule_x5b(character_rec& cr)
        {
            cr = {'b', class_ES, 58};
            return cr;
        };

    using bidi::rule_x5c;
};

void test_create_delete(void)
{
    std::string test = "create/delete: ";
    fake_bidi *b = NULL;

    try
    {
        b = new fake_bidi();
    }
    catch (...)
    {
        fail(test + "constructor exception");
    }

    is(b->direction_stack.size(), 0, test + "expected stack size");
    is(b->overflow_isolate, 0, test + "expected overflow isolate count");
    is(b->overflow_embed, 0, test + "expected overflow embed count");
    is(b->valid_isolate, 0, test + "expected valid isolate count");

    delete b;
}

void test_char_type(void)
{
    std::string test = "char_type: ";

    fake_bidi b;

    is(b.char_type(0x202a), class_LRE, test + "expected LRE");
    is(b.char_type(0x202b), class_RLE, test + "expected RLE");
    is(b.char_type(0x202c), class_PDF, test + "expected PDF");
    is(b.char_type(0x202d), class_LRO, test + "expected LRO");
    is(b.char_type(0x202e), class_RLO, test + "expected RLO");
    is(b.char_type(0x2066), class_LRI, test + "expected LRI");
    is(b.char_type(0x2067), class_RLI, test + "expected RLI");
    is(b.char_type(0x2068), class_FSI, test + "expected FSI");
    is(b.char_type(0x2069), class_PDI, test + "expected PDI");
    is(b.char_type(0x061c), class_AL, test + "expected ALM AL");
    is(b.char_type(0x0627), class_AL, test + "expected char AL");
    is(b.char_type(0x200f), class_R, test + "expected RLM R");
    is(b.char_type(0x05d0), class_R, test + "expected char R");
    is(b.char_type(0x066b), class_AN, test + "expected AN");
    is(b.char_type(0x0085), class_B, test + "expected B");
    is(b.char_type(0x206a), class_BN, test + "expected BN");
    is(b.char_type(0xff0c), class_CS, test + "expected CS");
    is(b.char_type(0x0039), class_EN, test + "expected EN");
    is(b.char_type(0x207a), class_ES, test + "expected ES");
    is(b.char_type(0x20a9), class_ET, test + "expected ET");
    is(b.char_type(0x065a), class_NSM, test + "expected NSM");
    is(b.char_type(0x1809), class_ON, test + "expected ON");
    is(b.char_type(0x000b), class_S, test + "expected S");
    is(b.char_type(0x2005), class_WS, test + "expected WS");
    is(b.char_type(0x200e), class_L, test + "expected LRM L");
    is(b.char_type(0x0061), class_L, test + "expected char L");
}

void test_reset_direction_class(void)
{
    std::string test = "reset_direction_class: ";

    fake_bidi b;

    b.direction_stack.push({0, fake_bidi::direction_rec::NEUTRAL, false});
    is(b.reset_direction_class(class_EN), class_EN,
       test + "neutral: expected class");

    b.direction_stack.push({0, fake_bidi::direction_rec::LTR, false});
    is(b.reset_direction_class(class_EN), class_L,
       test + "LTR: expected class");

    b.direction_stack.push({0, fake_bidi::direction_rec::RTL, false});
    is(b.reset_direction_class(class_EN), class_R,
       test + "RTL: expected class");
}

void test_rule_p1(void)
{
    std::string test = "rule_p1: ", st;

    fake_bidi b;

    st = "no newlines: ";

    std::u32string no_newlines = ui::utf8tou32str("abcde12345");

    std::vector<std::u32string> res1 = b.rule_p1(no_newlines);

    is(res1.size(), 1, test + st + "expected vector size");
    is(ui::u32strtoutf8(res1[0]), "abcde12345",
       test + st + "expected string");

    st = "newline at beginning: ";

    std::u32string leading_lf = ui::utf8tou32str("\nabc123");

    std::vector<std::u32string> res2 = b.rule_p1(leading_lf);

    is(res2.size(), 2, test + st + "expected vector size");
    is(ui::u32strtoutf8(res2[0]), "\n", test + st + "expected string 1");
    is(ui::u32strtoutf8(res2[1]), "abc123", test + st + "expected string 2");

    st = "newline at end: ";

    std::u32string trailing_lf = ui::utf8tou32str("abc123\n");

    std::vector<std::u32string> res3 = b.rule_p1(trailing_lf);

    is(res3.size(), 2, test + st + "expected vector size");
    is(ui::u32strtoutf8(res3[0]), "abc123\n", test + st + "expected string 1");
    is(ui::u32strtoutf8(res3[1]), "", test + st + "expected string 2");

    st = "newline in middle: ";

    std::u32string middle_lf = ui::utf8tou32str("abc\n123");

    std::vector<std::u32string> res4 = b.rule_p1(middle_lf);

    is(res4.size(), 2, test + st + "expected vector size");
    is(ui::u32strtoutf8(res4[0]), "abc\n", test + st + "expected string 1");
    is(ui::u32strtoutf8(res4[1]), "123", test + st + "expected string 2");

    st = "crlf: ";

    std::u32string crlf = { 'a', 'b', 'c', 0x0d, 0x0a, '1', '2', '3' };

    std::vector<std::u32string> res5 = b.rule_p1(crlf);

    is(res5.size(), 2, test + st + "expected vector size");
    is(res5[0] == std::u32string({ 'a', 'b', 'c', 0x2029 }), true,
       test + st + "expected string 1");
    is(ui::u32strtoutf8(res5[1]), "123", test + st + "expected string 2");

    st = "paragraph separator: ";

    std::u32string para_sep = { 'a', 'b', 'c', 0x2029, '1', '2', '3' };

    std::vector<std::u32string> res6 = b.rule_p1(para_sep);

    is(res6.size(), 2, test + st + "expected vector size");
    is(res6[0] == std::u32string({ 'a', 'b', 'c', 0x2029 }), true,
       test + st + "expected string 1");
    is(ui::u32strtoutf8(res6[1]), "123", test + st + "expected string 2");
}

void test_rule_p2_p3(void)
{
    std::string test = "rule_p2_p3: ";

    fake_bidi b;

    std::u32string empty;
    is(b.rule_p2_p3(empty), 0, test + "empty: expected embedding");

    std::u32string l_first = { 'a' };
    is(b.rule_p2_p3(l_first), 0, test + "L first: expected embedding");

    std::u32string r_first = { 0x5d0 };
    is(b.rule_p2_p3(r_first), 1, test + "R first: expected embedding");

    std::u32string al_first = { 0x627 };
    is(b.rule_p2_p3(al_first), 1, test + "AL first: expected embedding");

    std::u32string l_before_r = { 'a', 0x5d0 };
    is(b.rule_p2_p3(l_before_r), 0, test + "L before R: expected embedding");

    std::u32string l_before_al = { 'a', 0x627 };
    is(b.rule_p2_p3(l_before_al), 0, test + "L before AL: expected embedding");

    std::u32string neutral_l = { '-', 'a' };
    is(b.rule_p2_p3(neutral_l), 0,
       test + "L after neutral: expected embedding");

    std::u32string neutral_r = { '-', 0x5d0 };
    is(b.rule_p2_p3(neutral_r), 1,
       test + "R after neutral: expected embedding");

    std::u32string neutral_al = { '-', 0x627 };
    is(b.rule_p2_p3(neutral_al), 1,
       test + "AL after neutral: expected embedding");

    std::u32string all_isolate = { 0x2066, 0x5d0, 0x2069 };
    is(b.rule_p2_p3(all_isolate), 0,
       test + "all isolate: expected embedding");

    std::u32string isolate_l = { 0x2066, 0x5d0, 0x2069, 'a' };
    is(b.rule_p2_p3(isolate_l), 0,
       test + "L after isolate: expected embedding");

    std::u32string isolate_r = { 0x2066, 'a', 0x2069, 0x5d0 };
    is(b.rule_p2_p3(isolate_r), 1,
       test + "R after isolate: expected embedding");

    std::u32string isolate_al = { 0x2066, 'a', 0x2069, 0x627 };
    is(b.rule_p2_p3(isolate_al), 1,
       test + "AL after isolate: expected embedding");

    std::u32string nested =
        { 0x2066, 'a', 0x2067, 'b', 0x2069, 0x2069, 0x627 };
    is(b.rule_p2_p3(nested), 1,
       test + "nested isolates: expected embedding");

    std::u32string extra = { 0x2069, 0x627 };
    is(b.rule_p2_p3(extra), 0,
       test + "extra isolate terminator: expected embedding");
}

void test_rule_x1(void)
{
    std::string test = "rule_x1: ", st;

    fake_bidi b;

    st = "base initialization: ";

    b.direction_stack.push({1, fake_bidi::direction_rec::RTL, false});

    b.rule_x1(9, std::u32string());

    is(b.direction_stack.size(), 1, test + st + "expected stack size");
    is(b.direction_stack.top().embed, 9, test + st + "expected base embed");
    is(b.direction_stack.top().override, fake_bidi::direction_rec::NEUTRAL,
       test + st + "expected base direction");
    is(b.direction_stack.top().isolate, false,
       test + st + "expected base isolate");
    is(b.overflow_isolate, 0, test + st + "expected overflow isolate");
    is(b.overflow_embed, 0, test + st + "expected overflow embed");
    is(b.valid_isolate, 0, test + st + "expected valid isolate");

    st = "simple LTR: ";

    std::u32string simple_ltr = {'a'};

    auto result = b.rule_x1(0, simple_ltr);

    is(result.size(), 1, test + st + "expected result size");
    is(result.front().c, 'a', test + st + "expected character");
    is(result.front().c_class, class_L, test + st + "expected type");
    is(result.front().embed, 0, test + st + "expected embed");

    st = "simple RTL: ";

    std::u32string simple_rtl = {0x05d0};

    result = b.rule_x1(1, simple_rtl);

    is(result.size(), 1, test + st + "expected result size");
    is(result.front().c, 0x05d0, test + st + "expected character");
    is(result.front().c_class, class_R, test + st + "expected type");
    is(result.front().embed, 1, test + st + "expected embed");

    st = "class RLE: ";

    std::u32string with_rle = {'a', RLE, 'b'};

    result = b.rule_x1(0, with_rle);

    is(result.size(), 3, test + st + "expected result size");
    is(result.back().c, 'b', test + st + "expected character");
    is(result.back().c_class, class_L, test + st + "expected type");
    is(result.back().embed, 1, test + st + "expected embed");

    st = "class LRE: ";

    std::u32string with_lre = {0x05d0, LRE, 0x05d1};

    result = b.rule_x1(1, with_lre);

    is(result.size(), 3, test + st + "expected result size");
    is(result.back().c, 0x05d1, test + st + "expected character");
    is(result.back().c_class, class_R, test + st + "expected type");
    is(result.back().embed, 2, test + st + "expected embed");

    st = "class RLO: ";

    std::u32string with_rlo = {'a', RLO, 'b'};

    result = b.rule_x1(0, with_rlo);

    is(result.size(), 3, test + st + "expected result size");
    is(result.back().c, 'b', test + st + "expected character");
    is(result.back().c_class, class_R, test + st + "expected type");
    is(result.back().embed, 1, test + st + "expected embed");

    st = "class LRO: ";

    std::u32string with_lro = {0x05d0, LRO, 0x05d1};

    result = b.rule_x1(1, with_lro);

    is(result.size(), 3, test + st + "expected result size");
    is(result.back().c, 0x05d1, test + st + "expected character");
    is(result.back().c_class, class_L, test + st + "expected type");
    is(result.back().embed, 2, test + st + "expected embed");

    st = "class PDF: ";

    std::u32string with_pdf = {'a', RLE, 'b', PDF};

    result = b.rule_x1(0, with_pdf);

    is(result.size(), 4, test + st + "expected result size");
    is(result.back().c, PDF, test + st + "expected character");
    is(result.back().c_class, class_PDF, test + st + "expected type");
    is(result.back().embed, 0, test + st + "expected embed");

    st = "class B: ";

    std::u32string with_b = {'a', RLE, 'b', 0x000d};

    result = b.rule_x1(0, with_b);

    is(result.size(), 4, test + st + "expected result size");
    is(result.back().c, 0x000d, test + st + "expected character");
    is(result.back().c_class, class_B, test + st + "expected type");
    is(result.back().embed, 0, test + st + "expected embed");

    st = "class BN: ";

    std::u32string with_bn = {'a', RLO, 0x206e};

    result = b.rule_x1(0, with_bn);

    is(result.size(), 3, test + st + "expected result size");
    is(result.back().c, 0x206e, test + st + "expected character");
    is(result.back().c_class, class_BN, test + st + "expected type");
    is(result.back().embed, 1, test + st + "expected embed");
}

void test_rule_x2(void)
{
    std::string test = "rule_x2: ", st;

    fake_bidi b;

    b.direction_stack.push({14, fake_bidi::direction_rec::RTL, true});

    st = "even stack embed: ";

    fake_bidi::character_rec cr = {'a', class_L, 4};
    fake_bidi::character_rec new_cr = b.rule_x2(cr);

    is(new_cr.c, 'a', test + st + "expected character");
    is(new_cr.c_class, class_L, test + st + "expected type");
    is(new_cr.embed, 15, test + st + "expected embed");

    is(b.direction_stack.size(), 2, test + st + "expected stack size");
    is(b.direction_stack.top().embed, 15, test + st + "expected stack embed");
    is(b.direction_stack.top().override, fake_bidi::direction_rec::NEUTRAL,
       test + st + "expected stack override");
    is(b.direction_stack.top().isolate, false,
       test + st + "expected stack isolate");

    st = "odd stack embed: ";

    b.direction_stack.push({5, fake_bidi::direction_rec::RTL, true});
    fake_bidi::character_rec new_cr2 = b.rule_x2(cr);

    is(b.direction_stack.size(), 4, test + st + "expected stack size");
    is(new_cr2.embed, 7, test + st + "expected embed");

    st = "nonzero embed overflow: ";

    b.overflow_embed = 1;

    fake_bidi::character_rec new_cr3 = b.rule_x2(cr);

    is(b.direction_stack.size(), 4, test + st + "no new stack entries");
    is(new_cr3.embed, 7, test + st + "expected embed");
    is(b.overflow_embed, 2, test + st + "expected embed overflow");

    st = "nonzero isolate overflow: ";

    b.overflow_isolate = 1;

    fake_bidi::character_rec new_cr4 = b.rule_x2(cr);

    is(b.direction_stack.size(), 4, test + st + "no new stack entries");
    is(new_cr4.embed, 7, test + st + "exepected embed");
    is(b.overflow_embed, 2, test + st + "expected embed overflow");

    st = "too-deep stack: ";

    b.overflow_isolate = b.overflow_embed = 0;
    int i;
    for (i = 0; i < fake_bidi::MAX_STACK_SIZE; ++i)
        b.direction_stack.push({i, fake_bidi::direction_rec::RTL, true});

    fake_bidi::character_rec new_cr5 = b.rule_x2(cr);

    is(b.overflow_embed, 1, test + st + "expected embed overflow");
    is(new_cr5.embed, fake_bidi::MAX_STACK_SIZE - 1,
       test + st + "exepected embed");
}

void test_rule_x3(void)
{
    std::string test = "rule_x3: ", st;

    fake_bidi b;

    b.direction_stack.push({14, fake_bidi::direction_rec::RTL, true});

    st = "even stack embed: ";

    fake_bidi::character_rec cr = {'a', class_L, 4};
    fake_bidi::character_rec new_cr = b.rule_x3(cr);

    is(new_cr.c, 'a', test + st + "expected character");
    is(new_cr.c_class, class_L, test + st + "expected type");
    is(new_cr.embed, 16, test + st + "expected embed");

    is(b.direction_stack.size(), 2, test + st + "expected stack size");
    is(b.direction_stack.top().embed, 16, test + st + "expected stack embed");
    is(b.direction_stack.top().override, fake_bidi::direction_rec::NEUTRAL,
       test + st + "expected stack override");
    is(b.direction_stack.top().isolate, false,
       test + st + "expected stack isolate");

    st = "odd stack embed: ";

    b.direction_stack.push({5, fake_bidi::direction_rec::RTL, true});
    fake_bidi::character_rec new_cr2 = b.rule_x3(cr);

    is(b.direction_stack.size(), 4, test + st + "expected stack size");
    is(new_cr2.embed, 6, test + st + "expected embed");

    st = "nonzero embed overflow: ";

    b.overflow_embed = 1;

    fake_bidi::character_rec new_cr3 = b.rule_x3(cr);

    is(b.direction_stack.size(), 4, test + st + "no new stack entries");
    is(new_cr3.embed, 6, test + st + "expected embed");
    is(b.overflow_embed, 2, test + st + "expected embed overflow");

    st = "nonzero isolate overflow: ";

    b.overflow_isolate = 1;

    fake_bidi::character_rec new_cr4 = b.rule_x3(cr);

    is(b.direction_stack.size(), 4, test + st + "no new stack entries");
    is(new_cr4.embed, 6, test + st + "exepected embed");
    is(b.overflow_embed, 2, test + st + "expected embed overflow");

    st = "too-deep stack: ";

    b.overflow_isolate = b.overflow_embed = 0;
    int i;
    for (i = 0; i < fake_bidi::MAX_STACK_SIZE; ++i)
        b.direction_stack.push({i, fake_bidi::direction_rec::RTL, true});

    fake_bidi::character_rec new_cr5 = b.rule_x3(cr);

    is(b.overflow_embed, 1, test + st + "expected embed overflow");
    is(new_cr5.embed, fake_bidi::MAX_STACK_SIZE - 1,
       test + st + "exepected embed");
}

void test_rule_x4(void)
{
    std::string test = "rule_x4: ", st;

    fake_bidi b;

    b.direction_stack.push({14, fake_bidi::direction_rec::NEUTRAL, true});

    st = "even stack embed: ";

    fake_bidi::character_rec cr = {'a', class_L, 4};
    fake_bidi::character_rec new_cr = b.rule_x4(cr);

    is(new_cr.c, 'a', test + st + "expected character");
    is(new_cr.c_class, class_L, test + st + "expected type");
    is(new_cr.embed, 15, test + st + "expected embed");

    is(b.direction_stack.size(), 2, test + st + "expected stack size");
    is(b.direction_stack.top().embed, 15, test + st + "expected stack embed");
    is(b.direction_stack.top().override, fake_bidi::direction_rec::RTL,
       test + st + "expected stack override");
    is(b.direction_stack.top().isolate, false,
       test + st + "expected stack isolate");

    st = "odd stack embed: ";

    b.direction_stack.push({5, fake_bidi::direction_rec::NEUTRAL, true});
    fake_bidi::character_rec new_cr2 = b.rule_x4(cr);

    is(b.direction_stack.size(), 4, test + st + "expected stack size");
    is(new_cr2.embed, 7, test + st + "expected embed");

    st = "nonzero embed overflow: ";

    b.overflow_embed = 1;

    fake_bidi::character_rec new_cr3 = b.rule_x4(cr);

    is(b.direction_stack.size(), 4, test + st + "no new stack entries");
    is(new_cr3.embed, 7, test + st + "expected embed");
    is(b.overflow_embed, 2, test + st + "expected embed overflow");

    st = "nonzero isolate overflow: ";

    b.overflow_isolate = 1;

    fake_bidi::character_rec new_cr4 = b.rule_x4(cr);

    is(b.direction_stack.size(), 4, test + st + "no new stack entries");
    is(new_cr4.embed, 7, test + st + "exepected embed");
    is(b.overflow_embed, 2, test + st + "expected embed overflow");

    st = "too-deep stack: ";

    b.overflow_isolate = b.overflow_embed = 0;
    int i;
    for (i = 0; i < fake_bidi::MAX_STACK_SIZE; ++i)
        b.direction_stack.push({i, fake_bidi::direction_rec::RTL, true});

    fake_bidi::character_rec new_cr5 = b.rule_x4(cr);

    is(b.overflow_embed, 1, test + st + "expected embed overflow");
    is(new_cr5.embed, fake_bidi::MAX_STACK_SIZE - 1,
       test + st + "exepected embed");
}

void test_rule_x5(void)
{
    std::string test = "rule_x5: ", st;

    fake_bidi b;

    b.direction_stack.push({14, fake_bidi::direction_rec::NEUTRAL, true});

    st = "even stack embed: ";

    fake_bidi::character_rec cr = {'a', class_L, 4};
    fake_bidi::character_rec new_cr = b.rule_x5(cr);

    is(new_cr.c, 'a', test + st + "expected character");
    is(new_cr.c_class, class_L, test + st + "expected type");
    is(new_cr.embed, 16, test + st + "expected embed");

    is(b.direction_stack.size(), 2, test + st + "expected stack size");
    is(b.direction_stack.top().embed, 16, test + st + "expected stack embed");
    is(b.direction_stack.top().override, fake_bidi::direction_rec::LTR,
       test + st + "expected stack override");
    is(b.direction_stack.top().isolate, false,
       test + st + "expected stack isolate");

    st = "odd stack embed: ";

    b.direction_stack.push({5, fake_bidi::direction_rec::NEUTRAL, true});
    fake_bidi::character_rec new_cr2 = b.rule_x5(cr);

    is(b.direction_stack.size(), 4, test + st + "expected stack size");
    is(new_cr2.embed, 6, test + st + "expected embed");

    st = "nonzero embed overflow: ";

    b.overflow_embed = 1;

    fake_bidi::character_rec new_cr3 = b.rule_x5(cr);

    is(b.direction_stack.size(), 4, test + st + "no new stack entries");
    is(new_cr3.embed, 6, test + st + "expected embed");
    is(b.overflow_embed, 2, test + st + "expected embed overflow");

    st = "nonzero isolate overflow: ";

    b.overflow_isolate = 1;

    fake_bidi::character_rec new_cr4 = b.rule_x5(cr);

    is(b.direction_stack.size(), 4, test + st + "no new stack entries");
    is(new_cr4.embed, 6, test + st + "exepected embed");
    is(b.overflow_embed, 2, test + st + "expected embed overflow");

    st = "too-deep stack: ";

    b.overflow_isolate = b.overflow_embed = 0;
    int i;
    for (i = 0; i < fake_bidi::MAX_STACK_SIZE; ++i)
        b.direction_stack.push({i, fake_bidi::direction_rec::RTL, true});

    fake_bidi::character_rec new_cr5 = b.rule_x5(cr);

    is(b.overflow_embed, 1, test + st + "expected embed overflow");
    is(new_cr5.embed, fake_bidi::MAX_STACK_SIZE - 1,
       test + st + "exepected embed");
}

void test_rule_x5a(void)
{
    std::string test = "rule_x5a: ", st;

    fake_bidi b;

    b.direction_stack.push({14, fake_bidi::direction_rec::RTL, true});

    st = "even stack embed: ";

    fake_bidi::character_rec cr = {'a', class_L, 4};
    fake_bidi::character_rec new_cr = b.rule_x5a(cr);

    is(new_cr.c, 'a', test + st + "expected character");
    is(new_cr.c_class, class_R, test + st + "expected type");
    is(new_cr.embed, 14, test + st + "expected embed");

    is(b.direction_stack.size(), 2, test + st + "expected stack size");
    is(b.direction_stack.top().embed, 15, test + st + "expected stack embed");
    is(b.direction_stack.top().override, fake_bidi::direction_rec::NEUTRAL,
       test + st + "expected stack override");
    is(b.direction_stack.top().isolate, true,
       test + st + "expected stack isolate");
    is(b.valid_isolate, 1, test + st + "expected valid isolate");

    st = "odd stack embed: ";

    b.direction_stack.push({5, fake_bidi::direction_rec::RTL, true});
    fake_bidi::character_rec new_cr2 = b.rule_x5a(cr);

    is(b.direction_stack.size(), 4, test + st + "expected stack size");
    is(new_cr2.embed, 5, test + st + "expected embed");
    is(new_cr2.c_class, class_R, test + st + "expected type");
    is(b.direction_stack.top().embed, 7, test + st + "expected stack embed");
    is(b.valid_isolate, 2, test + st + "expected valid isolate");

    st = "nonzero embed overflow: ";

    b.overflow_embed = 1;

    b.direction_stack.push({5, fake_bidi::direction_rec::RTL, true});
    fake_bidi::character_rec new_cr3 = b.rule_x5a(cr);

    is(b.direction_stack.size(), 5, test + st + "no new stack entries");
    is(new_cr3.embed, 5, test + st + "expected embed");
    is(b.overflow_isolate, 1, test + st + "expected isolate overflow");

    st = "nonzero isolate overflow: ";

    b.overflow_embed = 0;

    fake_bidi::character_rec new_cr4 = b.rule_x5a(cr);

    is(b.direction_stack.size(), 5, test + st + "no new stack entries");
    is(new_cr4.embed, 5, test + st + "expected embed");
    is(b.overflow_isolate, 2, test + st + "expected isolate overflow");

    st = "too-deep stack: ";

    b.overflow_isolate = b.overflow_embed = 0;
    int i;
    for (i = 0; i < fake_bidi::MAX_STACK_SIZE; ++i)
        b.direction_stack.push({i, fake_bidi::direction_rec::RTL, true});

    fake_bidi::character_rec new_cr5 = b.rule_x5a(cr);

    is(b.overflow_isolate, 1, test + st + "expected isolate overflow");
    is(new_cr5.embed, fake_bidi::MAX_STACK_SIZE - 1,
       test + st + "exepected embed");
}

void test_rule_x5b(void)
{
    std::string test = "rule_x5b: ", st;

    fake_bidi b;

    b.direction_stack.push({14, fake_bidi::direction_rec::RTL, true});

    st = "even stack embed: ";

    fake_bidi::character_rec cr = {'a', class_L, 4};
    fake_bidi::character_rec new_cr = b.rule_x5b(cr);

    is(new_cr.c, 'a', test + st + "expected character");
    is(new_cr.c_class, class_R, test + st + "expected type");
    is(new_cr.embed, 14, test + st + "expected embed");

    is(b.direction_stack.size(), 2, test + st + "expected stack size");
    is(b.direction_stack.top().embed, 16, test + st + "expected stack embed");
    is(b.direction_stack.top().override, fake_bidi::direction_rec::NEUTRAL,
       test + st + "expected stack override");
    is(b.direction_stack.top().isolate, true,
       test + st + "expected stack isolate");
    is(b.valid_isolate, 1, test + st + "expected valid isolate");

    st = "odd stack embed: ";

    b.direction_stack.push({5, fake_bidi::direction_rec::RTL, true});
    fake_bidi::character_rec new_cr2 = b.rule_x5b(cr);

    is(b.direction_stack.size(), 4, test + st + "expected stack size");
    is(new_cr2.embed, 5, test + st + "expected embed");
    is(new_cr2.c_class, class_R, test + st + "expected type");
    is(b.direction_stack.top().embed, 6, test + st + "expected stack embed");
    is(b.valid_isolate, 2, test + st + "expected valid isolate");

    st = "nonzero embed overflow: ";

    b.overflow_embed = 1;

    b.direction_stack.push({5, fake_bidi::direction_rec::RTL, true});
    fake_bidi::character_rec new_cr3 = b.rule_x5b(cr);

    is(b.direction_stack.size(), 5, test + st + "no new stack entries");
    is(new_cr3.embed, 5, test + st + "expected embed");
    is(b.overflow_isolate, 1, test + st + "expected isolate overflow");

    st = "nonzero isolate overflow: ";

    b.overflow_embed = 0;

    fake_bidi::character_rec new_cr4 = b.rule_x5b(cr);

    is(b.direction_stack.size(), 5, test + st + "no new stack entries");
    is(new_cr4.embed, 5, test + st + "expected embed");
    is(b.overflow_isolate, 2, test + st + "expected isolate overflow");

    st = "too-deep stack: ";

    b.overflow_isolate = b.overflow_embed = 0;
    int i;
    for (i = 0; i < fake_bidi::MAX_STACK_SIZE; ++i)
        b.direction_stack.push({i, fake_bidi::direction_rec::RTL, true});

    fake_bidi::character_rec new_cr5 = b.rule_x5b(cr);

    is(b.overflow_isolate, 1, test + st + "expected isolate overflow");
    is(new_cr5.embed, fake_bidi::MAX_STACK_SIZE - 1,
       test + st + "exepected embed");
}

void test_rule_x5c(void)
{
    std::string test = "rule_x5c: ", st;

    mock_x5c_bidi b;

    st = "RTL: ";

    b.fake_embed = 1;

    std::u32string str = {'a', 'b', 'c'};

    mock_x5c_bidi::character_rec cr = {'q', class_L, 4};
    mock_x5c_bidi::character_rec new_cr = b.rule_x5c(cr, str);

    is(new_cr.c, 'a', test + st + "expected character");
    is(new_cr.c_class, class_BN, test + st + "expected type");
    is(new_cr.embed, 99, test + st + "expected embed");

    st = "LTR: ";

    b.fake_embed = 0;

    cr = {'q', class_L, 4};
    mock_x5c_bidi::character_rec new_cr2 = b.rule_x5c(cr, str);

    is(new_cr2.c, 'b', test + st + "expected character");
    is(new_cr2.c_class, class_ES, test + st + "expected type");
    is(new_cr2.embed, 58, test + st + "expected embed");
}

void test_rule_x6a(void)
{
    std::string test = "rule_x6a: ", st;

    fake_bidi b;

    st = "nonzero overflow isolate: ";

    b.overflow_isolate = 1;

    b.direction_stack.push({6, fake_bidi::direction_rec::RTL, false});

    fake_bidi::character_rec cr = {'a', class_L, 38};
    fake_bidi::character_rec new_cr = b.rule_x6a(cr);

    is(b.direction_stack.size(), 1, test + st + "no stack pops");
    is(b.overflow_isolate, 0, test + st + "expected isolate overflow");
    is(new_cr.c_class, class_R, test + st + "expected type");
    is(new_cr.embed, 6, test + st + "expected embed");

    st = "nonzero valid isolate: ";

    b.valid_isolate = 1;
    b.overflow_embed = 5;

    b.direction_stack.push({7, fake_bidi::direction_rec::NEUTRAL, true});
    b.direction_stack.push({8, fake_bidi::direction_rec::RTL, false});
    b.direction_stack.push({9, fake_bidi::direction_rec::RTL, false});

    fake_bidi::character_rec new_cr2 = b.rule_x6a(cr);

    is(b.direction_stack.size(), 1, test + st + "stack cleaned up");
    is(b.valid_isolate, 0, test + st + "expected valid isolate");
    is(new_cr2.c_class, class_R, test + st + "expected type");
    is(new_cr2.embed, 6, test + st + "expected embed");

    st = "no isolate: ";

    fake_bidi::character_rec new_cr3 = b.rule_x6a(cr);

    is(b.direction_stack.size(), 1, test + st + "no stack operations");
    is(new_cr3.c_class, class_R, test + st + "expected type");
    is(new_cr3.embed, 6, test + st + "expected embed");
}

void test_rule_x7(void)
{
    std::string test = "rule_x7: ", st;

    fake_bidi b;

    st = "nonzero overflow isolate: ";

    b.overflow_isolate = 1;

    fake_bidi::character_rec cr = {'a', class_L, 38};
    fake_bidi::character_rec new_cr = b.rule_x7(cr);

    is(b.overflow_isolate, 1, test + st + "no overflow isolate change");
    is(new_cr.c_class, class_L, test + st + "expected type");
    is(new_cr.embed, 38, test + st + "expected embed");

    st = "nonzero overflow embed: ";

    b.overflow_isolate = 0;
    b.overflow_embed = 1;

    b.direction_stack.push({6, fake_bidi::direction_rec::RTL, false});

    fake_bidi::character_rec new_cr2 = b.rule_x7(cr);

    is(b.overflow_embed, 0, test + st + "expected overflow embed");
    is(new_cr2.embed, 6, test + st + "expected embed");

    st = "direction stack too small: ";

    b.direction_stack.pop();
    b.direction_stack.push({48, fake_bidi::direction_rec::RTL, false});

    fake_bidi::character_rec new_cr3 = b.rule_x7(cr);

    is(b.direction_stack.size(), 1, test + st + "expected stack size");
    is(new_cr3.embed, 48, test + st + "expected embed");

    st = "direction stack top wrong: ";

    b.direction_stack.push({19, fake_bidi::direction_rec::RTL, true});

    fake_bidi::character_rec new_cr4 = b.rule_x7(cr);

    is(b.direction_stack.size(), 2, test + st + "expected stack size");
    is(new_cr4.embed, 19, test + st + "expected embed");

    st = "good stack state: ";

    b.direction_stack.push({66, fake_bidi::direction_rec::RTL, false});

    fake_bidi::character_rec new_cr5 = b.rule_x7(cr);

    is(b.direction_stack.size(), 2, test + st + "expected stack size");
    is(new_cr5.embed, 19, test + st + "expected embed");
}

void test_rule_x8(void)
{
    std::string test = "rule_x8: ";

    fake_bidi b;

    b.overflow_isolate = 38;
    b.overflow_embed = 16;
    b.valid_isolate = 185;

    b.direction_stack.push({7, fake_bidi::direction_rec::NEUTRAL, true});
    b.direction_stack.push({8, fake_bidi::direction_rec::RTL, false});
    b.direction_stack.push({9, fake_bidi::direction_rec::RTL, false});

    fake_bidi::character_rec cr = {'a', class_L, 38};
    fake_bidi::character_rec new_cr = b.rule_x8(cr);

    is(b.direction_stack.size(), 1, test + "expected stack size");
    is(b.overflow_isolate, 0, test + "expected overflow isolate");
    is(b.overflow_embed, 0, test + "expected overflow embed");
    is(b.valid_isolate, 0, test + "expected valid isolate");
    is(new_cr.embed, 7, test + "expected embed");
}

int main(int argc, char **argv)
{
    plan(251);

    test_create_delete();
    test_char_type();
    test_reset_direction_class();
    test_rule_p1();
    test_rule_p2_p3();
    test_rule_x1();
    test_rule_x2();
    test_rule_x3();
    test_rule_x4();
    test_rule_x5();
    test_rule_x5a();
    test_rule_x5b();
    test_rule_x5c();
    test_rule_x6a();
    test_rule_x7();
    test_rule_x8();
    return exit_status();
}
