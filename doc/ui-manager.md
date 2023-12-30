ui::manager
===========

## NAME ##

`ui::manager` - CuddlyGL manager composite widget class

## SYNOPSIS ##

```cpp
#include <cuddly-gl/manager.h>

ui::manager *m = new ui::manager(parent, ...);
```

## DESCRIPTION ##

The `ui::manager` is the most basic composite widget which can be
drawn on the screen.  It functions as a "bulletin board", in that
child widgets are largely on their own to determine where they will be
placed.  Depending on settings, the `ui::manager` can automatically
grow or shrink to fit all its children.

## METHODS ##

The `ui::manager` introduces no new methods.  All additions are
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

Inherited from [ui::composite](ui-composite.md):

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

## NEW RESOURCES ##

* **ui::element::child_spacing** - how many pixels of padding should
  be left between children and the border of the manager.  The
  `ui::size::all` subtype is a convenience flag for both
  `ui::size::width` and `ui::size::height` flags or'ed together, and
  does require a change of data type.

  | Subtype              | Data type  |
  | -------------------- | ---------- |
  | **ui::size::width**  | GLuint     |
  | **ui::size::height** | GLuint     |
  | **ui::size::all**    | glm::ivec2 |

* **ui::element::resize** - how the manager should/should not change
  its own size according to the placement of its children.  There are
  no subtypes, but there are defined values of type `GLuint`.  The
  defined values function as a mask, and can be or'ed together.  The
  `ui::resize::all` value is a convenience flag for both
  `ui::resize::shrink` and `ui::resize::grow`.

  | Value                  | Notes                          |
  | ---------------------- | ------------------------------ |
  | **ui::resize::none**   | Do not change size             |
  | **ui::resize::shrink** | Only shrink to fit children    |
  | **ui::resize::grow**   | Grow to fit children as needed |
  | **ui::resize::all**    | Change size as needed          |

## INHERITED RESOURCES ##

* **ui::element::border** ([ui::widget](ui-widget.md))

  | Subtype              | Data type |
  | -------------------- | --------- |
  | **ui::side::top**    | GLuint    |
  | **ui::side::bottom** | GLuint    |
  | **ui::side::left**   | GLuint    |
  | **ui::side::right**  | GLuint    |
  | **ui::side::all**    | GLuint    |

* **ui::element::child** ([ui::composite](ui-composite.md))

  | Subtype                | Data type    |
  | ---------------------- | ------------ |
  | **ui::child::focused** | ui::widget * |
  | **ui::child::radio**   | ui::widget * |

* **ui::element::color** ([ui::widget](ui-widget.md))

  | Subtype                   | Data type |
  | ------------------------- | --------- |
  | **ui::color::foreground** | glm::vec4 |
  | **ui::color::background** | glm::vec4 |

* **ui::element::margin** ([ui::widget](ui-widget.md))

  | Subtype              | Data type |
  | -------------------- | --------- |
  | **ui::side::top**    | GLuint    |
  | **ui::side::bottom** | GLuint    |
  | **ui::side::left**   | GLuint    |
  | **ui::side::right**  | GLuint    |
  | **ui::side::all**    | GLuint    |

* **ui::element::pixel_size** ([ui::composite](ui-composite.md))

  | Subtype              | Data type |
  | -------------------- | --------- |
  | **ui::size::width**  | float     |
  | **ui::size::height** | float     |
  | **ui::size::all**    | glm::vec3 |

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

  | Subtype                  | Data type | Class                              |
  | ------------------------ | --------- | ---------------------------------- |
  | **ui::state::visible**   | bool      | ([ui::widget](ui-widget.md))       |
  | **ui::state::radio_box** | bool      | ([ui::composite](ui-composite.md)) |

## SUBCLASSING ##

The `ui::manager` has virtual inheritances from the `ui::rect` by way
of the `ui::active` superclass, and from the `ui::active` by way of
the `ui::widget` superclass, so the constructor of each subclass of
the `ui::manager` must also explicitly initialize the `ui::rect` and
`ui::active`.

## RELATED ##

[ui::widget](ui-widget.md)(3), [ui::composite](ui-composite.md)(3),
[ui::pie_menu](ui-pie-menu.md)(3), [ui::row_column](ui-row_column.md)(3)
