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

### Getting and setting resources ###

Some widgets need a bit more configuration than just size.  All
widgets have a set of resources that can be configured however
desired.  We can `get` or `set` any of them at any time, and the call
interface for both is identical:

* element name (`GLuint`)  
  This will be a constant from the `ui::element` namespace.
* type (`GLuint`)  
  Most types have their own `ui::` namespaces for subtypes.  Some
  items don't really have a subtype, so this value doesn't matter.
* value (`void *`)  
  Always a void pointer, which we cast internally to what it needs to
  be.

Examples:
```c++
ui::panel *p = new ui::panel(parent, 200, 100);
int w = 300;
p->set(ui::element::size, ui::size::width, &w);

w = 937;
p->get(ui::element::size, ui::size::width, &w);
/* w is now 300 */

ui::button *b = new ui::panel(parent, 10, 10);
std::string some_string = "Howdy!";
b->set(ui::element::string, 0, &some_string);

some_string = "Buh-bye.";
b->get(ui::element::string, 0, &some_string);
/* some_string is now "Howdy!" */
```

There are also vararg-type get and set functions for each object,
`get_va` and `set_va`.  Syntax is mostly the same, sets of
element/type/value, and ends with an extra 0.

Examples:
```c++
ui::panel *p = new ui::panel(parent, 100, 100);
int margin = 5, border = 1;
p->set_va(ui::element::margin, ui::side::all, &margin,
          ui::element::border, ui::side::top, &border, 0);

margin = 123;
border = 456;
p->get_va(ui::element::margin, ui::side::right, &margin,
          ui::element::border, ui::side::bottom, &border, 0);
/* margin is now 5, and border is now 0 */
```
