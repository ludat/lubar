lubar
=====

My stupid little attempt at learning C better and making a more complete status bar along the way.

**This is not in any way complete. This file is only to order my thoughts as they appear.**

Description
-----------

This is a lighweight status bar which uses cairo and pango to provide full UTF-8 support, images and drawing vectors. **(Nothing of this is implemented yet of course)**

Command line
------------

lubar [OPTIONS]

* -h: get help
* -g **geometry**: Set the geometry for the window (see geometry)
* [--name|-n] **name**: A string to set the name of the window to identify it from other instances
* --border-size **width**: Set the width of the borders (usually 0)
* --border-color **color**: Color for the borders if width is 0 this option is useless
* -fn **font**: Font in pango format (some day I'll find out what it is)
* -fg **color**: Foreground default color (it can be changed later)
* -bg **color**: Background default color (it can be changed later)
* -ax [**center**|**right**|**left**]: Alignment of the X axis, the X geometry will be set using a different reference frame
    * **center**: (0, 0) in the screen will be the center of the screen and (0, 0) for the window will be te center of the window
    * **right**: (0, 0) in the screen will be the far right of the screen and (0, 0) for the window will be te far right of the window
    * **left**: (0, 0) in the screen will be the far left of the screen and (0, 0) for the window will be te far left of the window
* -ay [**center**|**bottom**|**top**]: Alignment of the Y axis, the Y geometry will be set using a different reference frame
    * **center**: (0, 0) in the screen will be the center of the screen and (0, 0) for the window will be te center of the window
    * **bottom**: (0, 0) in the screen will be the bottom of the screen and (0, 0) for the window will be te bottom of the window
    * **top**: (0, 0) in the screen will be the top of the screen and (0, 0) for the window will be te top of the window
