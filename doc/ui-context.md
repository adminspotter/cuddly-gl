ui::context
===========

## NAME ##

`ui::context` - CuddlyGL top-level composite widget class

## SYNOPSIS ##

```cpp
#include <cuddly-gl/ui.h>

ui::context *c = new ui::context(...);

c->draw();
```

## DESCRIPTION ##

The `ui::context` is a composite widget which acts as the top-level
widget in any hierarchy.  It is the ultimate parent; it has no parent
of its own.  It also manages the shader program which is used across
all CuddlyGL widgets.

It is used by various connector modules to hook up to the windowing
toolkit which is being used.  Other than to add widgets, there is
typically very little direct programmatic interaction with the
`ui::context`.

## METHODS ##

* **draw()**

  Draw the entire widget hierarchy on the screen.  It is typically
  called within the main program's rendering event loop.

Inherited from [`ui::composite`](ui-composite.md):

* **add_child(ui::widget \*_widget_)**
* **move_child(ui::widget \*_widget_)**
* **remove_child(ui::widget \*_widget_)**
* **manage_children()**
* **mouse_pos_callback(int _x_, int _y_)**
* **mouse_pos_callback(glm::ivec2& _pos_)**
* **mouse_btn_callback(int _btn_, int _state_, int _mods_)**
* **mouse_btn_callback(ui::btn_call_data& _call_data_)**
* **key_callback(int _key_, uint32_t _char_, int _state_, int _mods_)**
* **key_callback(ui::key_call_data& _call_data_)**

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

* **ui::element::attribute** - read-only attributes used internally to
  inject CuddlyGL parameters into the OpenGL rendering program.

  | Subtype                  | Data type |
  | ------------------------ | --------- |
  | ui::attribute::position  | GLuint    |
  | ui::attribute::color     | GLuint    |
  | ui::attribute::texture   | GLuint    |
  | ui::attribute::text_bgnd | GLuint    |

## INHERITED RESOURCES ##

* **ui::element::child** ([ui::composite](ui-composite.md))

  | Subtype            | Data type    |
  | ------------------ | ------------ |
  | ui::child::focused | ui::widget * |
  | ui::child::radio   | ui::widget * |

* **ui::element::pixel_size** ([ui::composite](ui-composite.md))

  | Subtype          | Data type |
  | ---------------- | --------- |
  | ui::size::width  | float     |
  | ui::size::height | float     |
  | ui::size::all    | glm::vec3 |

* **ui::element::size** ([ui::rect](ui-rect.md))

  | Subtype              | Data type  |
  | -------------------- | ---------- |
  | **ui::size::width**  | GLuint     |
  | **ui::size::height** | GLuint     |
  | **ui::size::all**    | glm::ivec2 |

* **ui::element::state** ([ui::composite](ui-composite.md))

  | Subtype              | Data type |
  | -------------------- | --------- |
  | ui::state::radio_box | bool      |

## SUBCLASSING ##

This widget is not meant to be subclassed.

## RELATED ##

[ui::composite](ui-composite.md)(3)
