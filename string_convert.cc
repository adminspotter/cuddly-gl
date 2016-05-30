/* We need to be able to convert from UTF-8 representation to
 * actual Unicode code points and back.  All network traffic
 * should be in UTF-8, and we'll of course need to display things
 * in whatever native font the user needs.
 *
 * Ref: http://www.cprogramming.com/tutorial/unicode.html
 */

#include <string>

std::u32string& utf8tou32str(const std::string& str)
{
    std::string::iterator i = str.begin();
    std::u32string newstr();
    uint32_t ch;

    while (i != str.end())
    {
        if (*i & 0xf0 == 0xf0)
        {
            ch = (*i & 0x07) << 18;
            ch += (*(++i) & 0x3f) << 12;
            ch += (*(++i) & 0x3f) << 6;
            ch += (*(++i) & 0x3f);
        }
        else if (*i & 0xe0 == 0xe0)
        {
            ch = (*i & 0x0f) << 12;
            ch += (*(++i) & 0x3f) << 6;
            ch += (*(++i) & 0x3f);
        }
        else if (*i & 0xc0 == 0xc0)
        {
            ch = (*i & 0x1f) << 6;
            ch += (*(++i) & 0x3f);
        }
        else
            ch = *i;

        newstr.push_back(ch);
        ++i;
    }
    return newstr;
}

std::string& u32strtoutf8(const std::u32string& str)
{
    std::u32string::iterator i = str.begin();
    std::string newstr;

    while (i != str.end())
    {
        if (*i >= 0x10000)
        {
            newstr.push_back(0xf0 & ((*i & 0x1c0000) >> 18));
            newstr.push_back(0x80 & ((*i & 0x3f000) >> 12));
            newstr.push_back(0x80 & ((*i & 0xfc0) >> 6));
            newstr.push_back(0x80 & (*i & 0x3f));
        }
        else if (*i >= 0x800)
        {
            newstr.push_back(0xe0 & ((*i & 0x1f000) >> 12));
            newstr.push_back(0x80 & ((*i & 0xfc0) >> 6));
            newstr.push_back(0x80 & (*i & 0x3f));
        }
        else if (*i >= 0x80)
        {
            newstr.push_back(0xc0 & ((*i & 0x7c0) >> 6));
            newstr.push_back(0x80 & (*i & 0x3f));
        }
        else
        {
            newstr.push_back(*i & 0x7f);
        }
        ++i;
    }

    return newstr;
}
