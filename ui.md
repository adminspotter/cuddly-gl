# r9 User Interface library #

We evaluated a number of other UI toolkits, and found that all of them
had limitations that didn't work for what we wanted:  they required a
specific UI library that we weren't using; they had an inconsistent
call structure; or they just didn't work like we wanted them to.

We wanted a lightweight, C++-specific, OpenGL-only toolkit, that made
sense to call, and was internally consistent.  We're Xt programmers
from way back, and despite Xt being a C library, it is still largely
object-oriented, so that model really seemed to fit our requirements.
Callbacks, consistent create/get/set, consistent set of resources, and
so on.

All classes and constants live within the `ui::` namespace.  It's
short, and yet still descriptive.

Our interfaces use pixel dimensions and positions.  (0, 0) is in the
upper left of our window.  OpenGL screen coordinates, which fall into
floating-point [-1.0, 1.0] ranges in both dimensions, are only used
internally when generating vertex buffers and the like, so any
interaction with the UI widget set will be in pixels.

### Creation ###

All our widgets take the same thing to create:

* parent (`ui::composite *`)
* width (`GLuint`)
* height (`GLuint`)

Examples:
```c++
ui::panel *p = new ui::panel(parent, 200, 100);
ui::button *b = new ui::button(NULL, 60, 30);
```
