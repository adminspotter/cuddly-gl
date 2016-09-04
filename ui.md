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

### Callbacks ###

Events are propagated through the widget set by callbacks.  Each type
of callback has a list of zero or more functions that it will call for
that event.  Currently the events that our toolkit handles are mouse
motion, enter, leave, mouse button down, mouse button up, key down,
and key up.  They, as with the get and set, have a consistent
interfaces.  A callback routine has a standard signature:

* widget (`ui::event_target *`)  
  This is the widget which is processing the event.
* call data (`void *`)  
  This is (usually) an event-dependent structure, detailed below.
* client data (`void *`)  
  This is any sort of data that the client may wish to pass into
  the callback, and is completely implementation-defined.

The `cb_fptr` type, defined in
[callback.h](../blob/client/ui/callback.h), defines the required
signature for a callback routine.

Adding to and removing from a callback list also has a consistent
interface:

* list name (`GLuint`)  
  This is a constant from the `ui::callback` namespace
* function name (`cb_fptr`)  
  The name of the function.  Any class methods must be static.
* client data (`void *`)  
  Any free-form data that the callback will expect to receive.

Examples:
```c++
void some_callback(ui::event_target *t, void *call, void *client)
{
    /* ... */
}

stuct something
{
    /* ... */
}
some_struct;

ui::button *b = new ui::button(parent, 100, 50);
b->add_callback(ui::callback::btn_down, some_callback, NULL);
b->add_callback(ui::callback::btn_down, some_callback, &some_struct);

/* The mouse button down callback list has two elements */

b->remove_callback(ui::callback::btn_down, some_callback, &some_struct);

/* Now the mouse button down callback list has one element */
```

In almost all cases, the toolkit handles event propagation and calling
the appropriate callback lists for us, but there may be situations in
which we'd like to call a callback list on our own.  Calling callback
lists has a consistent interface:

* list name (`GLuint`)  
  As above, a constant from the `ui::callback` namespace.
* call data (`void *`)  
  A call defined structure; internal calls will contain the relevant
  struct defined in [ui_defs.h](../blob/client/ui/ui_defs.h).

```c++
/* Continuing from the add/remove example above */

b->call_callbacks(ui::callback::btn_down, NULL);

/* some_callback will be called with both call and client data NULL */
```

Please note that the client data pointer is saved in the callback list
when it is added, so if the structure or variable that the client data
points to changes between the time when the callback is added to the
list, and the time it is called, whatever is pointed to at the time of
the call will be what is received by the callback routine.  Typically,
these will be file-static or global variables, or objects or class
members, so that their scope does not impact any calls the callback
routines might see.  For an example:

```c++
ui::button *b = new ui::button(parent, 100, 50);

int data = 123;
b->add_callback(ui::callback::btn_down, some_callback, &data);

/* ... */

data = 456;

/* ... */

b->call_callbacks(ui::callback::btn_down, NULL);

/* The some_callback routine will receive '456' as its client data */
```

Removing from a callback list does not depend on what the client data
pointer points to, but simply the value of the pointer, i.e. the
address that it points to.  It is used as a secondary key, because we
may use the same callback routine multiple times in a single callback
list, differing only in the client data we pass.

The `ui::event_target` class, which is used by all types of widgets,
adds callback lists and handling.  The full implementation is found in
[callback.h](../blob/client/ui/callback.h) and
[callback.cc](../blob/client/ui/callback.cc).

## Widgets ##

There are a couple classes of widgets.  The composite-derived widgets
may have children, and are mostly considered manager-type widgets.
The panel-derived widgets are most of the "regular" things:  buttons,
text fields, etc.

### Panel-derived widgets ###

<a name="ui_panel"></a>The `ui::panel` class
([panel.h](../blob/client/ui/panel.h) and
[panel.cc](../blob/client/ui/panel.cc)) isn't really a functional
widget on its own, but provides a basic set of pieces for other
widgets:  border, margin, size, position, color.  It also handles
general setup and cleanup of the OpenGL VAO and VBO for each widget.

The panel is also derived from `ui::event_target`, so the standard
callback handling is available.

##### Resources #####

* `ui::element::position`
  * `ui::position::x` (`int`)
  * `ui::position::y` (`int`)
  * `ui::position::all` (`glm::ivec2`)
* `ui::element::size`
  * `ui::size::width` (`int`)
  * `ui::size::height` (`int`)
  * `ui::size::all` (`glm::ivec2`)
* `ui::element::border`
  * `ui::side::top` (`GLuint`)
  * `ui::side::bottom` (`GLuint`)
  * `ui::side::left` (`GLuint`)
  * `ui::side::right` (`GLuint`)
  * `ui::side::all` (`GLuint`)
* `ui::element::margin`
  * `ui::side::top` (`GLuint`)
  * `ui::side::bottom` (`GLuint`)
  * `ui::side::left` (`GLuint`)
  * `ui::side::right` (`GLuint`)
  * `ui::side::all` (`GLuint`)
* `ui::element::color`
  * `ui::color::foreground` (`glm::vec4`)
  * `ui::color::background` (`glm::vec4`)

#### Label ####

<a name="ui_label"></a>The `ui::label` class
([label.h](../blob/client/ui/label.h) and
[label.cc](../blob/client/ui/label.cc)) is a descendent of the
[`ui::panel`](#ui_panel) widget class, and adds either a string or an
image.  The string and image are mutually exclusive; setting an image
removes any configured string, and setting a string removes any
configured image.

Input strings are `std::string`, and should be encoded UTF-8.  Output
strings will always be UTF-8 `std::string`.  Internally, the string is
converted into `std::u32string`.  Font handling is done via the
[`ui::font`](#ui_font) support type.

Images use the [`ui::image`](#ui_image) support type.

##### Resources #####

* `ui::element::font`
  * No subtypes ([`ui::font`](#ui_font))
* `ui::element::string`
  * No subtypes (`std::string`)
* `ui::element::bgimage`
  * No subtypes ([`ui::image`](#ui_image))

###### Inherited resources ######

* `ui::element::position` ([`ui::panel`](#ui_panel))
* `ui::element::border` (`ui::panel`)
* `ui::element::margin` (`ui::panel`)
* `ui::element::color` (`ui::panel`)

#### Button ####
#### Text Field ####
#### Password ####

### Composite-derived widgets ###

#### Context ####
#### Manager ####
#### Popup Menu ####
#### Row Column ####

### Support classes ###

#### Font ####
#### Image ####
#### Quadtree ####
