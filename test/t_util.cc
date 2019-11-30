#include <tap++.h>

using namespace TAP;

#include "../util.h"

void test_basic_string(void)
{
    std::string test = "0x00 - 0x7f: ";

    std::u32string str = { 'a', 'b', 'c' };
    std::string converted = ui::u32strtoutf8(str);
    std::u32string result = ui::utf8tou32str(converted);

    is(converted.size(), str.size(), test + "equal string lengths");
    ok(str == result, test + "good round trip");
}

int main(int argc, char **argv)
{
    plan(2);
    test_basic_string();
    return exit_status();
}
