ui::label
=========

## NAME ##

`ui::label` - CuddlyGL label class

## SYNOPSIS ##

```cpp
#include <cuddly-gl/label.h>

ui::label *l = new ui::label(parent, ...);
```

## DESCRIPTION ##

The `ui::label` displays a string or image within the boundaries of
the widget.  Setting x and y dimensions of the label will be ignored;
the string or image is wrapped as tightly as possible.  There will be
a 1-pixel internal margin between string/image and the label's
_border_ (see [`ui::widget`](ui-widget.md)(3) for description of the
border).  The string will be displayed in the label's _foreground_
color.

The `ui::label` can display either a string or an image, but not both
simultaneously.  Setting a string to be displayed will remove any
existing image, and setting an image to be displayed will remove any
existing string.

Setting the label's string uses `std::string` objects, which are
expected in UTF-8 format.  They are converted to 32-bit representation
internally.  Getting a string from a `ui::label` will return a
`std::string` in UTF-8 format.  All strings are processed through the
Unicode BIDI algorithm, so any mixed-direction strings will be
displayed as expected by Unicode rules.

Setting or getting the label's image uses the
[`ui::image`](ui-image.md)(3) object.

When setting a string, a font must also be specified.  Fonts may be
either _owned_ by the widget, in which case they will be deleted when
the label is deleted, or _shared_, which will not be deleted upon
label deletion.  The subtype argument for the font to **set()**
determines whether the font is owned or shared.

The `ui::label` is a subclass of [`ui::active`](ui-active.md)(3), so
it has all of the usual event handling available.  The label could be
made to act like a button, though the [`ui::button`](ui-button.md)(3)
has more typical button behaviours, and would be a more appropriate
choice.

## METHODS ##

The `ui::label` introduces no new methods.  All additions are
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

* **ui::element::font** - font used in rendering the string

  | Subtype               | Data type     | Notes                                              |
  | --------------------- | ------------- | -------------------------------------------------- |
  | ui::ownership::owned  | ui::base_font | Font will be deleted when the label is deleted     |
  | ui::ownership::shared | ui::base_font | Font will not be deleted when the label is deleted |

* **ui::element::string** - string displayed in the object

  | Subtype | Data type   | Notes           |
  | ------- | ----------- | --------------- |
  | none    | std::string | In UTF-8 format |

* **ui::element::image** - image displayed in the object

  | Subtype | Data type |
  | ------- | --------- |
  | none    | ui::image |

## INHERITED RESOURCES ##

* **ui::element::size** ([ui::rect](ui-rect.md))

  | Subtype              | Data type  |
  | -------------------- | ---------- |
  | **ui::size::width**  | GLuint     |
  | **ui::size::height** | GLuint     |
  | **ui::size::all**    | glm::ivec2 |

* **ui::element::position** ([ui::widget](ui-widget.md))

  | Subtype                    | Data type  |
  | -------------------------- | ---------- |
  | **ui::position::x**        | int        |
  | **ui::position::y**        | int        |
  | **ui::position::all**      | glm::ivec2 |
  | **ui::position::absolute** | *special*  |

* **ui::element::border** ([ui::widget](ui-widget.md))

  | Subtype              | Data type |
  | -------------------- | --------- |
  | **ui::side::top**    | GLuint    |
  | **ui::side::bottom** | GLuint    |
  | **ui::side::left**   | GLuint    |
  | **ui::side::right**  | GLuint    |
  | **ui::side::all**    | GLuint    |

* **ui::element::margin** ([ui::widget](ui-widget.md))

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

* **ui::element::state** ([ui::widget](ui-widget.md))

  | Subtype                | Data type |
  | ---------------------- | --------- |
  | **ui::state::visible** | bool      |

## SUBCLASSING ##

The `ui::label` has a virtual inheritance from the `ui::rect` by way
of the `ui::active` superclass, so the constructor of each subclass of
the `ui::label` must also explicitly initialize the `ui::rect`.

## RELATED ##

[ui::widget](ui-widget.md)(3), [ui::button](ui-button.md)(3),
[ui::text_field](ui-text_field.md)(3), [ui::base_font](ui-base_font.md)(3),
[ui::image](ui-image.md)(3)
