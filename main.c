#include <cairo.h>
#include <cairo-xlib.h>
#include <pango/pangocairo.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZEX 500
#define SIZEY 500

static Display *dpy;
static Window rootwin;
static Window win;
static int scr;
static XSetWindowAttributes wa;

void init_X(){
	if(!(dpy=XOpenDisplay(NULL))) {
		fprintf(stderr, "ERROR: Could not open display\n");
		exit(1);
	}

	scr=DefaultScreen(dpy);
	rootwin=RootWindow(dpy, scr);


	wa.override_redirect = 0;
	wa.background_pixmap = ParentRelative;
	wa.bit_gravity = SouthGravity;
	wa.win_gravity = SouthGravity;
	wa.event_mask = ExposureMask | ButtonReleaseMask | ButtonPressMask | ButtonMotionMask | EnterWindowMask | LeaveWindowMask | KeyPressMask;

	win = XCreateWindow(
			dpy, /* Display */
			rootwin, /* Parent */
			0, /* X position */
			0, /* Y position */
			500, /* width */
			13, /* height */
			1, /* border width */
			DefaultDepth(dpy, scr), /* depth */
			CopyFromParent, class
			DefaultVisual(dpy, scr), /* visual */
			CWOverrideRedirect | CWBackPixmap | CWEventMask, /* value mask */
			&(wa) /* atributes */
		);


	XClassHint *class_hint;
	class_hint = XAllocClassHint();
	class_hint->res_name  = "lubar";
	class_hint->res_class = "Bar";
	XSetClassHint(dpy, win, class_hint);
	XFree(class_hint);
	XSetWindowBorder(dpy, win, 255);
	XStoreName(dpy, win, "my_name");

	Atom type;
	type = XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_DOCK", False);
	XChangeProperty(
			dpy,
			win,
			XInternAtom(dpy, "_NET_WM_WINDOW_TYPE", False),
			XInternAtom(dpy, "ATOM", False),
			32,
			PropModeReplace,
			(unsigned char *)&type,
			1
			);

	type = XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_TOOLBAR", False);
	XChangeProperty(
			dpy,
			win,
			XInternAtom(dpy, "_NET_WM_WINDOW_TYPE", False),
			XInternAtom(dpy, "ATOM", False),
			32,
			PropModeAppend,
			(unsigned char *)&type,
			1
			);

	/* some window managers honor this properties*/
	type = XInternAtom(dpy, "_NET_WM_STATE_ABOVE", False);
	XChangeProperty(
			dpy,
			win,
			XInternAtom(dpy, "_NET_WM_STATE", False),
			XInternAtom(dpy, "ATOM", False),
			32,
			PropModeReplace,
			(unsigned char *)&type,
			1
			);

	type = XInternAtom(dpy, "_NET_WM_STATE_STICKY", False);
	XChangeProperty(
			dpy,
			win,
			XInternAtom(dpy, "_NET_WM_STATE", False),
			XInternAtom(dpy, "ATOM", False),
			32,
			PropModeAppend,
			(unsigned char *)&type,
			1
			);

	unsigned char strut_p[12] = {0, 0, 15, 0, 0, 0, 0, 0, 0, 0, 15, 0};
	XChangeProperty(
			dpy,
			win,
			XInternAtom(dpy, "_NET_WM_STRUT_PARTIAL", False),
			XInternAtom(dpy, "CARDINAL", False),
			32,
			PropModeReplace,
			&strut_p,
			12
			);

	unsigned char strut[4] = { 0, 0, 15, 0 };
	XChangeProperty(
			dpy,
			win,
			XInternAtom(dpy, "_NET_WM_STRUT", False),
			XInternAtom(dpy, "CARDINAL", False),
			32,
			PropModeReplace,
			(unsigned char *)&strut,
			4
			);

	XMapWindow(dpy, win);
}

void paint(cairo_surface_t *cs) {
	cairo_t *c;
	cairo_surface_t *image;

	c=cairo_create(cs);

	image=cairo_image_surface_create_from_png("le.png");
	cairo_set_source_surface(c, image, 0, 0);
	cairo_paint(c);

	/* cairo_move_to(c, 16.0, 11.0); */
	/* cairo_select_font_face (c, "Ubuntu", */
	/* 	CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL); */
	/* cairo_set_font_size (c, 16); */
	/* cairo_set_source_rgb(c, 0.0, 0.0, 0.0); */
	/* cairo_show_text(c, "Hellá World!   "); */
	PangoLayout *layout;
	PangoFontDescription *desc;

	cairo_translate(c, 0, 0);
	layout = pango_cairo_create_layout(c);
	pango_layout_set_text(layout, "Hellá World!   ", -1);
	desc = pango_font_description_from_string("Terminus bold 12");
	pango_layout_set_font_description(layout, desc);
	pango_font_description_free(desc);

	cairo_set_source_rgb(c, 0.0, 0.0, 0.0);
	pango_cairo_update_layout(c, layout);
	pango_cairo_show_layout(c, layout);

	g_object_unref(layout);

	cairo_show_page(c);

	cairo_destroy(c);
}

int main(int argc, char *argv[]) {
	XEvent e;
	cairo_surface_t *cs;

	init_X();

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
