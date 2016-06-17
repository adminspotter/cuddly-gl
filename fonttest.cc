#include <iostream>

#include "font.h"

std::string font_name = "Times New Roman.ttf";
std::vector<std::string> paths =
{
    "~/Library/Fonts",
    "/Library/Fonts",
    "/Network/Library/Fonts",
    "/System/Library/Fonts",
    "/System/Folder/Fonts",
};

int main(int argc, char **argv)
{
    int i, j;
    unsigned int w, h;
    Font f(font_name, 20, paths);
    std::string s("Howdy");
    std::u32string str(s.begin(), s.end());
    unsigned char *buf = f.render_string(str, w, h);

    for (i = h - 1; i >= 0; --i)
    {
        for (j = 0; j < w; ++j)
        {
            unsigned char c = buf[i * w + j];
            if (c & 0xc0)
                std::cout << "▓";
            else if (c & 0x30)
                std::cout << "▒";
            else if (c & 0x0c)
                std::cout << "░";
            else
                std::cout << ' ';
        }
        std::cout << '|' << std::endl;
    }
}
