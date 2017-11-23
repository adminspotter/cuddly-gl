#include <iostream>
#include <tuple>
#include <functional>

#include "../font.h"

std::vector<std::string> font_names =
{
    "techover.ttf",
    "nehama.ttf"
};
std::vector<std::string> paths =
{
    ".",
    "./test"
};

int main(int argc, char **argv)
{
    int i, j;
    ui::font_set f(font_names[0]);
    std::string s("Howdy שלום");
    std::u32string str(ui::utf8tou32str(s));
    ui::image img;

    for (auto i : font_names)
    {
        auto f1 = std::make_tuple(std::ref(i), 20, std::ref(paths));
        f << f1;
    }

    f.render_string(str, img);

    for (i = img.height - 1; i >= 0; --i)
    {
        for (j = 0; j < img.width; ++j)
        {
            unsigned char c = img.data[i * img.width + j];
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
