#include <tap++.h>

using namespace TAP;

#include "../active.h"

class test_active : public ui::active
{
  public:
    using ui::rect::dim;
    using ui::active::enter_cb;
    using ui::active::leave_cb;
    using ui::active::motion_cb;
    using ui::active::btn_down_cb;
    using ui::active::btn_up_cb;
    using ui::active::key_down_cb;
    using ui::active::key_up_cb;
    using ui::active::resize_cb;
    using ui::active::focus_cb;
    using ui::active::visible_cb;
    using ui::active::timeout_func;
    using ui::active::timeout_arg;

    test_active(GLuint w, GLuint h) : ui::rect(w, h), ui::active(w, h) {};
    virtual ~test_active() {};
};

int callback_calls = 0;

void fake_mouse_callback(ui::active *a, ui::mouse_call_data& mcd, void *client)
{
    ++callback_calls;
}

void fake_btn_callback(ui::active *a, ui::btn_call_data& bcd, void *client)
{
    ++callback_calls;
}

void fake_key_callback(ui::active *a, ui::key_call_data& kcd, void *client)
{
    ++callback_calls;
}

void fake_resize_callback(ui::active *a,
                          ui::resize_call_data& rcd,
                          void *client)
{
    ++callback_calls;
}

void fake_focus_callback(ui::active *a, ui::focus_call_data& fcd, void *client)
{
    ++callback_calls;
}

void fake_visible_callback(ui::active *a,
                           ui::visible_call_data& vcd,
                           void *client)
{
    ++callback_calls;
}

void unused_callback(ui::active *a, ui::mouse_call_data& mcd, void *client)
{
}

int timeout_calls = 0;

void fake_timeout(ui::active *a, void *client)
{
    ++timeout_calls;
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
    ui::mouse_call_data mcd;

    try
    {
        a = new test_active(9, 87);
    }
    catch (...)
    {
        fail(test + "constructor exception");
        return;
    }

    st = "call empty: ";
    a->call_callbacks(ui::callback::enter, mcd);
    is(callback_calls, 0, test + st + "didn't call anything");

    st = "add: ";
    a->add_callback(ui::callback::enter, fake_mouse_callback, NULL);
    is(a->enter_cb.size(), 1, test + st + "expected list size");
    a->add_callback(ui::callback::enter, fake_mouse_callback, (void *)1);
    is(a->enter_cb.size(), 2, test + st + "expected larger size");

    st = "call: ";
    a->call_callbacks(ui::callback::enter, mcd);
    is(callback_calls, 2, test + st + "expected call count");

    st = "remove: ";
    a->remove_callback(ui::callback::enter, unused_callback, (void *)1);
    is(a->enter_cb.size(), 2,
       test + st + "bad funcptr doesn't remove anything");
    a->remove_callback(ui::callback::enter, fake_mouse_callback, (void *)2);
    is(a->enter_cb.size(), 2,
       test + st + "bad client data doesn't remove anything");
    a->remove_callback(ui::callback::enter, fake_mouse_callback, (void *)1);
    is(a->enter_cb.size(), 1, test + st + "good element is removed");

    st = "wrong type: ";
    try
    {
        a->add_callback(ui::callback::btn_down, fake_mouse_callback, NULL);
        fail(test + st + "missing add exception");
    }
    catch (std::runtime_error& e)
    {
        pass(test + st + "expected add exception");
    }
    try
    {
        a->call_callbacks(ui::callback::btn_down, mcd);
        fail(test + st + "missing call exception");
    }
    catch (std::runtime_error& e)
    {
        pass(test + st + "expected call exception");
    }
    try
    {
        a->remove_callback(ui::callback::btn_down, fake_mouse_callback, NULL);
        fail(test + st + "missing remove exception");
    }
    catch (std::runtime_error& e)
    {
        pass(test + st + "expected remove exception");
    }

    try
    {
        delete a;
    }
    catch (...)
    {
        fail(test + "destructor exception");
    }
}

void test_which_callback(void)
{
    std::string test = "which callback: ";
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

    a->add_callback(ui::callback::enter, fake_mouse_callback, NULL);
    is(a->enter_cb.size(), 1, test + "picked enter");

    a->add_callback(ui::callback::leave, fake_mouse_callback, NULL);
    is(a->leave_cb.size(), 1, test + "picked leave");

    a->add_callback(ui::callback::motion, fake_mouse_callback, NULL);
    is(a->motion_cb.size(), 1, test + "picked motion");

    a->add_callback(ui::callback::btn_down, fake_btn_callback, NULL);
    is(a->btn_down_cb.size(), 1, test + "picked btn_down");

    a->add_callback(ui::callback::btn_up, fake_btn_callback, NULL);
    is(a->btn_up_cb.size(), 1, test + "picked btn_up");

    a->add_callback(ui::callback::key_down, fake_key_callback, NULL);
    is(a->key_down_cb.size(), 1, test + "picked key_down");

    a->add_callback(ui::callback::key_up, fake_key_callback, NULL);
    is(a->key_up_cb.size(), 1, test + "picked key_up");

    a->add_callback(ui::callback::resize, fake_resize_callback, NULL);
    is(a->resize_cb.size(), 1, test + "picked resize");

    a->add_callback(ui::callback::focus, fake_focus_callback, NULL);
    is(a->focus_cb.size(), 1, test + "picked focus");

    a->add_callback(ui::callback::visible, fake_visible_callback, NULL);
    is(a->visible_cb.size(), 1, test + "picked visible");

    try
    {
        a->add_callback(99999, fake_mouse_callback, NULL);
        fail(test + "used a default callback list");
    }
    catch (const std::invalid_argument& e)
    {
        pass(test + "didn't use a default callback list");
    }

    try
    {
        delete a;
    }
    catch (...)
    {
        fail(test + "destructor exception");
    }
}

void test_timeout(void)
{
    std::string test = "timeout: ", st;
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

    st = "call empty: ";
    a->call_timeout();
    is(timeout_calls, 0, test + st + "didn't call anything");

    st = "add: ";
    a->add_timeout(ui::to_until::zero(), fake_timeout, (void *)1);
    is(a->timeout_func, fake_timeout, test + st + "expected timeout func");
    is(a->timeout_arg, (void *)1, test + st + "expected timeout arg");

    st = "remove: ";
    a->remove_timeout();
    is(a->timeout_func, (ui::to_fptr)NULL, test + st + "timeout func unset");
    is(a->timeout_arg, (void *)NULL, test + st + "timeout arg unset");

    st = "call: ";
    a->add_timeout(ui::to_until::zero(), fake_timeout, (void *)1);
    a->call_timeout();
    is(timeout_calls, 1, test + st + "timeout function called");
    is(a->timeout_func, (ui::to_fptr)NULL, test + st + "timeout func unset");
    is(a->timeout_arg, (void *)NULL, test + st + "timeout arg unset");

    st = "call NULL func: ";
    a->add_timeout(ui::to_until::zero(), (ui::to_fptr)NULL, (void *)1);
    a->call_timeout();
    is(a->timeout_arg, (void *)NULL, test + st + "timeout arg unset");

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
    plan(33);

    test_create_delete();
    test_callback();
    test_which_callback();
    test_timeout();
    return exit_status();
}
