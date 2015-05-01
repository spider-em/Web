/* Standard header file for file header data*/

#ifndef FILESH
#define FILESH

#include "std.h"
#include <X11/Xlib.h>

 typedef struct {
        FILE   *fp;
        int    offset;

        int    nslice;
        int    nsam;
        int    nrow;
        int    totrec;
        int    nlabel;
        int    iform;
        int    imami;
        double fmax_spi;
        double fmin_spi;
        double av;
        double sig;
        int    flip;
        int    headrec;
        int    iangle;
        double phi;
        double theta;
        double gamma;
        int    headbyt;
        int    reclen;
        int    nstack;
        int    maxim;
        int    inuse;
        int    imginstack;
        char   date[12];
        char   time[9];
        char   title[160];
        char   filenam[80];
        float  *buf;
        XImage *ximage;
        char   *cimage;
 } FILEDATA;
#endif
