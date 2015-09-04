
/*$Header: /usr8/web/src/RCS/common.h,v 1.24 2015/09/01 17:51:23 leith Exp $*/

#include "std.h"
#include "files.h"
#include "x.h"

#ifndef COMMONH
#define COMMONH

#define MAXCOL   256 
#define MAXCOLSPI 8

// Used for resetting buttons in shift.c
#define TILT	1
#define IMAGE	2
#define MARKER 	3
#define	PIXEL	4
#define	MASK	5

 extern Display        *idispl;           /* current display               */
 extern Screen         *iscreen;          /* current screen                */ 

 extern Widget         iw_top;            /* top widget                    */
 extern Widget         iw_main ;          /* main box widget               */
 extern Widget         iw_menub;          /* menu bar widget               */
 extern Widget         iw_win;            /* image display widget          */
 extern Widget         iw_files;          /* file selection widget         */      
 extern Widget         iw_filesnol;       /* no list file widget           */      
 extern Widget         iw_scroll;         /* scrolled output widget        */
 extern Widget         iw_scrollpar;      /* parent scrolled output widget */

 extern int            idepth;            /* display window depth          */
 extern unsigned int   ihighx, iwidex;    /* display window X & Y dim      */
 extern int            ixulx, iyulx;      /* display window location       */
 extern GC             icontx;            /* usual graphics context        */
 extern Window         iwin, iroot,iwtop; /* image, root & top windows     */
 extern Pixmap         imagsav;           /* image backing store           */
 extern Colormap       map;               /* current colormap              */
 extern Visual         *visual;           /* visual                        */
 extern int            mapgo,mapend;      /* start & end of colormap       */
 extern int            ngo,nend;          /* image start & end in colormap */
 extern int            colorgo,colorend;  /* color start & end in colormap */
 extern int            nmax;              /* normalization range           */
 extern int            imagego;           /* start normalization range     */
 extern int            imageend;          /* end normalization range       */
 extern int            icolor;            /* current color number          */
 extern int            icolorx;           /* current x color number        */
 extern int            icolorb;           /* current background            */
 extern int            icolorxb;          /* current x background          */
 extern int            icolorcf;          /* current curs. foreground      */
 extern int            icolorxcf;         /* current x curs. foreground    */
 extern int            icolorcb;          /* current curs. background      */
 extern int            icolorxcb;         /* current x curs. background    */
 extern int            icolorov;          /* current overlay color         */
 extern int            icolorxov;         /* current x overlay color       */
 extern unsigned long  icolorxmb;         /* current x menu background     */
 extern unsigned long  icolorxmf;         /* current x menu foreground     */
 extern unsigned long  ispicol[MAXCOL];   /* spider color array            */
 extern int            ispicoln[MAXCOL];  /* spider color indices          */
 extern int            ncolors;           /* number of defined colors      */
 extern int            icursor;           /* current spider cursor         */
 extern Cursor         icursorx;          /* current x cursor number       */
 extern XColor         colorcursf;        /* cursor foreground color       */
 extern XColor         colorcursb;        /* cursor background color       */

 extern int            ioper;             /* current operation #           */
    
 extern char           dirnow[81];        /* current directory             */ 
 extern char           lutdir[81];        /* lut directory                 */ 
 extern char           datexc[4];         /* file extension                */
 extern char           filwant[81];       /* file search pattern           */
 extern char           filnow[81];        /* name of last image file       */ 
 extern char           filnext[81];       /* name of next image file       */ 
 extern char           winnam[81];        /* name of last window file      */ 
 extern char           docnam[81];        /* name of last doc file         */ 
 extern FILEDATA *     filedata;          /* current image file info       */
 extern int            ixul,iyul;         /* upper left corner last image  */
 extern int            ixuln,iyuln;       /* corner of next image          */
 extern int            nsam,nrow,nslice;  /* current image size            */
 extern float          scaleval;          /* current scaling value         */
 extern double         fmin_spi,fmax_spi; /* image min & max               */
 extern int            islice;            /* current slice                 */
 extern float          param[102];        /* Spider registers              */

 extern int            nsam1,nsam2;       /* column range                  */
 extern int            nrow1,nrow2;       /* row range                     */
 extern int            nslice1,nslice2;   /* slice range                   */
 extern char           axis;              /* slice axis                    */
 extern int            xaxis,yaxis,zaxis; /* axis selected                 */
 extern int            nprow;             /* pictures / row                */
 extern int            normit;            /* normalize by slice            */

 extern int            ntop,nbot,nrit;    /* top, bottem & rit side        */
 extern int            nstack;		  /* stack file indicator          */
 extern int            inuse;		  /* stacked file inuse            */
 extern int            maxim;		  /* max. imgage in stack file     */
 extern int            imginstack;	  /* stacked image number          */
 extern int	       marker;		  /* set marker flag 	           */
 extern int            marlef,martop;     /* left and top window margins   */
 extern int            marx,mary;         /* x & y image margins           */
 extern int            newline,nuscrn;    /* new line & screen flags       */
 extern int	       ntimg;		  /* start next image (marker      */
 extern int            names;             /* print image names flag        */
 extern int            names_height;      /* pixel height of image filename*/
 extern int            iskip;             /* image mag. factor             */
 extern int            lastskip;          /* last image mag. factor        */
 extern int            movie;             /* image movie flag              */
 extern int            placed;            /* image position sent flag      */
 extern int            force;             /* force images / row flag       */
 extern int            listfi;            /* list files in selector        */
 extern int            trace_web;         /* make a web_results file       */
 extern int            keepxy;            /* retain values of x & y        */
 extern int            gotsurf;           /* surface in memory             */
 extern int            samefile;          /* filnex == filnow              */
 extern int            ixulw,iyulw;       /* window position               */
 extern int            nsamw,nroww;       /* window size                   */
 extern int            leavit,sayit;      /* window data flags             */
 extern int            inimag;            /* inside image data only        */
 extern int            getscreen;         /* get screen pixel value        */
 extern int            getfile;           /* get file pixel value          */
 extern int            docit;             /* store data in doc file        */
 extern int            iradius;           /* location radius               */
 extern int            ikey;              /* doc  file key                 */
 extern int            ikeyc;             /* doc  file key for categorizing*/
 extern int            drawing;           /* flag for xor drawing          */

 extern float          refl, fct;         /* surface rendering param       */
 extern int            shiftimg;          /* shift the image               */
 extern XColor         colort[MAXCOL];    /* temporary color array         */

 extern int            magsizx;           /* magnification size (x)        */
 extern int            magsizy;           /* magnification size (y)        */
 extern int            magfac;            /* magnification factor          */
 extern int            nsam8,nrow8,nslice8; /* size of raw files           */
 extern int	       show_num;	  /* show marker number            */

 extern Boolean        neednewmap;        /* update vmap */

 extern XImage  *      imagel, * imager;  /* image pointers */         
 extern int            nsaml,nrowl,nsamr,nrowr;
 extern int            nsam1l,nrow1l,nsam2l,nrow2l;
 extern int            nsam1r,nrow1r,nsam2r,nrow2r;
 extern int            ixull, iyull, ixulr, iyulr;
 extern int            ixulli,iyulli,ixulri,iyulri;
 extern int            ixullmin,ixlrlmax,iyullmin,iylrlmax;
 extern int            ixulrmin,ixlrrmax,iyulrmin,iylrrmax;

#endif
