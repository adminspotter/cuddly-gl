#include <tap++.h>

using namespace TAP;

#include "../ui_defs.h"
#include "../active.h"

class test_active : public ui::active
{
  public:
    using ui::rect::dim;
    using ui::active::enter_cb;

    test_active(GLuint w, GLuint h) : ui::rect(w, h), ui::active(w, h) {};
    virtual ~test_active() {};
};

int callback_calls = 0;

void fake_callback(ui::active *a, void *call, void *client)
{
    ++callback_calls;
}

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

void test_callback(void)
{
    std::string test = "callback: ", st;
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

    st = "add: ";
    a->add_callback(ui::callback::enter, fake_callback, NULL);
    is(a->enter_cb.size(), 1, test + st + "expected list size");
    a->add_callback(ui::callback::enter, fake_callback, (void *)1);
    is(a->enter_cb.size(), 2, test + st + "expected larger size");

    st = "call: ";
    a->call_callbacks(ui::callback::enter, NULL);
    is(callback_calls, 2, test + st + "expected call count");

    st = "remove: ";
    a->remove_callback(ui::callback::enter, (ui::cb_fptr)1, (void *)1);
    is(a->enter_cb.size(), 2,
       test + st + "bad funcptr doesn't remove anything");
    a->remove_callback(ui::callback::enter, fake_callback, (void *)2);
    is(a->enter_cb.size(), 2,
       test + st + "bad client data doesn't remove anything");
    a->remove_callback(ui::callback::enter, fake_callback, (void *)1);
    is(a->enter_cb.size(), 1, test + st + "good element is removed");

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
    plan(9);

    test_create_delete();
    test_callback();
    return exit_status();
}
