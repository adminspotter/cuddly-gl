ui::radio_box
=============

## NAME ##

`ui::radio_box` - CuddlyGL radio-box template composite class

## SYNOPSIS ##

```cpp
#include <cuddly-gl/radio.h>
#include <cuddly-gl/manager.h>
#include <cuddly-gl/row_column.h>

ui::radio_box<ui::manager> *rb1 = new ui::radio_box<ui::manager>(parent, ...);
ui::radio_box<ui::row_column> *rb2 = new ui::radio_box<ui::row_column>(parent, ...);
```

## DESCRIPTION ##

The `ui::radio_box` template offers specialization for subclasses of
[`ui::composite`](ui-composite.md)(3).  All children of type
[`ui::toggle`](ui-toggle.md)(3) are treated as a *radio group*.  A
radio group can only have a single toggle child checked at any time;
checking one toggle of the group will uncheck all other toggles in
that group.

Querying which child is checked will return a pointer to the object in
question.

```cpp
ui::radio_box<ui::manager> *rb = new ui::radio_box<ui::manager>(...);
ui::toggle *t1 = new ui::toggle(rb, ...);
ui::toggle *t2 = new ui::toggle(rb, ...);

t1->set(ui::element::state, ui::state::checked, true);

ui::toggle *checked_obj;

rb->get(ui::element::child, ui::child::radio, &checked_obj);

/* checked_obj will be t1 */

bool is_checked;

t2->set(ui::element::state, ui::state::checked, true);
t1->get(ui::element::state, ui::state::checked, &is_checked);

/* is_checked will be false */
```

## METHODS ##

All methods correspond to that of the specialized class.

## NEW RESOURCES ##

* **ui::element::child** - checked child for the object.

  | Subtype          | Data type    |
  | ---------------- | ------------ |
  | ui::child::radio | ui::widget * |

## INHERITED RESOURCES ##

All other resources correspond to that of fhe specialized class.

## RELATED ##

[ui::composite](ui-composite.md)(3), [ui::toggle](ui-toggle.md)(3)
