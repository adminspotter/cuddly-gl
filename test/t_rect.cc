#include <tap++.h>

using namespace TAP;

#include "../ui_defs.h"
#include "../rect.h"

class test_rect : public ui::rect
{
  public:
    using ui::rect::dim;
    using ui::rect::get_size;
    using ui::rect::set_size;

    test_rect(GLuint w, GLuint h) : ui::rect(w, h) {};
    virtual ~test_rect() {};
};

int main(int argc, char **argv)
{
    plan(0);

    return 0;
}
