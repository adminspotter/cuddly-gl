ui::button
============

## NAME ##

`ui::button` - CuddlyGL button class

## SYNOPSIS ##

```cpp
#include <cuddly-gl/button.h>

ui::button *b = new ui::button(parent, ...);
```

## DESCRIPTION ##

The `ui::button` displays an active label or image, and makes simple
changes upon focus and use.  It inherits from the `ui::armable`, and
implements the active/armed concepts on top of the `ui::label`.  The
_active_ state is reached when the button receives focus, which grows
the border width by 1 pixel.  The _armed_ state is reached when the
button is depressed, which also grows the border width by 1 pixel.
Leaving either state reduces the border width again by 1 pixel for
each state.

Any other actions which the button should perform should be handled
via its `ui::callback::btn_down` or `ui::callback::btn_up` callback
lists.

## METHODS ##

The `ui::button` introduces no new methods.

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

The `ui::button` introduces no new resources.

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

The `ui::button` has virtual inheritances from the `ui::rect` by way
of the `ui::active` superclass, and from the `ui::active` by way of
the `ui::widget` superclass, so the constructor of each subclass of
the `ui::button` must also explicitly initialize the `ui::rect` and
`ui::active`.

## RELATED ##

[ui::armable](ui-armable.md)(3)
