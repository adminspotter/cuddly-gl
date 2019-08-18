ui::vertex_buffer
=================

## NAME ##

`ui::vertex_buffer` - CuddlyGL vertex buffer class

## SYNOPSIS ##

```c++
#include <widget.h>

ui::vertex_buffer *b = new ui::vertex_buffer();

b->generate_box(upper_left, lower_right, color);

b->generate_ellipse(center, radii, inner_pct, segments, color);

b->generate_ellipse_divider(center, radii, inner_pct, angle, color);

size_t vert_sz = b->vertex_size();
const float *verts = b->vertex_data();

size_t elt_sz = b->element_size();
const GLuint *elts = b->element_data();
GLuint elt_count = b->element_count();
```

## DESCRIPTION ##

The `ui::vertex_buffer` class adds convenience functions around
drawing boxes and divided ellipses within OpenGL vertex buffers.  All
coordinate arguments to the object will be in OpenGL screen
coordinates, rather than pixel coordinates.

OpenGL provides a number of different buffer types for passing
information into the memory of the video device, and the
`ui::vertex_buffer` provides the two that CuddlyGL uses:  the vertex
buffer and the element buffer.  In order to save vertex memory, we try
to reuse existing vertices as much as possible by using an element
buffer.  All of the OpenGL primitives we use are triangles, and most
of the triangles are packed together, so we do have good opportunity
to reuse vertex data.

Each coordinate within the vertex buffer consists of 8 `float` values:

<table>
  <tr>
    <td colspan="2">Position</td>
    <td colspan="4">Color</td>
    <td colspan="2">Texture</td>
  </tr>
  <tr>
    <td>x</td>
    <td>y</td>
    <td>r</td>
    <td>g</td>
    <td>b</td>
    <td>a</td>
    <td>u</td>
    <td>v</td>
  </tr>
</table>

The element buffer contains indices of items within the vertex buffer.
Each box or ellipse segment requires 6 `GLuint` element values, 3 for
each triangle.  A `0` element would correspond with `vertex[0]`, `1`
would correspond with `vertex[8]`, and so on.

## CONSTRUCTION ##

* **vertex_buffer()**

  Creates a vertex buffer.

## METHODS ##

* **void generate_box(glm::vec2 _upper_left_, glm::vec2 _lower_right_, const glm::vec4& _color_)**

  Add the vertices and elements for a box in the object.  Each box
  consists of 32 vertex items (4 coordinates) and 6 element items.

  Boxes are axis-aligned, between the _upper_left_ point and the
  _lower_right_ point.  These points are in OpenGL floating-point
  screen coordinates.  _color_ is an RGBA tuple.

* **void generate_ellipse(glm::vec2 _center_, glm::vec2 _radius_, float _inner_pct_, int _segments_, const glm::vec4& _color_)**

  Add the vertices and elements for an ellipse in the object.  Each
  segment of the ellipse consists of 16 vertex items (2 coordinates)
  and 6 element items.

  _center_ and _radii_ are in OpenGL floating-point screen
  coordinates.  _center_ is the center of the ellipse, and _radii_
  contains the horizontal and vertical radii of the ellipse,
  respectively.  These ellipses are generated with a hole in the
  middle, which is placed at _inner_pct_ percent of the total radius;
  0.0f will generate a completely filled ellipse, and 0.99f will
  generate a narrow elliptical line.  Any values outside of
  [0.0, 0.99] will be clamped.  _segments_ is the number of
  wedge-shaped segments which are used to draw the ellipse.  _color_
  is an RGBA tuple, as with `generate_box`.

* **void generate_ellipse_divider(glm::vec2 _center_, glm::vec2 _radius_, float _inner_pct_, float _angle_, const glm::vec4& _color_)**

  Add the vertices and elements for an ellipse divider to the object.
  Each divider consists of 32 vertex items (4 coordinates) and 6
  element items.

  _center_, _radii_, _inner_pct_, and _color_ are exactly as
  `generate_ellipse` above.  _angle_ is the angle in degrees where the
  divider will be located.  0 is along the positive x-axis, and moves
  clockwise as the angle increases.

* **const float \*vertex_data(void)**

  Return a vertex data array suitable for passing into the
  `glBufferData` function.

* **size_t vertex_size(void)**

  Return the size in bytes of the vertex buffer items which have been
  added via the generation methods above.  The `glBufferData` function
  requires this size directly.

* **const float \*element_data(void)**

  Return an element data array suitable for passing into the
  `glBufferData` function.

* **size_t element_size(void)**

  Return the size in bytes of the element buffer items which have been
  added via the generation methods above.  The `glBufferData` function
  requires this size directly.

* **GLuint element_count(void)**

  Return the number of elements in the element buffer.

## CONSTANTS ##

* **ui::vertex_buffer::no_texture**

  A named constant which will represent a vertex item without any
  mapped texture.  By default all vertices are created without
  texture; the caller must modify the vertex array manually if a
  texture map is required for some piece of the generated primitives.

## BUGS ##

The texture mapping primitive is entirely manual, and relies
completely on the caller knowing the internal structure of the vertex
array.

## RELATED ##

[ui::widget](ui-widget.md)(3)
