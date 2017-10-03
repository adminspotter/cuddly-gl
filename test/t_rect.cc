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

void test_get_size(void)
{
    std::string test = "get_size: ", st;
    test_rect *r = NULL;
    GLuint dim;
    glm::ivec2 sz;
    int ret;

    try
    {
        r = new test_rect(9, 87);
    }
    catch (...)
    {
        fail(test + "constructor exception");
        return;
    }

    st = "get_size single: ";
    ret = r->get_size(ui::size::width, &dim);
    is(ret, 0, test + st + "expected return");
    is(dim, 9, test + st + "expected width");

    st = "get_size vector: ";
    ret = r->get_size(ui::size::all, &sz);
    is(ret, 0, test + st + "expected return");
    is(sz.x, 9, test + st + "expected x-dim");
    is(sz.y, 87, test + st + "expected y-dim");

    st = "get_size bad element subtype: ";
    dim = 9876;
    ret = r->get_size(-1, &dim);
    is(ret, 1, test + st + "expected return");
    is(dim, 9876, test + st + "value unchanged");

    st = "get single: ";
    ret = r->get(ui::element::size, ui::size::height, &dim);
    is(ret, 0, test + st + "expected return");
    is(dim, 87, test + st + "expected height");

    st = "get bad element type: ";
    dim = 9876;
    ret = r->get(-1, ui::size::width, &dim);
    is(ret, 1, test + st + "expected return");
    is(dim, 9876, test + st + "value unchanged");

    try
    {
        delete r;
    }
    catch (...)
    {
        fail(test + "destructor exception");
    }
}

void test_set_size(void)
{
    std::string test = "set_size: ", st;
    test_rect *r = NULL;
    GLuint dim = 42;
    glm::ivec2 sz = {6, 5};

    try
    {
        r = new test_rect(9, 87);
    }
    catch (...)
    {
        fail(test + "constructor exception");
        return;
    }

    st = "set_size single: ";
    r->set_size(ui::size::height, &dim);
    is(r->dim.y, 42, test + st + "expected height");

    st = "set_size vector: ";
    r->set_size(ui::size::all, &sz);
    is(r->dim.x, 6, test + st + "expected x-dim");
    is(r->dim.y, 5, test + st + "expected y-dim");

    st = "set_size bad element subtype: ";
    dim = 9876;
    r->set_size(-1, &dim);
    is(r->dim.x, 6, test + st + "x-dim unchanged");
    is(r->dim.y, 5, test + st + "y-dim unchanged");

    st = "set single: ";
    dim = 42;
    r->set(ui::element::size, ui::size::width, &dim);
    is(r->dim.x, 42, test + st + "expected width");

    st = "set bad element type: ";
    dim = 9876;
    r->set(-1, ui::size::width, &dim);
    isnt(r->dim.x, 9876, test + st + "width unchanged");

    try
    {
        delete r;
    }
    catch (...)
    {
        fail(test + "destructor exception");
    }
}

void test_va_get(void)
{
    std::string test = "va_get: ", st;
    test_rect *r = NULL;
    GLuint dim = 0;
    glm::ivec2 sz = {6, 5};

    try
    {
        r = new test_rect(9, 87);
    }
    catch (...)
    {
        fail(test + "constructor exception");
        return;
    }

    st = "single: ";
    r->get_va(ui::element::size, ui::size::width, &dim,
              ui::element::size, ui::size::height, &dim, 0);
    is(dim, 87, test + st + "expected size");

    st = "vector: ";
    r->get_va(ui::element::size, ui::size::all, &sz, 0);
    is(sz.x, 9, test + st + "expected width");
    is(sz.y, 87, test + st + "expected height");

    st = "bad element type: ";
    dim = 4;
    r->get_va(99999, ui::size::width, &dim, 0);
    is(dim, 4, test + st + "value unchanged");

    st = "bad element subtype: ";
    r->get_va(ui::element::size, 99999, &dim, 0);
    is(dim, 4, test + st + "value unchanged");

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
    plan(25);

    test_create_delete();
    test_get_size();
    test_set_size();
    test_va_get();
    return 0;
}
