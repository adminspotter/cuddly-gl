ui::template
============

## NAME ##

`ui::template` - CuddlyGL manpage template class

## SYNOPSIS ##

<!-- Show each *new* method that the class provides. -->

```cpp
#include <cuddly-gl/template.h>

ui::template *t = new ui::template();

t->method_name(a, b, c);
```

## DESCRIPTION ##

<!-- Overview of what is added in this class.  If larger concepts are
introduced, add headings directly below to discuss each concept in
detail.  Include code snippets in additional headings where
relevant. -->

## TYPES ##

<!-- Optional section.  If the class has new types or special
namespaces, describe them here.  New resource namespaces should be
described in the NEW RESOURCES section, not here. -->

## METHODS ##

<!-- Only describe the public call interface.  Protected or private
methods should not be listed. -->

* **method_name(type1 _arg1_, type2 _arg2_, type3 _arg3_)**

  Description of the method and its arguments.

<!-- Group methods inherited from each superclass together, and link
to that manual page.  Links should go to where the methods are
*defined*, rather than immediate superclasses (where different). -->

* **inherited_one()**
* **inherited_two()**
* **inherited_three()**

  Inherited from [ui::super](ui-super.md).

## NEW RESOURCES ##

* **ui::element::foo** - foo of the object

  | Subtype      | Data type | Notes           |
  | ------------ | --------- | --------------- |
  | ui::foo::bar | GLuint    | Optional column |

## INHERITED RESOURCES ##

* **ui::element::inherited** ([ui::super](ui-super.md))

  | Subtype            | Data type | Class (optional)         |
  | ------------------ | --------- | ------------------------ |
  | ui::inherited::foo | GLuint    | [ui::super](ui-super.md) |
  | ui::inherited::bar | GLuint    |                          |

* **ui::element::other** ([ui::other](ui-other.md))

  | Subtype        | Data type | Class (optional)         |
  | -------------- | --------- | ------------------------ |
  | ui::other::foo | GLuint    | [ui::other](ui-other.md) |
  | ui::other::bar | GLuint    |                          |

<!-- If the entire type and all subtypes are inherited from a single
class, link the class after the type name and omit the Class column
from the table.  Otherwise, omit the class following the type name,
add the Class column to the subtype table, and link to the appropriate
class for each subtype. -->

## SUBCLASSING ##

<!-- Optional section.  Any particular instructions on special
subclassing needs for the class. -->

## RELATED ##

<!-- Links to direct parent classes and subclasses of the class.  Each
link should be followed with the parenthesized section number of the
other page; programming libraries are always in section 3.  Links
should be separated by commas. -->

[ui::super](ui-super.md)(3), [ui::subclass](ui-subclass.md)(3)
