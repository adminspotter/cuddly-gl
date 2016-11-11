# r9 User Interface library #

We want a lightweight, C++-specific, OpenGL-only toolkit, that makes
sense to call, is internally consistent, and doesn't force us to use
any specific windowing toolkit.  We're Xt programmers from way back,
and despite Xt being a C library, it is still largely object-oriented,
so that model really seemed to fit our requirements.  Callbacks,
consistent create/get/set, consistent set of resources, and so on.

We evaluated a number of other OpenGL-specific UI toolkits, and found
that none of them had all the things that we wanted:  some required a
specific UI library that we weren't using; others had an inconsistent
call structure.  A lot of the advice we saw on various OpenGL
discussion forums was "write your own!  It's not that hard, and it'll
be exactly what you want!" so we did.  "Not that hard" wasn't quite
the whole story, but we're definitely getting exactly what we want.

All classes and constants live within the `ui::` namespace.  It's
short, and yet still descriptive.

Our interfaces use pixel dimensions and positions.  (0, 0) is in the
upper left of our window.  OpenGL screen coordinates, which fall into
floating-point [-1.0, 1.0] ranges in both dimensions, are only used
internally when generating vertex buffers and the like, so any
interaction with the UI widget set will be in pixels.

### Creation ###

All our widgets take the same set of arguments to create:

* parent (`ui::composite *`)
* width (`GLuint`)
* height (`GLuint`)

Examples:
```c++
ui::widget *p = new ui::widget(parent, 200, 100);
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
ui::widget *p = new ui::widget(parent, 200, 100);
int w = 300;
p->set(ui::element::size, ui::size::width, &w);

w = 937;
p->get(ui::element::size, ui::size::width, &w);
/* w is now 300 */

ui::button *b = new ui::button(parent, 10, 10);
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
ui::widget *p = new ui::widget(parent, 100, 100);
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

* widget (`ui::active *`)  
  This is the widget which is processing the event.
* call data (`void *`)  
  This is (usually) an event-dependent structure, detailed below.
* client data (`void *`)  
  This is any sort of data that the client may wish to pass into
  the callback, and is completely implementation-defined.

The `cb_fptr` type, defined in
[active.h](../client/ui/active.h), defines the required
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
void some_callback(ui::active *t, void *call, void *client)
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
  struct defined in [ui_defs.h](../client/ui/ui_defs.h).

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

The `ui::active` class, which is used by all types of widgets, adds
callback lists and handling.  The full implementation is found in
[active.h](../client/ui/active.h) and
[active.cc](../client/ui/active.cc).

### Closing ###

There is a single call to close any widget-derived widget:

```c++
ui::widget *p = new ui::widget(context, 123, 456);

/* ... */

p->close();
```

## Widgets ##

There are a couple classes of widgets.  The composite-derived widgets
may have children, and are mostly considered manager-type widgets.
The widget-derived widgets are most of the "regular" things:  buttons,
text fields, etc.  We also have a couple of base classes which provide
specific behaviours, from which all other widget types inherit.

### Base widget classes ###

There are a couple of basic classes from which all of the toolkit
derives:  `ui::rect` and `ui::active`.  They would never be
instantiated, but serve to add basic functionality in a layered way.

#### Rect ####

The `ui::rect` class ([rect.h](../client/ui/rect.h) and
[rect.cc](../client/ui/rect.cc)) provides a rectangle of a given size.
All the wiget classes in the R9 UI toolkit derive from the `ui::rect`.
It provides the `va_get` and `va_set` methods for all other classes.

##### Rect Resources #####

* `ui::element::size`
  * `ui::size::width` (`int`)
  * `ui::size::height` (`int`)
  * `ui::size::all` (`glm::ivec2`)

#### Active ####

The `ui::active` class ([active.h](../client/ui/active.h) and
[active.cc](../client/ui/active.cc)) is a descendent of the
[`ui::rect`](#rect) class, and adds callback lists and methods to
manage them.  See the [Callbacks](#callbacks) section above for a
complete description of the callback lists and methods.

##### Active resources #####

The `ui::active` class doesn't add any resources which are managed by
the typical `get` and `set` methods.

##### Active inherited resources #####

* `ui::element::size` (`ui::rect`)

### Widget-derived widgets ###

#### Widget ####

The `ui::widget` class ([widget.h](../client/ui/widget.h) and
[widget.cc](../client/ui/widget.cc)) is derived from the
[`ui::active`](#active) widget, and though it is the simplest object
which should be instantiated, it isn't really a functional widget on
its own.  It provides a basic set of pieces for other widgets:
border, margin, position, color.  It also handles general setup and
cleanup of the OpenGL VAO, VBO, and EBO for each widget.

Since the `ui::widget` is derived from `ui::active`, the standard
callback handling is available.

##### Widget Resources #####

* `ui::element::position`
  * `ui::position::x` (`int`)
  * `ui::position::y` (`int`)
  * `ui::position::all` (`glm::ivec2`)
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

The side subtypes for border and margin will function as masks, and
can be combined arbitrarily.

``` c++
GLuint left_and_right = ui::side::left | ui::side::right;
GLuint all_but_top = ui::side::left | ui::side::right | ui::side::bottom;
```

This allows flexibility in margins and borders, in that they do not
all need to be the same.  Each is independent of the others.

##### Widget inherited resources #####

* `ui::element::size` (`ui::rect`)

#### Label ####

The `ui::label` class ([label.h](../client/ui/label.h) and
[label.cc](../client/ui/label.cc)) is a descendent of the
[`ui::widget`](#widget) widget class, and adds either a string or an
image.  The string and image are mutually exclusive; setting an image
removes any configured string, and setting a string removes any
configured image.

Input strings are `std::string`, and should be encoded UTF-8.  Output
strings will always be UTF-8 `std::string`.  Internally, the string is
converted into `std::u32string`.  Font handling is done via the
[`ui::font`](#font) support type.

Images use the [`ui::image`](#image) support type.

##### Label resources #####

* `ui::element::font`
  * `ui::ownership::shared` ([`ui::font`](#font))
  * `ui::ownership::owned` ([`ui::font`](#font))
* `ui::element::string`
  * No subtypes (`std::string`)
* `ui::element::image`
  * No subtypes ([`ui::image`](#image))

The font element's subtypes change only the behaviour at widget close.
A font noted as `owned` will be deleted at widget close; a `shared`
font will not.

###### Label inherited resources ######

* `ui::element::size` ([`ui::rect`](#rect))
* `ui::element::position` ([`ui::widget`](#widget))
* `ui::element::border` (`ui::widget`)
* `ui::element::margin` (`ui::widget`)
* `ui::element::color` (`ui::widget`)

#### Button ####

The `ui::button` class ([button.h](../client/ui/button.h) and
[button.cc](../client/ui/button.cc)) is a descendent of the
[`ui::label`](#label) widget class.  It adds some state resources,
`active` and `armed` and some default callbacks.  When the cursor
enters the button, it becomes active, and when the mouse button is
pressed, the button becomes armed.

##### Resources #####

* `ui::element::state`
  * `ui::element::active` (`bool`)
  * `ui::element::armed` (`bool`)

###### Inherited resources ######

* `ui::element::size` ([`ui::rect`](#rect))
* `ui::element::position` ([`ui::widget`](#widget))
* `ui::element::border` (`ui::widget`)
* `ui::element::margin` (`ui::widget`)
* `ui::element::color` (`ui::widget`)
* `ui::element::font` ([`ui::label`](#label))
* `ui::element::string` (`ui::label`)
* `ui::element::image` (`ui::label`)

#### Text Field ####

The `ui::text_field` class ([text_field.h](../client/ui/text_field.h)
and [text_field.cc](../client/ui/text_field.cc)) is a descendent of
the [`ui::label`](#label) widget class.  It adds the ability to edit
the string.

The text field adds a size subtype of max_width, which indicates the
desired width of the field.  The standard height is 1 character.  The
inherited width and height subtypes can be retrieved, and can also be
set but will be ignored.

##### Resources #####

* `ui::element::cursor`
  * `ui::cursor::position` (`GLuint`)
  * `ui::cursor::blink` (`GLuint`)
* `ui::element::size`
  * `ui::size::width` (`GLuint`)
  * `ui::size::height` (`GLuint`)
  * `ui::size::all` (`GLuint`)
  * `ui::size::max_width` (`GLuint`)

###### Inherited resources ######

* `ui::element::position` ([`ui::widget`](#widget))
* `ui::element::border` (`ui::widget`)
* `ui::element::margin` (`ui::widget`)
* `ui::element::color` (`ui::widget`)
* `ui::element::font` ([`ui::label`](#label))
* `ui::element::string` (`ui::label`)

#### Password ####

The `ui::password` class ([password.h](../client/ui/password.h) and
[password.cc](../client/ui/password.cc)) is a descendent of the
[`ui::text_field`](#text_field) widget class.  It has almost the exact
same behaviour, but displays a series of '*' characters, rather than
the actual string.

##### Resources #####

The password field does not add any new resources.

###### Inherited resources ######

* `ui::element::position` ([`ui::widget`](#widget))
* `ui::element::border` (`ui::widget`)
* `ui::element::margin` (`ui::widget`)
* `ui::element::color` (`ui::widget`)
* `ui::element::font` ([`ui::label`](#label))
* `ui::element::string` (`ui::label`)
* `ui::element::cursor` ([`ui::text_field`](#text_field))
* `ui::element::size` (`ui::text_field`)

### Composite-derived widgets ###

#### Composite ####

The `ui::composite` class ([composite.h](../client/ui/composite.h) and
[composite.cc](../client/ui/composite.cc)) is a descendent of the
[`ui::active`](#active) class, and adds the ability to act as a parent
to other widgets.  It also handles the event propagation through the
toolkit.  It lacks any OpenGL handling, so it would not be
instantiated directly.

##### Composite resources #####

The size and resize resources are read-write, but the pixel_size
elements are read-only.

* `ui::element::pixel_size`
  * `ui::size::width` (`float`)
  * `ui::size::height` (`float`)
  * `ui::size::all` (`glm::vec2`)
* `ui::element::resize`
  * No subtypes, but defined value arguments (`GLuint`)
    * `ui::resize::none`
    * `ui::resize::shrink`
    * `ui::resize::grow`
    * `ui::resize::all`

The resize value arguments function as a mask, so that we can shrink
or grow independently.  `ui::resize::all` is the same as
`ui::resize::shrink | ui::resize::grow`.  Also, each screen dimension
is considered independently of the others, e.g. if a composite widget
needs to shrink in the x dimension and grow in the y dimension, each
will be compared against the resize resource to see if the required
size change should be allowed.

##### Composite inherited resources #####

* `ui::element::size` ([`ui::rect`](#rect))

#### Context ####

The `ui::context` ([ui.h](../client/ui/ui.h) and
[ui.cc](../client/ui/ui.cc)) is a bit of an outlier when compared with
most of the other classes.  It functions as the top-level "window",
and manages the OpenGL shaders which we use to draw our widgets.

The context's constructor also has a different signature, and only
takes width and height.  It has no parent because it is the ultimate
parent; all toolkit elements are owned, directly or indirectly, by the
context.

```c++
ui::context *ctx = new ui::context(800, 600);
```

##### Resources #####

All of the new resources which the context adds are read-only, and are
controlled via the OpenGL rendering program contained in
[ui_vertex.glsl](../client/shaders/ui_vertex.glsl) and
[ui_fragment.glsl](../client/shaders/ui_fragment.glsl).

* `ui::element::attribute`
  * `ui::attribute::position` (`GLuint`)
  * `ui::attribute::normal` (`GLuint`)
  * `ui::attribute::color` (`GLuint`)
  * `ui::attribute::texture` (`GLuint`)
  * `ui::attribute::use_text` (`GLuint`)
  * `ui::attribute::text_bgnd` (`GLuint`)
  * `ui::attribute::translate` (`GLuint`)

###### Inherited resources ######

* `ui::element::size` ([`ui::rect`](#rect))
* `ui::element::pixel_size` ([`ui::composite`](#composite))
* `ui::element::resize` (`ui::composite`)

#### Manager ####

The `ui::manager` class ([manager.h](../client/ui/manager.h) and
[manager.cc](../client/ui/manager.cc)) inherits from both the
[`ui::composite`](#composite) and the [`ui::widget`](#widget), and
functions as a very basic "bulletin board" widget.  Its children are
positioned independently within the confines of the manager, which can
grow and shrink to fit.  Each child's position is relative to the
upper-left corner of the manager.

The new child spacing resource controls the spacing between elements
and the edges of the manager, for purposes of grow/shrink.

##### Resources #####

* `ui::element::child_spacing`
  * `ui::size::width` (`int`)
  * `ui::size::height` (`int`)
  * `ui::size::all` (`glm::ivec2`)

###### Inherited resources ######

* `ui::element::size` ([`ui::rect`](#rect))
* `ui::element::pixel_size` (`ui::composite`)
* `ui::element::position` (`ui::widget`)
* `ui::element::border` (`ui::widget`)
* `ui::element::margin` (`ui::widget`)
* `ui::element::color` (`ui::widget`)

#### Popup Menu ####

The `ui::popupmenu` class ([popupmenu.h](../client/ui/popupmenu.h) and
[popupmenu.cc](../client/ui/popupmenu.cc)) inherits from the
[`ui::manager`](#manager), and implements a popup pie menu.

The border and margin function the same as the resources from the
widget, except for the names it recognizes.  "left" and "right" don't
have a lot of meaning for an ellipse with a hole in the middle of it,
so we instead use "inner" and "outer".

The popup resources control a couple of different aspects of the popup
menu.  The visible resource controls whether the menu is visible, and
the button resource controls which mouse button will cause the menu to
pop up.  Any of the button constants within `ui::mouse` are
satisfactory for this resource.

##### Resources #####

* `ui::element::border`
  * `ui::side::inner` (`GLuint`)
  * `ui::side::outer` (`GLuint`)
* `ui::element::margin`
  * `ui::side::inner` (`GLuint`)
  * `ui::side::outer` (`GLuint`)
* `ui::element::popup`
  * `ui::popup::visible` (`bool`)
  * `ui::popup::button` (`int`)

###### Inherited resources ######

* `ui::element::size` ([`ui::rect`](#rect))
* `ui::element::pixel_size` (`ui::composite`)
* `ui::element::position` ([`ui::widget`](#widget))
* `ui::element::border` (`ui::widget`)
* `ui::element::margin` (`ui::widget`)
* `ui::element::color` (`ui::widget`)

#### Row Column ####

The `ui::row_column` class ([row_column.h](../client/ui/row_column.h)
and [row_column.cc](../client/ui/row_column.cc)) inherits from the
[`ui::manager`](#manager), and arranges its children into a gridded
layout.

The inherited size resources are taken over by the grid, and can be
specified as a certain number of rows and columns.  Objects in excess
of the grid's size will cause the prescribed size to spill out along
the appropriate order (see order resource below).  One dimension may
be specified as 0, which indicates that the only constraint is the
other dimension.

There is an order resource, which indicates whether objects are added
to the grid in row-major or column-major order.

The row column extends the child spacing resource from the manager
widget to include the spacing between its grid elements.

##### Resources #####

* `ui::element::size`
  * `ui::size::rows` (`int`)
  * `ui::size::columns` (`int`)
  * `ui::size::grid` (`glm::ivec2`)
* `ui::element::order`
  * No subtypes, but defined value arguments (`GLuint`)
    * `ui::order::row`
    * `ui::order::column`

###### Inherited resources ######

* `ui::element::transform` ([`ui::composite`](#composite))
* `ui::element::pixel_size` (`ui::composite`)
* `ui::element::position` ([`ui::widget`](#widget))
* `ui::element::border` (`ui::widget`)
* `ui::element::margin` (`ui::widget`)
* `ui::element::color` (`ui::widget`)
* `ui::element::child_spacing` ([`ui::manager`](#manager))

### Support classes ###

#### Font ####

The `ui::font` ([font.h](../client/ui/font.h) and
[font.cc](../client/ui/font.cc)) is used by the `ui::label` and its
descendents.  It is a wrapper around libfreetype2, which provides the
specific functionality that we require:  sizing of glyphs and strings,
and rendering of strings into bitmaps.

Its interface shares very little in common with the rest of our UI
toolkit, but it is largely meant to be an internal interface, used
only by the calling widgets.

#### Image ####

The `ui::image` ([image.h](../client/ui/image.h)) is a simple
structure which holds image data and some metadata about the image
(size and bytes per pixel).

It has very little interface at all; a blank constructor, a copy
constructor, and an assignment operator.

#### Quadtree ####

The `ui::quadtree` ([quadtree.h](../client/ui/quadtree.h) and
[quadtree.cc](../client/ui/quadtree.cc)) is the search structure that
the `ui::composite` and its descendents use to quickly search their
area for children.  Almost all of the event handling routines use this
tree to locate the child which should be sent the event in question.

The interface is very simple; insert, search, remove, and clear.  It
deals only with `ui::widget` objects and descendents.
