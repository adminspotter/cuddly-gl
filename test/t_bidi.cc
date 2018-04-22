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

}

int main(int argc, char **argv)
{
    plan(2);

    test_bidi_p1();
    return exit_status();
}
