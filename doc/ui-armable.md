ui::armable
===========

## NAME ##

`ui::armable` - CuddlyGL armable widget class

## SYNOPSIS ##

```cpp
#include <cuddly-gl/armable.h>

ui::armable *a = new ui::armable(parent, ...);
```

## DESCRIPTION ##

The `ui::armable` class is an intermediate class which adds handling
related to button actions.  It subclasses the `ui::label` to add
`active` and `armed` states, with callbacks for enter/exit, focus,
button down/up, and key down/up.

When the widget gains focus, either by receiving focus from its
parent, or by the cursor entering the widget area, it becomes
*activated*.  On losing focus, either from the parent or by the cursor
leaving the widget area, the widget is *deactivated*.

When mouse button 0 or the space key is pressed while the widget is
active, it becomes *armed*.  Once the button or key is released, the
widget is *disarmed*.  When the widget is deactivated while armed, it
is also disarmed.

Both activation and arming generally manifest as small changes in
appearance, often highlights of some sort.  Disarming and deactivation
will usually roll back such changes.  Since they are
context-dependent, the `ui::armable` does not perform any appearance
changes upon activation/deactivation or arm/disarm; subclasses must
implement any such changes.

The `ui::armable` does not add any visual differences over the
`ui::label`, and isn't meant to be used directly.  It is intended only
to layer functionality in for more flexible inheritance points.  Use
the `ui::button` or `ui::toggle` when button or checkbox semantics are
needed.

## METHODS ##

The `ui::armable` introduces no new methods.  All additions are
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

  | Subtype           | Data type |
  | ----------------- | --------- |
  | ui::state::active | bool      |
  | ui::state::armed  | bool      |

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

  | Subtype               | Data type     |
  | --------------------- | ------------- |
  | ui::ownership::owned  | ui::base_font |
  | ui::ownership::shared | ui::base_font |

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

* **ui::element::state** ([ui::widget](ui-widget.md))

  | Subtype                | Data type |
  | ---------------------- | --------- |
  | **ui::state::visible** | bool      |

* **ui::element::string** ([ui::label](ui-label.md))

  | Subtype | Data type   |
  | ------- | ----------- |
  | none    | std::string |

## SUBCLASSING ##

The `ui::armable` has virtual inheritances from the `ui::rect` by way
of the `ui::active` superclass, and from the `ui::active` by way of
the `ui::widget` superclass, so the constructor of each subclass of
the `ui::armable` must also explicitly initialize the `ui::rect` and
`ui::active`.

Any subclasses which require visual behaviours upon
activate/deactivate or arm/disarm must override the relevant protected
`activate`, `deactivate`, `arm`, or `disarm` methods.

## RELATED ##

[ui::label](ui-label.md)(3), [ui::button](ui-button.md)(3),
[ui::toggle](ui-toggle.md)(3)
