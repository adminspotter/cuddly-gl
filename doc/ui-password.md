ui::password
============

## NAME ##

`ui::password` - CuddlyGL password field widget class

## SYNOPSIS ##

```cpp
#include <cuddly-gl/password.h>

ui::password *p = new ui::password(parent, ...);
```

## DESCRIPTION ##

The `ui::password` is a text-input field which inherits from the
[`ui::text_field`](ui-text_field.md)(3).  Instead of showing the full
string, all printable characters are replaced with `*`, for hiding the
entered text.

## METHODS ##

The `ui::password` introduces no new methods.

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

## INHERITED RESOURCES ##

The `ui::password` adds no new resources.

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

* **ui::element::cursor** ([ui::text_field](ui-text_field.md))

  | Subtype                  | Data type |
  | ------------------------ | --------- |
  | **ui::cursor::position** | GLuint    |
  | **ui::cursor::blink**    | GLuint    |

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

* **ui::element::cursor** ([ui::text_field](ui-text_field.md))

  | Subtype                  | Data type |
  | ------------------------ | --------- |
  | **ui::cursor::position** | GLuint    |
  | **ui::cursor::blink**    | GLuint    |

* **ui::element::size** ([ui::rect](ui-rect.md))

  | Subtype                 | Data type  |
  | ----------------------- | ---------- |
  | **ui::size::width**     | GLuint     |
  | **ui::size::height**    | GLuint     |
  | **ui::size::all**       | glm::ivec2 |
  | **ui::size::max_width** | GLuint     |

* **ui::element::state** ([ui::widget](ui-widget.md))

  | Subtype                | Data type |
  | ---------------------- | --------- |
  | **ui::state::visible** | bool      |

* **ui::element::string** ([ui::label](ui-label.md))

  | Subtype | Data type   |
  | ------- | ----------- |
  | none    | std::string |

## SUBCLASSING ##

The `ui::password` has virtual inheritances from the `ui::rect` by way
of the `ui::active` superclass, and from the `ui::active` by way of
the `ui::widget` superclass, so the constructor of each subclass of
the `ui::password` must also explicitly initialize the `ui::rect` and
`ui::active`.

## RELATED ##

[ui::text_field](ui-text_field.md)(3)
