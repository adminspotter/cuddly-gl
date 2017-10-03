#include <tap++.h>

using namespace TAP;

#include "../ui_defs.h"
#include "../active.h"

class test_active : public ui::active
{
  public:
    using ui::rect::dim;
    using ui::active::enter_cb;
    using ui::active::which_cb_list;

    test_active(GLuint w, GLuint h) : ui::rect(w, h), ui::active(w, h) {};
    virtual ~test_active() {};
};

void test_create_delete(void)
{
    std::string test = "create/delete: ";
    test_active *a = NULL;

    try
    {
        a = new test_active(9, 87);
    }
    catch (...)
    {
        fail(test + "constructor exception");
        return;
    }

    is(a->dim.x, 9, test + "expected width");
    is(a->dim.y, 87, test + "expected height");
    is(a->enter_cb.size(), 0, test + "empty callback list");

    try
    {
        delete a;
    }
    catch (...)
    {
        fail(test + "destructor exception");
    }
}

int main(int argc, char **argv)
{
    plan(3);

    test_create_delete();
    return 0;
}
