ui::rect
========

## NAME ##

`ui::rect` - CuddlyGL rectangle base class

## SYNOPSIS ##

```c++
#include <rect.h>

ui::rect *r = new ui::rect(x, y);

r->set(ui::element::size, ui::size::width, &different_x);
r->set_va(ui::element::size, ui::size::width, &other_x,
          ui::element::size, ui::size::height, &other_y, 0);

r->get(ui::element::size, ui::size::all, &xy_vector);
r->get_va(ui::element::size, ui::size::width, &x_target,
          ui::element::size, ui::size::height, &y_target, 0);
```

## DESCRIPTION ##

The `ui::rect` class is the base of all widgets in the CuddlyGL widget
set.  It has a width and a height.  It also contains the base `get()`
and `set()` methods, along with the variable-argument `get_va()` and
`set_va()` methods.

The `ui::rect` has no methods or facilities which interact with
OpenGL, and can not be drawn on the screen in any way; it is strictly
a base class with a size.

## METHODS ##

* **get(type, subtype, obj_ptr)**

  Retrieve resources from the object.  The `type` argument will be
  from the `ui::element` namespace, and the `subtype` argument will be
  from a namespace which corresponds to the primary type.  The
  `obj_ptr` argument is a `void *`.

* **set(type, subtype, obj_ptr)**

  Set resources within the object.  As with the `get()` method, the
  `type` argument is an item from the `ui::element` namespace, and the
  `subtype` is from the corresponding sub-namespace.  The `obj_ptr` is
  also a `void *`.

* **get_va(type, subtype, obj_ptr, ...)**

  A variable-argument version of the `get()` method.  Argument list is
  sets of the three regular `get()` arguments, terminated with an
  additional 0.

* **set_va(type, subtype, obj_ptr, ...)**

  A variable-argument version of the `set()` method.  Argument list is
  sets of the three regular `set()` arguments, terminated with an
  additional 0.

The variable-argument methods should not need to be overridden by any
subclasses; they call out to the corresponding `get()`/`set()`
methods.

## NEW RESOURCES ##

* **ui::element::size** - size of the object.

  | Subtype              | Data type  | Notes                                        |
  | -------------------- | ---------- | -------------------------------------------- |
  | **ui::size::width**  | GLuint     |                                              |
  | **ui::size::height** | GLuint     |                                              |
  | **ui::size::all**    | glm::ivec2 | Set both width and height parameters at once |

## RELATED ##

[ui::active](ui::active)(3)
