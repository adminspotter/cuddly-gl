ui::active
==========

## NAME ##

`ui::active` - CuddlyGL active widget class

## SYNOPSIS ##

```cpp
#include <active.h>

ui::active *a = new ui::active(x, y);
```

## DESCRIPTION ##

The `ui::active` class is an intermediate class which adds event
handling.  It subclasses the `ui::rect` to add *callback lists* and
*timeouts*.

As with the `ui::rect`, the `ui::active` has no interaction with
OpenGL, and can not be drawn on the screen.  It is meant to layer
functionality in a segmented way, and provide a variety of inheritance
points for widgets.

## CALLBACK LISTS ##

A *callback list* is a set of functions which are called in response
to some event.  Events which the `ui::active` recognizes are *enter*,
*leave*, *motion*, *btn_down*, *btn_up*, *key_down*, *key_up*, and
*resize*.

Callback functions receive a *call_data* argument, which contains data
relevant to the individual event.  For example, a *motion* event will
receive the location of the event, and a *btn_down* event will receive
the location, which mouse button was operated, the new state of that
button, and which modifier keys were pressed during the event.  See
the TYPES section for individual event types.

Callback functions also receive a *client_data* argument, which is an
arbitrary piece of data provided when a function is added to a
callback list.

## TIMEOUTS ##

A *timeout* is an event that will occur at some point in the future.
When setting a timeout, the `ui::active` takes the function to be
executed, an argument to pass to the function, and the time delay
until the function is executed.  Timeouts are **not** a hard realtime
mechanism; any true realtime needs should be handled outside of the
CuddlyGL toolkit.

Timeouts are handled internally to each widget, so each `ui::active`
widget or subclass may have its own active timeout running
simultaneously.  However, each widget only has one timeout slot, so
only one timeout may be active for a single widget at any time.
Adding a timeout while another timeout is set will clear the previous
timeout.

When a timeout expires and the function is called, the function and
its argument are cleared.  For a repeating timed event, add a new
timeout at the end of the timeout function.

```cpp
ui::active *a = new ui::active(x, y);
ui::to_until<Gluint, std::milli> expire(500);

a->add_timeout(expire, timeout_func, NULL);

void timeout_func(ui::active *a, void *client_data)
{
    /* Do some operations */

    a->add_timeout(expire, timeout_func, NULL);
}
```

## TYPES ##

* `ui::cb_fptr`

  A function pointer for a callback function.

  ```cpp
  void (*cb_fptr)(ui::active *a, void *call_data, void *client_data)
  ```

* `ui::callback`

  A namespace containing constants for the set of supported callback
  lists.

  | List                   |
  | ---------------------- |
  | ui::callback::enter    |
  | ui::callback::motion   |
  | ui::callback::leave    |
  | ui::callback::btn_down |
  | ui::callback::btn_up   |
  | ui::callback::key_down |
  | ui::callback::key_up   |
  | ui::callback::resize   |

* `ui::to_fptr`

  A function pointer for a timeout function.

  ```cpp
  void (*to_fptr)(ui::active *a, void *client_data)
  ```

* `ui::to_time`
* `ui::to_point`
* `ui::to_until`
* `ui::zero_time`

## METHODS ##

* **add_callback(list, func_ptr, client_data)**

  Adds a callback to the provided `list`.  The `list` argument is an
  element from the `ui::callback` namespace.  The `func_ptr` argument
  is a `ui::cb_fptr`.

  The `client_data` argument is a `void *`, which is saved as-is; if
  the `client_data` is a pointer to something, whatever is at the
  address at call time is what is available to the callback function.

* **remove_callback(list, func_ptr, client_data)**

* **call_callbacks(list, call_data)**

* **add_timeout(until, func_ptr, client_data)**

  Adds a timeout which will expire after the interval `until`.  The
  `until` argument is a `ui::to_time::duration`, which is a type of
  `std::chrono::steady_clock::duration`.  The `func_ptr` argument is a
  `ui::to_fptr`.

  The `client_data` argument is a `void *`, which is saved as-is; if
  the `client_data` is a pointer to something, whatever is at the
  address at call time is what is available to the callback function.

* **remove_timeout()**

* **call_timeout()**

* **get(type, subtype, obj_ptr)**
* **set(type, subtype, obj_ptr)**
* **get_va(type, subtype, obj_ptr, ...)**
* **set_va(type, subtype, obj_ptr, ...)**

  Inherited from [`ui::rect`](ui::rect).

## NEW RESOURCES ##

The `ui::active` adds no new resources which are accessed via the
`get()/set()` and `get_va()/set_va()` methods.

## INHERITED RESOURCES ##

* **ui::element::size** ([`ui::rect`](ui::rect))

  | Subtype              | Data type  |
  | -------------------- | ---------- |
  | **ui::size::width**  | GLuint     |
  | **ui::size::height** | GLuint     |
  | **ui::size::all**    | glm::ivec2 |

## SUBCLASSING ##

The `ui::active` has a virtual inheritance from the `ui::rect`, so the
constructor of each subclass of the `ui::active` must also explicitly
initialize the `ui::rect`.

## RELATED ##

[ui::rect](ui::rect)(3), [ui::widget](ui::widget)(3),
[ui::composite](ui::composite)(3)
