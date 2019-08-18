ui::widget
============

## NAME ##

`ui::widget` - CuddlyGL basic widget class

## SYNOPSIS ##

```cpp
#include <cuddly-gl/widget.h>

ui::widget *w = new ui::widget(parent, ...);

w->draw(trans_uniform, parent_trans);

w->close();
```

## DESCRIPTION ##

The `ui::widget` class is the most basic element of the CuddlyGL
toolkit which can be drawn on the screen.  It adds OpenGL handling of
VAO, VBO, and EBO buffers, interaction with the `ui::vertex_buffer`,
and a set of basic visual elements:  _position_, _border_, _margin_,
foreground/background _color_, and _visibility_.

The _border_ is a set of lines which define the edge of the usable
space of the widget.  They are separated from the edge of the widget's
area by the _margin_.  The default widget has no borders or margins,
and the usable space extends to the edges of the widget's area.

```plain
+----------------------+
|        margin        |
|   +----border---+    |
|   |             |    |
|   |   contents  |    |
|   |             |    |
|   +-------------+    |
|                      |
+----------------------+
```

The widget's *parent*, a subclass of type `ui::composite`, must be
specified at creation time, and can not be changed during the lifetime
of the object.

## METHODS ##

* **draw(GLuint _translation_uniform_, const glm::mat4& _parent_translation_)**

  Draws on the OpenGL pane using the VAO, VBO, and EBO of the object.
  The _translation_uniform_ and _parent_translation_ are used together
  to translate the widget to the right place on screen.

  This method would not typically be called directly; the
  [`ui::context`](ui-context.md)'s `draw()` method recursively handles
  the various operations with translation uniforms and matrices for
  all its children.

* **close()**

  Closes the widget.  The object will immediately be made invisible
  and removed from its parent's search tree.  It may not be deleted
  immediately.

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

* **ui::element::position** - position of the object within its
    parent's boundary

  | Subtype               | Data type  | Notes                               |
  | --------------------- | ---------- | ----------------------------------- |
  | **ui::position::x**   | GLuint     |                                     |
  | **ui::position::x**   | GLuint     |                                     |
  | **ui::position::all** | glm::ivec2 | Set both x and y parameters at once |

* **ui::element::border** - width of the border segments for each
    side.  The subtype flags function as a mask, and can be or'ed
    together.  The `ui::side::all` subtype is a convenience flag of all
    other flags or'ed together.

  | Subtype              | Data type |
  | -------------------- | --------- |
  | **ui::side::top**    | GLuint    |
  | **ui::side::bottom** | GLuint    |
  | **ui::side::left**   | GLuint    |
  | **ui::side::right**  | GLuint    |
  | **ui::side::all**    | GLuint    |

* **ui::element::margin** - width of the margins for each side.  The
    subtype flags function as a mask, and can be or'ed together.  The
    `ui::side::all` subtype is a convenience flag of all other flags
    or'ed together.

  | Subtype              | Data type |
  | -------------------- | --------- |
  | **ui::side::top**    | GLuint    |
  | **ui::side::bottom** | GLuint    |
  | **ui::side::left**   | GLuint    |
  | **ui::side::right**  | GLuint    |
  | **ui::side::all**    | GLuint    |

* **ui::element::color** - colors for the widget, in RGBA format

  | Subtype                   | Data type | Notes              |
  | ------------------------- | --------- | ------------------ |
  | **ui::color::foreground** | glm::vec4 | Used by the border |
  | **ui::color::background** | glm::vec4 |                    |

* **ui::element::state** - flags for the widget

  | Subtype                | Data type |
  | ---------------------- | --------- |
  | **ui::state::visible** | bool      |

## INHERITED RESOURCES ##

* **ui::element::size** ([ui::rect](ui-rect.md))

  | Subtype              | Data type  |
  | -------------------- | ---------- |
  | **ui::size::width**  | GLuint     |
  | **ui::size::height** | GLuint     |
  | **ui::size::all**    | glm::ivec2 |

## SUBCLASSING ##

The `ui::widget` has a virtual inheritance from the `ui::rect` by way
of the `ui::active` superclass, so the constructor of each subclass of
the `ui::widget` must also explicitly initialize the `ui::rect`.

Any special handling that needs to be performed when a subclass is
_closed_ (as opposed to _deleted_) should be handled in an overridden
`close()` method, since the two operations may not be performed at the
same time.

## RELATED ##

[ui::rect](ui-rect.md)(3), [ui::active](ui-active.md)(3),
[ui::composite](ui-composite.md)(3)
