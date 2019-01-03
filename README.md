# cuddly-gl #

[![Build Status](https://travis-ci.org/adminspotter/cuddly-gl.svg?branch=master)](https://travis-ci.org/adminspotter/cuddly-gl) [![codecov](https://codecov.io/gh/adminspotter/cuddly-gl/branch/master/graph/badge.svg)](https://codecov.io/gh/adminspotter/cuddly-gl)

CuddlyGL is a platform-agnostic OpenGL UI toolkit written in C++.

This toolkit was initially written because we weren't quite satisfied
with other GL widget toolkits that were available.  They required a
specific windowing platform, or had inconsistent call structures, or
just didn't make a whole lot of sense to us.  So in our hubris, we
heeded advice to "just write your own, it's not that hard, and you get
exactly what you want!"  We're definitely getting what we want, though
it's turning out to be a little more challenging than we'd expected.

Main design requirements were that there was a consistent way to get
and set resources, a consistent way to manage callback lists,
timeouts, creation, deletion, and pretty much any other operation that
we might need to do on any toolkit widget.

One of our primary design references was the X11 Xt intrinsics of old;
the call interface is very consistent from widget to widget, and all
of the operations are handled in (what we consider to be) very
sensible and predictable ways.

## Current widgets ##

* Labels
  * Text
  * Image
* Buttons
  * Push button
  * Toggle button
* Text fields
  * Text input fields
  * Password fields
* Composite/Manager widgets
  * Row-Column
  * Pie Menus

## Platforms ##

CuddlyGL has been built and tested on

* CentOS Linux 6/7
* Ubuntu GNU/Linux Trusty Tahr and Xenial Xerus (via [Travis CI](https://travis-ci.org))
* MacOS X Mavericks - ElCapitan

As long as you've got a new-enough GL stack, and a POSIX-y system with
the standard GL and C libraries and headers, you should be in pretty
good shape.

## Build requirements ##

* A C++11 compiler
* OpenGL 3.2 support
* The GNU autotools
* [glm](http://glm.g-truc.net/)

## Contributing ##

We love contributions!  Feel free to submit a pull request, or start
some discussion in an issue.

Try to keep things within 80 columns.  Try to keep individual commits
under 50-ish lines changed.  Try to keep functions short and to the
point - 50 lines is probably a good rule of thumb there too.  Compact
and easily-understandable are our big watch-words.

Pull requests should not be squashed, and should follow a logical flow
from commit to commit.  Commit messages should actually discuss what's
being changed, and if there's a relevant issue or other relevant
commit, tag it into the commit message.  Add yourself to the AUTHORS
file, and the comment block at the top of files you change.

Code is licensed GPL2.
