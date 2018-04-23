#include <tap++.h>

using namespace TAP;

#include "../bidi.h"
#include "../font.h"

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
}

int main(int argc, char **argv)
{
    plan(18);

    test_bidi_p1();
    test_bidi_p2_p3();
    return exit_status();
}
