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

int main(int argc, char **argv)
{
    plan(0);

    return 0;
}
