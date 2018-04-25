#include <tap++.h>

using namespace TAP;

#include "../bidi.h"
#include "../font.h"

void test_bidi_char_type(void)
{
    std::string test = "bidi_char_type: ";

    is(bidi_char_type(0x2066), class_LRI, test + "expected LRI");
    is(bidi_char_type(0x2067), class_RLI, test + "expected RLI");
    is(bidi_char_type(0x2068), class_FSI, test + "expected FSI");
    is(bidi_char_type(0x2069), class_PDI, test + "expected PDI");
    is(bidi_char_type(0x061c), class_AL, test + "expected ALM AL");
    is(bidi_char_type(0x0627), class_AL, test + "expected char AL");
    is(bidi_char_type(0x200f), class_R, test + "expected RLM R");
    is(bidi_char_type(0x05d0), class_R, test + "expected char R");
    is(bidi_char_type(0x0085), class_B, test + "expected B");
    is(bidi_char_type(0x200e), class_L, test + "expected LRM L");
    is(bidi_char_type(0x0061), class_L, test + "expected char L");
}

void test_bidi_p1(void)
{
    std::string test = "bidi_p1: ", st;

    st = "no newlines: ";

    std::u32string no_newlines = ui::utf8tou32str("abcde12345");

    std::vector<std::u32string> res1 = bidi_p1(no_newlines);

    is(res1.size(), 1, test + st + "expected vector size");
    is(ui::u32strtoutf8(res1[0]), "abcde12345",
       test + st + "expected string");

    st = "newline at beginning: ";

    std::u32string leading_lf = ui::utf8tou32str("\nabc123");

    std::vector<std::u32string> res2 = bidi_p1(leading_lf);

    is(res2.size(), 2, test + st + "expected vector size");
    is(ui::u32strtoutf8(res2[0]), "", test + st + "expected string 1");
    is(ui::u32strtoutf8(res2[1]), "abc123", test + st + "expected string 2");

    st = "newline at end: ";

    std::u32string trailing_lf = ui::utf8tou32str("abc123\n");

    std::vector<std::u32string> res3 = bidi_p1(trailing_lf);

    is(res3.size(), 2, test + st + "expected vector size");
    is(ui::u32strtoutf8(res3[0]), "abc123", test + st + "expected string 1");
    is(ui::u32strtoutf8(res3[1]), "", test + st + "expected string 2");

    st = "newline in middle: ";

    std::u32string middle_lf = ui::utf8tou32str("abc\n123");

    std::vector<std::u32string> res4 = bidi_p1(middle_lf);

    is(res4.size(), 2, test + st + "expected vector size");
    is(ui::u32strtoutf8(res4[0]), "abc", test + st + "expected string 1");
    is(ui::u32strtoutf8(res4[1]), "123", test + st + "expected string 2");

    st = "crlf: ";

    std::u32string crlf = { 'a', 'b', 'c', 0x0d, 0x0a, '1', '2', '3' };

    std::vector<std::u32string> res5 = bidi_p1(crlf);

    is(res5.size(), 2, test + st + "expected vector size");
    is(ui::u32strtoutf8(res5[0]), "abc", test + st + "expected string 1");
    is(ui::u32strtoutf8(res5[1]), "123", test + st + "expected string 2");

    st = "paragraph separator: ";

    std::u32string para_sep = { 'a', 'b', 'c', 0x2029, '1', '2', '3' };

    std::vector<std::u32string> res6 = bidi_p1(para_sep);

    is(res6.size(), 2, test + st + "expected vector size");
    is(ui::u32strtoutf8(res6[0]), "abc", test + st + "expected string 1");
    is(ui::u32strtoutf8(res6[1]), "123", test + st + "expected string 2");
}

void test_bidi_p2_p3(void)
{
    std::string test = "bidi_p2_p3: ";

    std::u32string empty;
    is(bidi_p2_p3(empty), 0, test + "empty: expected embedding");

    std::u32string l_first = { 'a' };
    is(bidi_p2_p3(l_first), 0, test + "L first: expected embedding");

    std::u32string r_first = { 0x5d0 };
    is(bidi_p2_p3(r_first), 1, test + "R first: expected embedding");

    std::u32string al_first = { 0x627 };
    is(bidi_p2_p3(al_first), 1, test + "AL first: expected embedding");

    std::u32string l_before_r = { 'a', 0x5d0 };
    is(bidi_p2_p3(l_before_r), 0, test + "L before R: expected embedding");

    std::u32string l_before_al = { 'a', 0x627 };
    is(bidi_p2_p3(l_before_al), 0, test + "L before AL: expected embedding");

    std::u32string neutral_l = { '-', 'a' };
    is(bidi_p2_p3(neutral_l), 0,
       test + "L after neutral: expected embedding");

    std::u32string neutral_r = { '-', 0x5d0 };
    is(bidi_p2_p3(neutral_r), 1,
       test + "R after neutral: expected embedding");

    std::u32string neutral_al = { '-', 0x627 };
    is(bidi_p2_p3(neutral_al), 1,
       test + "AL after neutral: expected embedding");

    std::u32string all_isolate = { 0x2066, 0x5d0, 0x2069 };
    is(bidi_p2_p3(all_isolate), 0,
       test + "all isolate: expected embedding");

    std::u32string isolate_l = { 0x2066, 0x5d0, 0x2069, 'a' };
    is(bidi_p2_p3(isolate_l), 0,
       test + "L after isolate: expected embedding");

    std::u32string isolate_r = { 0x2066, 'a', 0x2069, 0x5d0 };
    is(bidi_p2_p3(isolate_r), 1,
       test + "R after isolate: expected embedding");

    std::u32string isolate_al = { 0x2066, 'a', 0x2069, 0x627 };
    is(bidi_p2_p3(isolate_al), 1,
       test + "AL after isolate: expected embedding");

    std::u32string nested =
        { 0x2066, 'a', 0x2067, 'b', 0x2069, 0x2069, 0x627 };
    is(bidi_p2_p3(nested), 1,
       test + "nested isolates: expected embedding");
}

int main(int argc, char **argv)
{
    plan(42);

    test_bidi_char_type();
    test_bidi_p1();
    test_bidi_p2_p3();
    return exit_status();
}
