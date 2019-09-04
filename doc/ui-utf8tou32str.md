# ui::utf8tou32str #

## NAME ##

`ui::utf8tou32str`, `ui::u32strtoutf8` - convert between 8-bit UTF-8
strings and 32-bit strings

## SYNOPSIS ##

```cpp
#include <cuddly-gl/font.h>

std::string howdy = "howdy!"
std::u32string howdy32 = utf8tou32str(howdy);

std::string orig = u32strtoutf8(howdy32);
```

## DESCRIPTION ##

The widgets within the CuddlyGL widget set which handle text accept
their inputs as `std::string` in UTF-8 format, but font rendering
classes operate on `std::u32string`, for ease of converting
code-points to glyph indices, and also for simpler Unicode BIDI
handling.

The **ui::utf8tou32str(std::string _str_)** function converts _str_
from UTF-8 `std::string` to 32-bit `std::u32string`.  The
**ui::u32strtoutf8(std::u32string _str_)** function converts _str_
from 32-bit `std::u32string` to UTF-8 `std::string`.

## RETURN VALUES ##

`ui::utf8tou32str()` returns the converted `std::u32string`.

`ui::u32strtoutf8()` returns the converted `std::string`.

## ATTRIBUTES ##

For an explanation of the terms used in this section, see
attributes(7).

  | Interface                              | Attribute     | Value   |
  | -------------------------------------- | ------------- | ------- |
  | ui::utf8tou32str(), ui::u32strtoutf8() | Thread safety | MT-Safe |

## NOTES ##

Both `std::string` and `std::u32string` types are move-assignable and
move-constructible, so there should be no need for explicit moves of
the return values.

## SEE ALSO ##

[`ui::font`](ui-font.md)(3)
