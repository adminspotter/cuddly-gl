ui::composite
=============

## NAME ##

`ui::composite` - CuddlyGL composite widget class

## SYNOPSIS ##

```cpp
#include <cuddly-gl/composite.h>

ui::composite *c = new ui::composite(parent, ...);

c->add_child(child_widget);
c->move_child(child_widget);
c->remove_child(child_widget);

c->manage_children();

c->mouse_pos_callback(x, y);
c->mouse_pos_callback(xy_vector);

c->mouse_btn_callback(ui::mouse::button0, ui::mouse::up, ui::key_mod::shift);
c->mouse_btn_callback(btn_call_data);

c->key_callback(ui::key::a, ui::key::down, ui::key_mod::meta);
c->key_callback(key_call_data);
```

## DESCRIPTION ##

The `ui::composite` class is an intermediate class which adds the
concept of child widgets, and is the base of classes which implement
hierarchical trees for arranging individual widgets.  It subclasses
the `ui::active` to add lists of children.

One of the important parts of a hierarchy is the ability to dispatch
events to the appropriate child, based on their location within the
area of the composite, or based on event types.  This is referred to
as *focus* or *keyboard focus*, which is described below.

As with its immediate superclass [`ui::active`](ui-active.md)(3), the
`ui::composite` has no interaction with OpenGL, and can not be drawn
on the screen.  It is the base class for all widgets which take
children.

## WIDGET FOCUS ##

The *focus* of a composite widget refers to the dispatch of events to
the appropriate child widgets.  When a given widget is said to *have
focus*, it will be the recipient of any events which occur on the
level of the enclosing composite widget.  Arbitrating focus is handled
by two different mechanisms.

Dispatching events based on the location of the mouse cursor is the
most direct method.  Motion events are always delivered to the widget
underneath the mouse cursor.  Keyboard focus is acquired upon click of
the widget in question.  Any visible widget may receive focus by click
or by location.

Alternatively, focus may also be handled with keyboard shortcuts:  tab
and shift-tab will cycle focus through the composite's child widgets
in order of addition.  Nested composites act as a bit of a focus sink,
in that once focus drops from one composite into a child composite, it
will never come back to the parent by keyboard shortcuts alone.

## RADIO BOX ##

When the `ui::state::radio_box` resource of a `ui::composite` is set
to `true`, all children of type [`ui::toggle`](ui-toggle.md)(3) are
treated as a *radio group*.  The composite widget will manage which
toggle of the group is checked, and will set all others to be
unchecked.

Querying which child is checked will return a pointer to the object in
question.

```cpp
ui::composite *c = new ui::composite(...);
ui::toggle *t1 = new ui::toggle(c, ...);
ui::toggle *t2 = new ui::toggle(c, ...);

c->set(ui::element::state, ui::state::radio_box, true);
t1->set(ui::element::state, ui::state::checked, true);

ui::toggle *checked_obj;

c->get(ui::element::child, ui::child::radio, &checked_obj);

/* checked_obj will be t1 */

bool is_checked;

t2->set(ui::element::state, ui::state::checked, true);
t1->get(ui::element::state, ui::state::checked, &is_checked);

/* is_checked will be false */
```

## METHODS ##

* **add_child(ui::widget \*_widget_)**

  Add a new child to the composite.  Multiple additions of the same child
  are ignored.

* **move_child(ui::widget \*_widget_)**

  Recalculate the position of a given child within the composite.
  Each child maintains its own position, so when a child's position is
  changed, this method must be called to update the internal state of
  the composite object.  These calls are handled by the children
  themselves, so any manual calls should be mostly unnecessary.

* **remove_child(ui::widget \*_widget_)**

  Delete the child from the composite.  The child will immediately be
  made invisible and removed from the search tree, but will not be
  deleted until `manage_children` is called.

* **manage_children()**

  Delete any removed children.

The following methods deal with focus to individual child widgets;
calls are handled automatically, and are initiated by the various
windowing toolkit connector modules.

* **mouse_pos_callback(int _x_, int _y_)**
* **mouse_pos_callback(glm::ivec2& _pos_)**

  Handle mouse motion within the area of the composite.

* **mouse_btn_callback(int _btn_, int _state_, int _mods_)**
* **mouse_btn_callback(ui::btn_call_data& _call_data_)**

  Handle mouse button events within the area of the composite.

* **key_callback(int _key_, uint32_t _char_, int _state_, int _mods_)**
* **key_callback(ui::key_call_data& _call_data_)**

  Handle key events within the area of the composite.

Inherited from [`ui::rect`](ui-rect.md):

* **get(GLuint _type_, GLuint _subtype_, _res_type_ \*_obj_ptr_, ...) const**
* **set(GLuint _type_, GLuint _subtype_, _res_type_ _obj_, ...)**

Inherited from [`ui::active`](ui-active.md):

* **add_callback(GLuint _list_, ui::cb_fptr _func_ptr_, void \*_client_data_)**
* **remove_callback(GLuint _list_, ui::cbfptr _func_ptr_, void \*_client_data_)**
* **call_callbacks(GLuint _list_, void \*_call_data_)**
* **add_timeout(ui::to_time::duration _until_, ui::to_fptr _func_ptr_, void \*_client_data_)**
* **remove_timeout()**
* **call_timeout()**

## NEW RESOURCES ##

* **ui::element::child** - get or set child widgets for specific actions

  | Subtype            | Data type    | Notes                             |
  | ------------------ | ------------ | --------------------------------- |
  | ui::child::focused | ui::widget * | Child which has keyboard focus    |
  | ui::child::radio   | ui::widget * | Checked member of the radio group |

* **ui::element::pixel_size** - read-only resources used by child
  widgets to scale themselves properly for the size of the composite.

  | Subtype          | Data type |
  | ---------------- | --------- |
  | ui::size::width  | float     |
  | ui::size::height | float     |
  | ui::size::all    | glm::vec3 |

* **ui::element::state** - boolean flags for changing widget behaviour

  | Subtype              | Data type |
  | -------------------- | --------- |
  | ui::state::radio_box | bool      |

## INHERITED RESOURCES ##

* **ui::element::size** ([ui::rect](ui-rect.md))

  | Subtype              | Data type  |
  | -------------------- | ---------- |
  | **ui::size::width**  | GLuint     |
  | **ui::size::height** | GLuint     |
  | **ui::size::all**    | glm::ivec2 |

## SUBCLASSING ##

The `ui::composite` has virtual inheritances from the `ui::rect` by
way of the `ui::active` superclass, and from the `ui::active`
superclass, so the constructor of each subclass of the `ui::composite`
must also explicitly initialize the `ui::rect` and `ui::active`.

## BUGS ##

The radio box function should be implemented elsewhere.  It would be
more appropriate as a decorator or template which can wrap any
existing composite class.

## RELATED ##

[ui::active](ui-active.md)(3), [ui::manager](ui-manager.md)(3),
[ui::context](ui-context.md)(3)
