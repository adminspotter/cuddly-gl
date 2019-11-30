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

void test_second_group(void)
{
    std::string test = "0x80 - 0x7ff: ";

    std::u32string str = { 0x80, 0x100, 0x7ff };
    std::string converted = ui::u32strtoutf8(str);
    std::u32string result = ui::utf8tou32str(converted);

    isnt(converted.size(), str.size(), test + "unequal string lengths");
    is(converted.size(), 6, test + "expected UTF-8 string length");
    ok(str == result, test + "good round trip");
}

void test_third_group(void)
{
    std::string test = "0x800 - 0xffff: ";

    std::u32string str = { 0x800, 0x1000, 0xffff };
    std::string converted = ui::u32strtoutf8(str);
    std::u32string result = ui::utf8tou32str(converted);

    isnt(converted.size(), str.size(), test + "unequal string lengths");
    is(converted.size(), 9, test + "expected UTF-8 string length");
    ok(str == result, test + "good round trip");
}

void test_fourth_group(void)
{
    std::string test = "0x10000 - 0x1fffff: ";

    std::u32string str = { 0x10000, 0x80000, 0x1fffff };
    std::string converted = ui::u32strtoutf8(str);
    std::u32string result = ui::utf8tou32str(converted);

    isnt(converted.size(), str.size(), test + "unequal string lengths");
    is(converted.size(), 12, test + "expected UTF-8 string length");
    ok(str == result, test + "good round trip");
}

void test_fifth_group(void)
{
    std::string test = "0x200000 - 0x3ffffff: ";

    std::u32string str = { 0x200000, 0xffffff, 0x3ffffff };
    std::string converted = ui::u32strtoutf8(str);
    std::u32string result = ui::utf8tou32str(converted);

    isnt(converted.size(), str.size(), test + "unequal string lengths");
    is(converted.size(), 15, test + "expected UTF-8 string length");
    ok(str == result, test + "good round trip");
}

void test_sixth_group(void)
{
    std::string test = "0x4000000 - 0x7fffffff ";

    std::u32string str = { 0x4000000, 0x4ffffff, 0x7fffffff };
    std::string converted = ui::u32strtoutf8(str);
    std::u32string result = ui::utf8tou32str(converted);

    isnt(converted.size(), str.size(), test + "unequal string lengths");
    is(converted.size(), 18, test + "expected UTF-8 string length");
    ok(str == result, test + "good round trip");
}

int main(int argc, char **argv)
{
    plan(17);

    test_basic_string();
    test_second_group();
    test_third_group();
    test_fourth_group();
    test_fifth_group();
    test_sixth_group();
    return exit_status();
}
