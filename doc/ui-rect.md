ui::rect
========

## NAME ##

`ui::rect` - CuddlyGL rectangle base class

## SYNOPSIS ##

```c++
#include <rect.h>

ui::rect *r = new ui::rect(x, y);

r->set(ui::element::size, ui::size::width, different_x);
r->set(ui::element::size, ui::size::width, other_x,
       ui::element::size, ui::size::height, other_y);

r->get(ui::element::size, ui::size::all, &xy_vector);
r->get(ui::element::size, ui::size::width, &x_target,
       ui::element::size, ui::size::height, &y_target);
```

## DESCRIPTION ##

The `ui::rect` class is the base of all widgets in the CuddlyGL widget
set.  It has a width and a height.  It also contains the `get()` and
`set()` methods, which function in a variable-argument context.

The `ui::rect` has no methods or facilities which interact with
OpenGL, and can not be drawn on the screen in any way; it is strictly
a base class with a size.

## METHODS ##

* **get(GLuint _type_, GLuint _subtype_, _res_type_ \*_obj_ptr_, ...) const**

  Retrieve resources from the object.  Arguments are given in sets of
  three.  The _type_ argument will be from the `ui::element`
  namespace, and the _subtype_ argument will be from a namespace which
  corresponds to the primary type.  The _obj_ptr_ argument is a
  pointer to a type which corresponds with the resource being set; all
  `ui::rect` resources are either `GLuint` or `glm::ivec2`.  The
  method can take any number of sets of these three arguments.

* **set(GLuint _type_, GLuint _subtype_, _res_type_ _obj_, ...)**

  Set resources within the object.  As with the `get()` method,
  arguments are provided in sets of three, with the _type_ argument
  being an item from the `ui::element` namespace, the _subtype_ from
  the corresponding sub-namespace, and the _obj_ a variable or
  constant which is relevant for the type/subtype.  This method can
  take any number of three-argument sets.

The variable-argument methods need to be added to each subclass which
overrides either the get() or set() method.  They are added by
preprocessor macros within `ui_defs.h`:  `GET_VA` and `SET_VA`.

## NEW RESOURCES ##

* **ui::element::size** - size of the object.

  | Subtype              | Data type  | Notes                                        |
  | -------------------- | ---------- | -------------------------------------------- |
  | **ui::size::width**  | GLuint     |                                              |
  | **ui::size::height** | GLuint     |                                              |
  | **ui::size::all**    | glm::ivec2 | Set both width and height parameters at once |

## RELATED ##

[ui::active](ui-active.md)(3)
