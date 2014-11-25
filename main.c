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
static int geometry[4] = {0, 0, 500, 15};
static int border_size = 0;
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
	wa.bit_gravity = SouthGravity;
	wa.win_gravity = SouthGravity;
	wa.event_mask = ExposureMask | ButtonReleaseMask | ButtonPressMask | ButtonMotionMask | EnterWindowMask | LeaveWindowMask | KeyPressMask;

	win = XCreateWindow(
			dpy, /* Display */
			rootwin, /* Parent */
			geometry[3], /* X position */
			geometry[4], /* Y position */
			geometry[0], /* width */
			geometry[1], /* height */
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
	XSetWindowBorder(dpy, win, 255);
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
			(unsigned char *)&strut_p,
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

int set_geometry(char *str, int *geom){
	char tmp[16];
	char delimiters[4] = { 'x', '+', '+', 0};
	char *d = delimiters;
	int i;
	for(i = 0;i<4;i++){
		int j = 0;
		while(*str!=*d){
			tmp[j] = *str;
			str++; j++;
		}
		tmp[j] = 0;
		geom[i] = atoi(tmp);
		str++; d++;
	}
	fprintf(stderr, "width: %d\n", geometry[0]);
	fprintf(stderr, "height: %d\n", geometry[1]);
	fprintf(stderr, "x: %d\n", geometry[2]);
	fprintf(stderr, "y: %d\n", geometry[3]);
	return 0;
}

int main(int argc, char *argv[]) {
	char ch;
	/* opterr = 0; */
	while((ch = getopt(argc, argv, "hg:n:")) != -1){
		switch (ch){
		case 'h':
			printf("Nobody can help you\n");
			exit(EXIT_SUCCESS);
			break;
		case 'g':
			if(set_geometry(optarg, geometry)){
				fprintf(stderr, "Bad geometry\n");
				exit(EXIT_SUCCESS);
			}else{
				fprintf(stderr, "Geometry accepted\n");
			}
			break;
		case 'n':
			if(strlen(optarg)>64){
				fprintf(stderr, "Name too long\n");
				exit(EXIT_SUCCESS);
			}else{
				strcpy(name, optarg);
			}
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
			geometry[0],
			geometry[1]
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
