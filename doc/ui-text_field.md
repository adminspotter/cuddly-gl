ui::text_field
==============

## NAME ##

`ui::text_field` - CuddlyGL text field widget class

## SYNOPSIS ##

```cpp
#include <cuddly-gl/text_field.h>

ui::text_field *t = new ui::text_field(parent, ...);
```

## DESCRIPTION ##

The `ui::text_field` is the basic text input widget.  It derives from
the [`ui::label`](ui-label.md)(3), and adds a movable, blinking
cursor, and the ability to alter the text contents of the widget.

The cursor is moved one character left or right by the corresponding
arrow keys.  The `home` key moves the cursor to the beginning of the
string, and the `end` key moves the cursor to the end.  Printable keys
insert their appropriate character at the current cursor position.
Printable characters should follow the directionality of the script in
question.  The `backspace` key removes the character before the cursor
position, and the `delete` key removes the character after the cursor.

## FOCUS HOOK ##

In order to better support key repeat in some contexts, it is
sometimes necessary to turn off some callback slots when the text
field receives focus.  There is, however, not currently a way to
auto-add a given callback function to all instances of a given class,
when that callback is not known at library compile time.  The
`ui::text_field` has a static function pointer attribute called
`ui::text_field::focus_hook` which is meant for this purpose.

```cpp
void some_func(bool has_focus)
{
    ...
}

ui::text_field::focus_hook = &some_func;
```

It is a public part of the class, but is likely only of interest to
writers of connector modules for windowing toolkits.

## METHODS ##

The `ui::text_field` introduces no new methods.  All additions are
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

* **ui::element::cursor** - text cursor settings.

  | Subtype                  | Data type | Notes                                   |
  | ------------------------ | --------- | --------------------------------------- |
  | **ui::cursor::position** | GLuint    | Character position                      |
  | **ui::cursor::blink**    | GLuint    | Milliseconds per half-cycle, 0 disables |

* **ui::element::repeat** - how quickly character repeat starts, and
  how often it repeats.  Rates are in milliseconds.

  | Subtype                   | Data type |
  | ------------------------- | --------- |
  | **ui::repeat::initial**   | GLuint    |
  | **ui::repeat::secondary** | GLuint    |

* **ui::element::size** - additional subtype to set maximum width in
  character cells, instead of pixels.

  | Subtype                 | Data type |
  | ----------------------- | --------- |
  | **ui::size::max_width** | GLuint    |

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

* **ui::element::state** ([ui::widget](ui-widget.md))

  | Subtype                | Data type |
  | ---------------------- | --------- |
  | **ui::state::visible** | bool      |

* **ui::element::string** ([ui::label](ui-label.md))

  | Subtype | Data type   |
  | ------- | ----------- |
  | none    | std::string |

## SUBCLASSING ##

The `ui::text_field` has virtual inheritances from the `ui::rect` by
way of the `ui::active` superclass, and from the `ui::active` by way
of the `ui::widget` superclass, so the constructor of each subclass of
the `ui::text_field` must also explicitly initialize the `ui::rect`
and `ui::active`.

## BUGS ##

The cursor can not be moved via mouse click, only by arrow and
home/end keys.

There is no facility for making a selection within the text field.

The `focus_hook` is a terribly hacky mechanism.  Better would be a
more general method to add class-wide creation hooks.

`ui::repeat::secondary` is not a great name.

## RELATED ##

[ui::label](ui-label.md)(3), [ui::font](ui-font.md)(3),
[ui::password](ui-password.md)(3)
