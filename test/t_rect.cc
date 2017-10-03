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

void test_create_delete(void)
{
    std::string test = "create/delete: ";
    test_rect *r = NULL;

    try
    {
        r = new test_rect(9, 87);
    }
    catch (...)
    {
        fail(test + "constructor exception");
        return;
    }

    is(r->dim.x, 9, test + "expected width");
    is(r->dim.y, 87, test + "expected height");

    try
    {
        delete r;
    }
    catch (...)
    {
        fail(test + "destructor exception");
    }
}

int main(int argc, char **argv)
{
    plan(2);

    test_create_delete();
    return 0;
}
