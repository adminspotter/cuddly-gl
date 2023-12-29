ui::row_column
==============

## NAME ##

`ui::row_column` - CuddlyGL row-column composite widget class

## SYNOPSIS ##

```cpp
#include <cuddly-gl/row_column.h>

ui::row_column *t = new ui::row_column(parent, ...);
```

## DESCRIPTION ##

The `ui::row_column` is a composite widget which arranges its children
in a gridded layout.  New resources define the packing order for child
widgets, either row-first or column-first, and can specify the size of
the row-column widget in terms of number of rows, or number of
columns, or both.

## METHODS ##

The `ui::row_column` adds no new methods.

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

Inherited from [`ui::active`](ui-active.md):

* **add_callback(GLuint _list_, ui::cb_fptr _func_ptr_, void \*_client_data_)**
* **remove_callback(GLuint _list_, ui::cbfptr _func_ptr_, void \*_client_data_)**
* **call_callbacks(GLuint _list_, void \*_call_data_)**
* **add_timeout(ui::to_time::duration _until_, ui::to_fptr _func_ptr_, void \*_client_data_)**
* **remove_timeout()**
* **call_timeout()**

## NEW RESOURCES ##

* **ui::element::order** - sets how children are packed into the grid.
  There are no subtypes, but there are defined values of type `GLuint`
  to specify row-first packing or column-first packing.

  | Value                 |
  | --------------------- |
  | **ui::order::row**    |
  | **ui::order::column** |

* **ui::element::size** - new subtypes to specify the number of rows
  and/or columns.  Pixel size of the row-column is computed
  automatically for the specified layout.

  | Subtype               | Data type  |
  | --------------------- | ---------- |
  | **ui::size::rows**    | GLuint     |
  | **ui::size::columns** | GLuint     |
  | **ui::size::grid**    | glm::ivec2 |

  The existing `ui::size::width`, `ui::size::height`, and
  `ui::size::all` subtypes are still available, but the grid sizes
  above override these resources.

The `ui::row_column` also slightly changes the semantics of the
`ui::element::child_spacing` resource.  In the `ui::manager`, child
spacing is only used between the child widgets and the border of the
manager.  The row-column continues the edge-padding behaviour of the
manager, but the child spacing is also used as the spacing between
successive rows (`ui::size::height`) and successive columns
(`ui::size::width`).

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

* **ui::element::child_spacing**

  | Subtype              | Data type  |
  | -------------------- | ---------- |
  | **ui::size::width**  | GLuint     |
  | **ui::size::height** | GLuint     |
  | **ui::size::all**    | glm::ivec2 |

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

* **ui::element::resize**

  | Value                  |
  | ---------------------- |
  | **ui::resize::none**   |
  | **ui::resize::shrink** |
  | **ui::resize::grow**   |
  | **ui::resize::all**    |

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

The `ui::row_column` has virtual inheritances from the `ui::rect` by
way of the `ui::active` superclass, and from the `ui::active` by way
of the `ui::widget` superclass, so the constructor of each subclass of
the `ui::row_column` must also explicitly initialize the `ui::rect`
and `ui::active`.

## RELATED ##

[ui::manager](ui-manager.md)(3)
