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

struct XGlobals {
	Display *dpy;
	Window rootwin;
	Window win;
	int scr;
	XSetWindowAttributes wa;
} XG;

void init_X(){
	if(!(XG.dpy=XOpenDisplay(NULL))) {
		fprintf(stderr, "ERROR: Could not open display\n");
		exit(1);
	}

	XG.scr=DefaultScreen(XG.dpy);
	XG.rootwin=RootWindow(XG.dpy, XG.scr);


	XG.wa.override_redirect = 0;
	XG.wa.background_pixmap = ParentRelative;
	XG.wa.event_mask = ExposureMask | ButtonReleaseMask | ButtonPressMask | ButtonMotionMask | EnterWindowMask | LeaveWindowMask | KeyPressMask;

	XG.win = XCreateWindow(
			XG.dpy, //Display
			XG.rootwin, //Parent
			10, //X position
			10, //Y position
			500, //width
			50, //height
			0, //border width
			DefaultDepth(XG.dpy, XG.scr), //depth
			CopyFromParent, //class
			DefaultVisual(XG.dpy, XG.scr), //visual
			CWOverrideRedirect | CWBackPixmap | CWEventMask, //value mask
			&(XG.wa) //atributes
		);

	XStoreName(XG.dpy, XG.win, "hello");
	XMapWindow(XG.dpy, XG.win);
}

void paint(cairo_surface_t *cs) {
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

int main(int argc, char *argv[]) {
	XEvent e;
	cairo_surface_t *cs;

	init_X();

	if(!(XG.dpy=XOpenDisplay(NULL))) {
		fprintf(stderr, "ERROR: Could not open display\n");
		exit(1);
	}

	XG.scr=DefaultScreen(XG.dpy);
	XG.rootwin=RootWindow(XG.dpy, XG.scr);


	XG.wa.override_redirect = 0;
	XG.wa.background_pixmap = ParentRelative;
	XG.wa.event_mask = ExposureMask | ButtonReleaseMask | ButtonPressMask | ButtonMotionMask | EnterWindowMask | LeaveWindowMask | KeyPressMask;

	XG.win = XCreateWindow(
			XG.dpy, //Display
			XG.rootwin, //Parent
			10, //X position
			10, //Y position
			500, //width
			50, //height
			0, //border width
			DefaultDepth(XG.dpy, XG.scr), //depth
			CopyFromParent, //class
			DefaultVisual(XG.dpy, XG.scr), //visual
			CWOverrideRedirect | CWBackPixmap | CWEventMask, //value mask
			&(XG.wa) //atributes
		);

	XStoreName(XG.dpy, XG.win, "hello");
	XMapWindow(XG.dpy, XG.win);

	cs=cairo_xlib_surface_create(XG.dpy, XG.win, DefaultVisual(XG.dpy, 0), SIZEX, SIZEY);

	while(1) {
		XNextEvent(XG.dpy, &e);
		if(e.type==Expose && e.xexpose.count<1) {
			paint(cs);
		} else if(e.type==ButtonPress) break;
	}

	cairo_surface_destroy(cs);
	XCloseDisplay(XG.dpy);
    return 0;
}
