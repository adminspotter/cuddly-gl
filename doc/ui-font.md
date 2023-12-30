# ui::base_font #

## NAME ##

`ui::base_font`, `ui::font`, `ui::font_set` - CuddlyGL font class family

## SYNOPSIS ##

```cpp
#include <cuddly-gl/font.h>

std::u32string howdy = ui::utf8tou32str("howdy!");
ui::font *f = new ui::font(font_fname, size, search_paths);

f->max_cell_size(sizes);

f->get_string_size(howdy, ascender_sz, descender_sz);
f->get_string_size(howdy, ascender_sz, descender_sz, width);

img = f->render_string(howdy, fg_color, bg_color);

#include <functional>

std::u32string bye = ui::utf8tou32str("bye!");
ui::font_set *fs = new ui::font_set(name);

ui::font_set::font_spec spec = std::make_tuple(std::ref(font_fname),
                                               size,
                                               std::ref(search_paths));
fs << spec;

img = fs->render_string(bye, fg_color, bg_color);
```

## DESCRIPTION ##

The `ui::base_font` provides a simple interface for text rendering.
Subclasses `ui::font` and `ui::font_set` provide text rendering
implementations for any of the CuddlyGL widgets which need it.  The
`ui::font` provides rendering with a single font, where the
`ui::font_set` handles multiple fonts in a fallback capacity - the
font files are searched in turn until a glyph is found.

Strings provided to the objects are passed through the Unicode BIDI
algorithm before being rendered; left-to-right strings, right-to-left
strings, and mixed-direction strings will all be rendered correctly.

## TYPES ##

* `ui::glyph`

  A struct which contains the necessary information about a given
  glyph in a font.  The code point, size, bitmap data, and other
  related values are stored within the struct.  The glyph structures
  are managed by an internal cache object.

  In most cases, the `ui::glyph` can be treated as an internal object
  type, and should not need to be used directly.

* `ui::search_paths`

  A `std::vector<std::string>` container.  Each string is a path in
  the filesystem that the file search will traverse in looking for a
  given font file.  The ~ home directory shorthand may be used.

* `ui::font_set::font_spec`

  A `std::tuple<std::string&, int, ui::search_paths&>` tuple.  This
  type is used for injecting fonts into the `ui::font_set`.

## METHODS ##

* **ui::font(std::string& _fname_, int _size_, ui::search_paths& _paths_)**

  Create a `ui::font` object from font file _fname_, with pixel size
  _size_.  The constructor will use the strings in _paths_ to locate
  the font file.

* **operator[](FT_ULong _code_point_)**

  Returns a `ui::glyph` of the given _code_point_.

* **max_cell_size(int& _width_, int& _height_)**
* **max_cell_size(int& _width_, int& _ascender_, int& _descender_)**

  Each font has a maxiumum cell size that we can use when displaying
  text in consistently-sized boxes (e.g. for text entry fields).
  `max_cell_size` returns the _width_, and either the _height_, or the
  _ascender_ and _descender_.

* **get_string_size(const std::u32string& _str_, GLuint& _width_, GLuint& _height_)**
* **get_string_size(const std::u32string& _str_, GLuint& _width_, GLuint& _ascender_, GLuint& _descender_)**
* **get_string_size(const std::vector<bidi::mirror_t>& _str_, GLuint& _width_, GLuint& _ascender_, GLuint& _descender_)**

  Each string has a few size items which can be retrieved.  A _str_
  may be provided as `std::u32string`, or as
  `std::vector<bidi::mirror_t>`.  `get_string_size` returns the
  _width_, and either the _height_, or the _ascender_ and _descender_.

* **render_string(const std::u32string& _str_, const glm::vec4& _fg_color_, const glm::vec4& _bg_color_)**

  _str_ is passed through the Unicode BIDI algorithm and rendered into
  a `ui::image`, which is returned to the caller.

* **ui::font_set(std::string& _name_)**

  Create a `ui::font_set` object with the name _name_.  This name is
  used only by the object's internal cache, when evicting unused
  glyphs.

* **operator<<(ui::font_spec& _spec_)**

  The only way to insert individual font files into the `ui::font_set`
  is via this injection operator.  Each _spec_ is a `std::tuple` of
  filename, size, and search path, as described above.  The object
  will search for glyphs linearly; it will query each font file in
  turn, and stop its search whenever the first instance of a code
  point is found.  Font files are searched in the order in which they
  are injected into the `ui::font_set`.

## RELATED ##

[ui::utf8tou32str](ui-utf8tou32str.md)(3), [ui::image](ui-image)(3),
[ui::label](ui-label.md)(3), [ui::text_field](ui-text_field.md)(3)
