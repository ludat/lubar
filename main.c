#include <cairo.h>
#include <cairo-xlib.h>
#include <pango/pangocairo.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Display *dpy;
static Window rootwin;
static Window win;
static int scr;
static XSetWindowAttributes wa;

static char name[64] = "lololo";
/* Geometry array width, height, x, y*/
static struct {
	int x;
	int y;
	unsigned int width;
	unsigned int height;
} geometry = {0, 0, 500, 15};
static int border_size = 0;
static unsigned long border_color = 0;
/* static char font[64]; */
/* static char background_color; */
/* static char foreground_color; */

void init_X_win(){
	if(!(dpy=XOpenDisplay(NULL))) {
		fprintf(stderr, "ERROR: Could not open display\n");
		exit(1);
	}

	scr=DefaultScreen(dpy);
	rootwin=RootWindow(dpy, scr);

	wa.override_redirect = 0;
	wa.background_pixmap = ParentRelative;
	wa.event_mask = ExposureMask | ButtonReleaseMask | ButtonPressMask | ButtonMotionMask | EnterWindowMask | LeaveWindowMask | KeyPressMask;

	win = XCreateWindow(
			dpy, /* Display */
			rootwin, /* Parent */
			geometry.x, /* X position */
			geometry.y, /* Y position */
			geometry.width, /* width */
			geometry.height, /* height */
			border_size, /* border width */
			DefaultDepth(dpy, scr), /* depth */
			CopyFromParent, /* class */
			DefaultVisual(dpy, scr), /* visual */
			CWOverrideRedirect | CWBackPixmap | CWEventMask, /* value mask */
			&(wa) /* atributes */
		);


	XClassHint *class_hint;
	class_hint = XAllocClassHint();
	class_hint->res_name  = "lubar";
	class_hint->res_class = "Lubar";
	XSetClassHint(dpy, win, class_hint);
	XFree(class_hint);
	XSetWindowBorder(dpy, win, border_color);
	XStoreName(dpy, win, name);

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

	unsigned long strut_p[12] = {0, 0, 25, 0, 0, 0, 0, 0, 0, 1365, 0, 0};
	XChangeProperty(
			dpy,
			win,
			XInternAtom(dpy, "_NET_WM_STRUT_PARTIAL", False),
			XInternAtom(dpy, "CARDINAL", False),
			32,
			PropModeReplace,
			(unsigned char *)&strut_p,
			12
			);

	unsigned long strut[4] = { 0, 0, 25, 0 };
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

	XMoveWindow(dpy, win, geometry.x,geometry.y);

	XMapWindow(dpy, win);
}

void paint(cairo_surface_t *cs) {
	cairo_t *cr;

	cr=cairo_create(cs);

	cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
	cairo_rectangle (cr, 0, 0, geometry.width, geometry.height);
	cairo_fill (cr);
	/* cairo_surface_t *image; */
	/* image=cairo_image_surface_create_from_png("le.png"); */
	/* cairo_set_source_surface(c, image, 0, 0); */
	/* cairo_paint(c); */

	/* cairo_move_to(c, 16.0, 11.0); */
	/* cairo_select_font_face (c, "Ubuntu", */
	/* 	CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL); */
	/* cairo_set_font_size (c, 16); */
	/* cairo_set_source_rgb(c, 0.0, 0.0, 0.0); */
	/* cairo_show_text(c, "Hellá World!   "); */
	PangoLayout *layout;
	PangoFontDescription *desc;

	cairo_translate(cr, 0, 0);
	layout = pango_cairo_create_layout(cr);
	pango_layout_set_text(layout, "jgqHellá World!   ", -1);
	desc = pango_font_description_from_string("Terminus bold 8");
	pango_layout_set_font_description(layout, desc);
	pango_font_description_free(desc);

	cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
	pango_cairo_update_layout(cr, layout);
	pango_cairo_show_layout(cr, layout);

	g_object_unref(layout);

	cairo_show_page(cr);

	cairo_destroy(cr);
}

int main(int argc, char *argv[]) {
	char ch;
	/* opterr = 0; */
	while((ch = getopt(argc, argv, "hg:n:b:c:")) != -1){
		switch (ch){
		case 'h':
			printf("Nobody can help you\n");
			exit(EXIT_SUCCESS);
			break;
		case 'g':
			XParseGeometry(optarg, &(geometry.x),
					&(geometry.y), &(geometry.width), &(geometry.height));
			break;
		case 'b':
			border_size = atoi(optarg);
			break;
		case 'c':
			border_color = strtoul(optarg, NULL, 16);
			break;
		case 'n':
			strncpy(name, optarg, 63);
			break;
		case '?':
			exit(EXIT_SUCCESS);
		}
	}

	init_X_win();

	cairo_surface_t *cs;
	cs=cairo_xlib_surface_create(
			dpy,
			win,
			DefaultVisual(dpy, 0),
			geometry.width,
			geometry.height
		);

	XEvent e;
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
