ui::active
==========

## NAME ##

`ui::active` - CuddlyGL active widget class

## SYNOPSIS ##

```cpp
#include <active.h>

ui::active *a = new ui::active(x, y);

a->add_callback(ui::callback::enter, enter_callback, NULL);
a->add_callback(ui::callback::leave, leave_callback, (void *)5);

a->call_callbacks(ui::callback::btn_down, btn_call_data);

a->remove_callback(ui::callback::enter, enter_callback, NULL);

ui::to_until<GLuint, std::milli> expire(500);

a->add_timeout(expire, timeout_callback, NULL);

a->call_timeout();

a->add_timeout(expire, timeout_callback, NULL);
a->remove_timeout();
```

## DESCRIPTION ##

The `ui::active` class is an intermediate class which adds event
handling.  It subclasses the `ui::rect` to add *callback lists* and
*timeouts*.

As with the [`ui::rect`](ui-rect.md)(3), the `ui::active` has no
interaction with OpenGL, and can not be drawn on the screen.  It is
meant to layer functionality in a segmented way, and provide a variety
of inheritance points for widgets.

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
callback list.  The argument is kept in the list with the function
pointer, and is passed verbatim to the callback function.  If the
argument is a pointer itself, whatever is referenced by the pointer
will be available at invocation time.

Callback lists may contain any number of callbacks; the functions are
called in order of addition.  A callback list may contain multiple
invocations of the same function, with varying, or even the same,
*client_data* arguments.

## CALLBACK TYPES ##

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

* `ui::mouse_call_data`

  The call-data structure which is passed to *enter*, *motion*, and
  *leave* callback functions.

  ```cpp
  struct {
      glm::ivec2 location;
  }
  ```

* `ui::btn_call_data`

  The call-data structure which is passed to *btn_down* and *btn_up*
  callback functions.

  ```cpp
  struct {
      glm::ivec2 location;
      GLuint button;
      GLuint state;
      GLuint mods;
  }
  ```

  The `button` field will contain a constant from the `ui::mouse`
  namespace.  The state field will contain either `ui::mouse::down` or
  `ui::mouse::up`.  The `mods` field will contain a bitmask of
  constants from `ui::key_mod`.

* `ui::key_call_data`

  The call-data structure which is passed to *key_down* and *key_up*
  callback functions.

  ```cpp
  struct {
      glm::ivec2 location;
      uint32_t character;
      GLuint key;
      GLuint state;
      GLuint mods;
  }
  ```

  The `character` field will contain the textual character which is
  associated with the event, where the `key` field will contain a
  constant from the `ui::key` namespace.  If the event does have a
  textual character, the `key` field will contain `ui::key::no_key`.
  The `state` field will contain either `ui::key::down` or
  `ui::key::up`.  The `mods` field will contain a bitmask of constants
  from `ui::key_mod`.

* `ui::resize_call_data`

  The call-data structure which is passed to *resize* callback
  functions.

  ```cpp
  struct {
      glm::ivec2 new_size;
  }
  ```

  The *resize* callbacks are mostly meant for handling resizes in the
  top-level UI context, but could be used in other widgets.

## TIMEOUTS ##

A *timeout* is an event that will occur after some interval.  When
setting a timeout, the `ui::active` takes the function to be executed,
an argument to pass to the function, and the time delay until the
function is executed.  Timeouts are **not** a hard realtime mechanism;
any true realtime needs should be handled outside of the CuddlyGL
toolkit.  Timeouts are also **not** alarms.  They cause widgets to
perform some action after an interval.  Alarms which go off at some
arbitrary local time should also be handled outside the toolkit.

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
ui::to_until<GLuint, std::milli> expire(500);

a->add_timeout(expire, timeout_func, NULL);

void timeout_func(ui::active *a, void *client_data)
{
    /* Do some operations */

    a->add_timeout(expire, timeout_func, NULL);
}
```

## TIMEOUT TYPES ##

* `ui::to_fptr`

  A function pointer for a timeout function.

  ```cpp
  void (*to_fptr)(ui::active *a, void *client_data)
  ```

* `ui::to_time`

  A typedef of `std::chrono::steady_clock`.  This is a monotonic clock
  which will never be adjusted (for daylight savings, leap seconds,
  etc.), so we can count on it being consistent, and providing us with
  accurate intervals.

* `ui::to_point`

  A typedef of `std::chrono::time_point<std::chrono::steady_clock>`.
  This is used internally to keep track of the expiration time of a
  timeout.

* `ui::to_until`

  A typedef of `std::chrono::steady_clock::duration`.  It uses all the
  same helper types (`std::chrono::milliseconds`, etc.), templates,
  and operators as the underlying classes.  This type is used in the
  time argument for setting a timeout.

* `ui::zero_time`

  A constant which represents epoch time 0.  The `ui::active` uses
  this constant internally to denote that a timeout is not set.

Most of the timeout-related typedefs are an effort to conquer the
excessive verbosity of the `std::chrono` library.  They are all direct
mappings to the underlying types, but with shorter names.

## METHODS ##

* **add_callback(GLuint _list_, ui::cb_fptr _func_ptr_, void \*_client_data_)**

  Adds a callback to the provided _list_.  The _list_ argument is an
  element from the `ui::callback` namespace.

  The _client_data_ argument is saved as-is; if _client_data_ is a
  pointer to something, whatever is at the address at call time is
  what is available to the callback function.

* **remove_callback(GLuint _list_, ui::cbfptr _func_ptr_, void \*_client_data_)**

  Removes a callback to the provided _list_ which has the provided
  _client_data_.

  If a given _func_ptr_/_client_data_ pair are contained in a callback
  list more than once, a call to `remove_callback` will remove the
  *first* instance.

  If no matching callback is found in a list, the list will be
  unchanged.

* **call_callbacks(GLuint _list_, void \*_call_data_)**

  Calls the callbacks in the provided _list_, passing _call_data_ in
  the second argument to each.  The _list_ argument is an element from
  the `ui::callback` namespace.

  This is the method which is used internally by the event-handling
  mechanism to trigger a set of callbacks.

* **add_timeout(ui::to_time::duration _until_, ui::to_fptr _func_ptr_, void \*_client_data_)**

  Adds a timeout which will expire after the interval _until_.  The
  _until_ argument is a type of `std::chrono::steady_clock::duration`,
  so all related types are usable.

  The _client_data_ argument is saved as-is; if _client_data_ is a
  pointer to something, whatever is at the address at call time is
  what is available to the callback function.

* **remove_timeout()**

  Removes a pending timeout.  The function, client data, and time
  interval are all cleared, and no function call is made.

* **call_timeout()**

  Calls the pending timeout.  The timeout data is cleared and the
  function call to the configured callback is made.

  This is the actual method that the timeout mechanism uses internally
  to fire the timeout call, but in the general case, this method would
  probably not be used very often in user code.

Inherited from [`ui::rect`](ui-rect.md):

* **get(GLuint _type_, GLuint _subtype_, _res_type_ \*_obj_ptr_, ...) const**
* **set(GLuint _type_, GLuint _subtype_, _res_type_ _obj_, ...)**

## NEW RESOURCES ##

The `ui::active` adds no new resources which are accessed via the
`get()/set()` methods.

## INHERITED RESOURCES ##

* **ui::element::size** ([`ui::rect`](ui-rect.md))

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

[ui::rect](ui-rect.md)(3), [ui::widget](ui-widget.md)(3),
[ui::composite](ui-composite.md)(3)
