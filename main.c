/* The authors of this work have released all rights to it and placed it
in the public domain under the Creative Commons CC0 1.0 waiver
(http://creativecommons.org/publicdomain/zero/1.0/).

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Retrieved from: http://en.literateprograms.org/Hello_World_(C,_Cairo)?oldid=10388
*/


#include<cairo.h>
#include<cairo-pdf.h>
#include<cairo-ps.h>
#include<cairo-xlib.h>
#include<X11/Xlib.h>

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define SIZEX 500
#define SIZEY 500


void paint(cairo_surface_t *cs)
{
	cairo_t *c;
	cairo_surface_t *image;

	c=cairo_create(cs);

	image=cairo_image_surface_create_from_png("le.png");
	cairo_set_source_surface(c, image, 0, 0);
	printf("width: %d\n", cairo_image_surface_get_width (image));
	printf("height: %d\n", cairo_image_surface_get_height (image));
	cairo_paint(c);

	/* cairo_rectangle(c, 0.0, 0.0, SIZEX, SIZEY); */
	/* cairo_set_source_rgb(c, 0.0, 0.0, 0.5); */
	/* cairo_fill(c); */

	cairo_move_to(c, 16.0, 11.0);
	cairo_select_font_face (c, "Terminus",
		CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size (c, 16);
	cairo_set_source_rgb(c, 0.0, 0.0, 0.0);
	cairo_show_text(c, "Hello World!");
	cairo_show_page(c);

	cairo_destroy(c);
}

int main(int argc, char *argv[])
{
	Display *dpy;
	Window rootwin;
	Window win;
	XEvent e;
	int scr;
	cairo_surface_t *cs;

	if(!(dpy=XOpenDisplay(NULL))) {
		fprintf(stderr, "ERROR: Could not open display\n");
		exit(1);
	}

	scr=DefaultScreen(dpy);
	rootwin=RootWindow(dpy, scr);

	win=XCreateSimpleWindow(dpy, rootwin, 1, 1, SIZEX, SIZEY, 0, 
			BlackPixel(dpy, scr), BlackPixel(dpy, scr));

	XStoreName(dpy, win, "hello");
	XSelectInput(dpy, win, ExposureMask|ButtonPressMask);
	XMapWindow(dpy, win);

	cs=cairo_xlib_surface_create(dpy, win, DefaultVisual(dpy, 0), SIZEX, SIZEY);

	while(1) {
		XNextEvent(dpy, &e);
		if(e.type==Expose && e.xexpose.count<1) {
			paint(cs);
		} else if(e.type==ButtonPress) break;
	}

	cairo_surface_destroy(cs);
	XCloseDisplay(dpy);
    return 0;
}
