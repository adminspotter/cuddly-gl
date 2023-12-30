ui::toggle
============

## NAME ##

`ui::toggle` - CuddlyGL toggle button class

## SYNOPSIS ##

```cpp
#include <cuddly-gl/toggle.h>

ui::toggle *t = new ui::toggle(parent, ...);
```

## DESCRIPTION ##

The `ui::toggle` displays a string or image button alongside a box
which shows the *checked* state of the widget.  The box is filled when
the widget is *checked*, and unfilled when the widget is *unchecked*.

Sets of `ui::toggle` widgets can become a *radio group* by being
direct children of a [`ui::composite`](ui-composite.md)(3) derivative
with the `ui::state::radio_box` resource set to `true`.  A radio group
may only have a single toggle checked at one time; checking a
`ui::toggle` in a radio group unchecks all other `ui::toggle`s in that
group.

The `ui::toggle` is a subclass of [`ui::button`](ui-button.md)(3), so
all typical button behaviour is present; *arming* and *activating* the
`ui::toggle` acts the same as the `ui::button`.

## METHODS ##

The `ui::toggle` introduces no new methods.  All additions are
resources, described below.

Inherited from [ui::rect](ui-rect.md):

* **get(GLuint _type_, GLuint _subtype_, _res_type_ \*_obj_ptr_, ...) const**
* **set(GLuint _type_, GLuint _subtype_, _res_type_ _obj_, ...)**

Inherited from [ui::active](ui-active.md):

* **add_callback(GLuint _list_, ui::cb_fptr _func_ptr_, void \*_client_data_)**
* **remove_callback(GLuint _list_, ui::cbfptr _func_ptr_, void \*_client_data_)**
* **call_callbacks(GLuint _list_, void \*_call_data_)**
* **add_timeout(ui::to_time::duration _until_, ui::to_fptr _func_ptr_, void \*_client_data_)**
* **remove_timeout()**
* **call_timeout()**

Inherited from [ui::widget](ui-widget.md):

* **draw(GLuint _translation_uniform_, const glm::mag4& _parent_translation_)**
* **close()**

## NEW RESOURCES ##

* **ui::element::state** - state of the object

  | Subtype                | Data type |
  | ---------------------- | --------- |
  | **ui::state::checked** | bool      |

## INHERITED RESOURCES ##

* **ui::element::border** ([ui::widget](ui-widget.md))

  | Subtype              | Data type |
  | -------------------- | --------- |
  | **ui::side::top**    | GLuint    |
  | **ui::side::bottom** | GLuint    |
  | **ui::side::left**   | GLuint    |
  | **ui::side::right**  | GLuint    |
  | **ui::side::all**    | GLuint    |

* **ui::element::color** ([ui::widget](ui-widget.md))

  | Subtype                   | Data type |
  | ------------------------- | --------- |
  | **ui::color::foreground** | glm::vec4 |
  | **ui::color::background** | glm::vec4 |

* **ui::element::font** ([ui::label](ui-label.md))

  | Subtype                   | Data type     |
  | ------------------------- | ------------- |
  | **ui::ownership::owned**  | ui::base_font |
  | **ui::ownership::shared** | ui::base_font |

* **ui::element::image** ([ui::label](ui-label.md))

  | Subtype | Data type |
  | ------- | --------- |
  | none    | ui::image |

* **ui::element::margin** ([ui::widget](ui-widget.md))

  | Subtype              | Data type |
  | -------------------- | --------- |
  | **ui::side::top**    | GLuint    |
  | **ui::side::bottom** | GLuint    |
  | **ui::side::left**   | GLuint    |
  | **ui::side::right**  | GLuint    |
  | **ui::side::all**    | GLuint    |

* **ui::element::position** ([ui::widget](ui-widget.md))

  | Subtype                    | Data type  |
  | -------------------------- | ---------- |
  | **ui::position::x**        | int        |
  | **ui::position::y**        | int        |
  | **ui::position::all**      | glm::ivec2 |
  | **ui::position::absolute** | *special*  |

* **ui::element::size** ([ui::rect](ui-rect.md))

  | Subtype              | Data type  |
  | -------------------- | ---------- |
  | **ui::size::width**  | GLuint     |
  | **ui::size::height** | GLuint     |
  | **ui::size::all**    | glm::ivec2 |

* **ui::element::state**

  | Subtype                | Data type | Class                          |
  | ---------------------- | --------- | ------------------------------ |
  | **ui::state::visible** | bool      | ([ui::widget](ui-widget.md))   |
  | **ui::state::active**  | bool      | ([ui::armable](ui-armable.md)) |
  | **ui::state::armed**   | bool      |                                |

* **ui::element::string** ([ui::label](ui-label.md))

  | Subtype | Data type   |
  | ------- | ----------- |
  | none    | std::string |

## SUBCLASSING ##

The `ui::toggle` has virtual inheritances from the `ui::rect` by way
of the `ui::active` superclass, and from the `ui::active` by way of
the `ui::widget` superclass, so the constructor of each subclass of
the `ui::toggle` must also explicitly initialize the `ui::rect` and
`ui::active`.

## RELATED ##

[ui::armable](ui-armable.md)(3), [ui::button](ui-button.md)(3)
