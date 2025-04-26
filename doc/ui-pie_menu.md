ui::pie_menu
============

## NAME ##

`ui::pie_menu` - CuddlyGL pie menu composite class

## SYNOPSIS ##

```cpp
#include <cuddly-gl/pie_menu.h>

ui::pie_menu *p = new ui::pie_menu(parent, ...);
```

## DESCRIPTION ##

The `ui::pie_menu` class implements an ellipsoid popup menu.  It is
divided up into a number of sectors equal to the number of children
the menu has.  Positioning of the children is handled by the pie menu;
each child is placed in one of the sectors.  Order of the sectors is
clockwise from positive x-axis.

The pie menu becomes visible on *btn_down* events for the configured
button within its parent, and hides on corresponding *btn_up* events.

The menu determines which sector is selected, and dispatches events to
the appropriate child.  The children do not need any armable
behaviour, so the [`ui::label`](ui-label.md)(3) is a satisfactory
child widget type.  Each child should be set with a *btn_up* callback
handler, to perform whatever action is appropriate for the menu
option.  If the cursor is not within the menu area when the mouse
button is released, no *btn_up* event will be generated for any child.

## METHODS ##

The `ui::pie_menu` introduces no new methods.  All additions are
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

* **ui::element::border** - new subtypes for inner and outer borders

  | Subtype             | Data type |
  | ------------------- | --------- |
  | **ui::side::inner** | GLuint    |
  | **ui::size::outer** | GLuint    |

* **ui::element::margin** - new subtypes for inner and outer margins

  | Subtype             | Data type |
  | ------------------- | --------- |
  | **ui::side::inner** | GLuint    |
  | **ui::size::outer** | GLuint    |

* **ui::element::popup** - the mouse button which should pop up the
  menu; valid values are one of `ui::mouse::button*`.

  | Subtype               | Data type |
  | --------------------- | --------- |
  | **ui::popup::button** | GLuint    |

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

* **ui::element::child_spacing** ([ui::manager](ui-manager.md))

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

* **ui::element::resize** ([ui::manager](ui-manager.md))

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

## SUBCLASSING ##

The `ui::pie_menu` has virtual inheritances from the `ui::rect` by way
of the `ui::active` superclass, and from the `ui::active` by way of
the `ui::widget` superclass, so the constructor of each subclass of
the `ui::pie_menu` must also explicitly initialize the `ui::rect` and
`ui::active`.

## BUGS ##

The inner radius is a compile-time definition, and not a settable
parameter.

## RELATED ##

[ui::manager](ui-manager.md)(3), [ui::label](ui-label.md)(3)
