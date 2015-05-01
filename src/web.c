/*
 C++********************************************************************
 C                                                                     *
 C    web.c -- C language version               May   92  ArDean Leith *
 C             Web_true                         Sep 2011  ArDean Leith *
 C             nsam8                            Nov 2012  ArDean Leith *
 C                                                                     *
 C *********************************************************************
 C=* PACKAGE AUTHOR: A. LEITH                                          *
 C=* FROM: WEB - VISUALIZER FOR SPIDER MODULAR IMAGE PROCESSING SYSTEM *
 C=* Copyright 1985-2012  Health Research Inc.,                        *
 C=* Riverview Center, 150 Broadway, Suite 560, Menands, NY 12204.     *
 C=* Email:  spider@wadsworth.org                                      *
 C=*                                                                   *
 C=* This program is free software; you can redistribute it and/or     *
 C=* modify it under the terms of the GNU General Public License as    *
 C=* published by the Free Software Foundation; either version 2 of    *
 C=* the License, or (at your option) any later version.               *
 C=*                                                                   *
 C=* This program is distributed in the hope that it will be useful,   *
 C=* but without any warranty; without even the implied warranty of    *
 c=* merchantability or fitness for a particular purpose.  See the GNU *
 C=* General Public License for more details.                          *
 C=* You should have received a copy of the GNU General Public License *
 C=* along with this program. If not, see <http://www.gnu.org/licenses>*
 C=*                                                                   *
 C *********************************************************************
 C
 C     PURPOSE: Main routine for web
 C              Initializes X windows and starts web
 C
 C     PARAMETERS: argv 
 C                 argc   
 C
 C--********************************************************************
*/

#include "x.h"
#include "files.h"

#ifdef app_plus
#include "../AppPlusS/AppPlusS.h"
#endif

#define MAXCOLSPI 8
#define MAXCOL    256
#define XtNfontPath "fontPath"
#define XtCFontPath "FontPath"

 /* External function prototypes */
 extern Widget        web_main(Widget);
 
 /* Command line option recovery structure */
 static XrmOptionDescRec options[] = {
    {"-reduction", "*imageReduction",     XrmoptionSepArg, NULL},
    {"-listfiles", "*listFiles",          XrmoptionNoArg, "FALSE"},
    {"+names",     "*listNames",          XrmoptionNoArg, "TRUE"},
    {"-nsam8",     "*nsam8",              XrmoptionSepArg, NULL},
    {"-nrow8",     "*nrow8",              XrmoptionSepArg, NULL},
    {"-nslice8",   "*nslice8",            XrmoptionSepArg, NULL},
    {"-cursor",    "*cursor",             XrmoptionSepArg, NULL},
    {"-wfg",       "*webForeground",      XrmoptionSepArg, NULL},
    {"-wbg",       "*webBackground",      XrmoptionSepArg, NULL},
    {"-marx",      "*imageXMargin",       XrmoptionSepArg, NULL},
    {"-mary",      "*imageYMargin",       XrmoptionSepArg, NULL},
    {"-marlef",    "*imageLeftMargin",    XrmoptionSepArg, NULL},
    {"-martop",    "*imageTopMargin",     XrmoptionSepArg, NULL},
    {"-cfg",       "*cursorForeground",   XrmoptionSepArg, NULL},
    {"-cbg",       "*cursorBackground",   XrmoptionSepArg, NULL},
    {"-lutdir",    "*lutDirectory",       XrmoptionSepArg, NULL},
    {"-ext",       "*dataExt",            XrmoptionSepArg, NULL},
    {"-trace",     "*traceWeb",           XrmoptionNoArg, "TRUE"},
 };    



 /* Resource recovery structure */
 typedef struct {
    char  *fontpath;
    char  *geom;
    int   iskip;
    int   marx;    
    int   mary;    
    int   marlef;    
    int   martop;   
    Boolean   listfi;
    Boolean   names;
    Boolean   trace_web;
    int   magsizx;
    int   magsizy;
    int   magfac;
    int   nsam8;
    int   nrow8;
    int   nslice8;
    int   iradius;
    float scaleval;
    float refl;
    float fct;
    int   icursor;
    int   icolorcf;
    int   icolorcb;
    int   icolorov;
    int   icolor;
    int   icolorb;
    char  *lutdir;
    char  *datexc;
    } ApplicationData, *ApplicationDataPtr;

 

 /* Resource recovery structure DEFAULT COLORS AND CURSORS SET HERE!!! */
 static XtResource resources[] = {
    {XtNfontPath, XtCFontPath, XtRString, sizeof(char *),
     XtOffset(ApplicationDataPtr, fontpath),
              XtRString, "/usr/lib/X11/fonts"},

    {"lutDirectory", "LutDirectory", XtRString, sizeof(char *),
     XtOffset(ApplicationDataPtr, lutdir), XtRString, "none"},

    {"dataExt", "DataExt", XtRString, sizeof(char *),
     XtOffset(ApplicationDataPtr, datexc), XtRString, "~"},

    {XtNgeometry,XtCGeometry , XmRString, sizeof(char *),
     XtOffset(ApplicationDataPtr, geom), XmRString, "1280x1024+0+0"},

    {"imageReduction","ImageReduction" , XtRInt, sizeof(int),
     XtOffset(ApplicationDataPtr, iskip), XmRImmediate, (XtPointer) 1 },

    {"imageXMargin","ImageXMargin" , XtRInt, sizeof(int),
     XtOffset(ApplicationDataPtr, marx), XmRImmediate, (XtPointer) 1 },

    {"imageYMargin","ImageYMargin" , XtRInt, sizeof(int),
     XtOffset(ApplicationDataPtr, mary), XmRImmediate, (XtPointer) 1 },

    {"imageLeftMargin","ImageLeftMargin" , XtRInt, sizeof(int),
     XtOffset(ApplicationDataPtr, marlef), XmRImmediate, (XtPointer) 1 },

    {"imageTopMargin","ImageTopMargin" , XtRInt, sizeof(int),
     XtOffset(ApplicationDataPtr, martop), XmRImmediate, (XtPointer) 1 },

    {"listFiles","ListFiles" , XtRBoolean, sizeof(Boolean),
     XtOffset(ApplicationDataPtr, listfi), XmRImmediate, (XtPointer) True },

    {"traceWeb","TraceWeb" , XtRBoolean, sizeof(Boolean),
     XtOffset(ApplicationDataPtr, trace_web), XmRImmediate, (XtPointer) False },

    {"listNames","ListNames" , XtRBoolean, sizeof(Boolean),
     XtOffset(ApplicationDataPtr, names), XmRImmediate, (XtPointer) False },
 
    {"magSizeX","magSizeX" , XtRInt, sizeof(int),
     XtOffset(ApplicationDataPtr, magsizx), XmRImmediate, (XtPointer) 64 },

    {"magSizeY","magSizeY" , XtRInt, sizeof(int),
     XtOffset(ApplicationDataPtr, magsizy), XmRImmediate, (XtPointer) 64 },

    {"nsam8","Nsam8" , XtRInt, sizeof(int),
     XtOffset(ApplicationDataPtr, nsam8), XmRImmediate, (XtPointer) 0 },

    {"nrow8","Nrow8" , XtRInt, sizeof(int),
     XtOffset(ApplicationDataPtr, nrow8), XmRImmediate, (XtPointer) 0 },

    {"nslice8","Nslice8" , XtRInt, sizeof(int),
     XtOffset(ApplicationDataPtr, nslice8), XmRImmediate, (XtPointer) 1 },

    {"markerRadius","MarkerRadius" , XtRInt, sizeof(int),
     XtOffset(ApplicationDataPtr, iradius), XmRImmediate, (XtPointer) 5 },

    {"scaleVal","ScaleVal" , XtRFloat, sizeof(float),
     XtOffset(ApplicationDataPtr, scaleval), XmRString, "1.0" },

    {"reflectionFactor","ReflectionFactor" , XtRFloat, sizeof(float),
     XtOffset(ApplicationDataPtr, refl), XmRString, " 0.5" },

    {"fct","Fct" , XtRFloat, sizeof(float),
     XtOffset(ApplicationDataPtr, fct),  XmRString, "1.0" },

    {"cursor","Cursor" , XtRInt, sizeof(int),
     XtOffset(ApplicationDataPtr, icursor), XmRImmediate, (XtPointer) 22 },

    {"cursorForeground","CursorForeground" , XtRInt, sizeof(int),
     XtOffset(ApplicationDataPtr, icolorcf), XmRImmediate, (XtPointer) 7 },

    {"cursorBackground","CursorBackground" , XtRInt, sizeof(int),
     XtOffset(ApplicationDataPtr, icolorcb), XmRImmediate, (XtPointer) 0 },

    {"overlayColor","OverlayColor" , XtRInt, sizeof(int),
     XtOffset(ApplicationDataPtr, icolorov), XmRImmediate, (XtPointer) 3  },

    {"webForeground","WebForeground" , XtRInt, sizeof(int),
     XtOffset(ApplicationDataPtr, icolor), XmRImmediate, (XtPointer)   2 },

    {"webBackground","WebBackground" , XtRInt, sizeof(int),
     XtOffset(ApplicationDataPtr, icolorb), XmRImmediate, (XtPointer)  0 },
    };

 /* To use these variables include common.h in your source code    */ 

 Display        *idispl;           /* Current display               */
 Screen         *iscreen;          /* Current screen                */

 Widget         iw_top;            /* Top widget                    */
 Widget         iw_main ;          /* Main box widget               */
 Widget         iw_menub;          /* Menu bar widget               */
 Widget         iw_win;            /* Image display widget          */
 Widget         iw_files  = 0;     /* File selection widget         */
 Widget         iw_filesnol = 0;   /* File (no list) widget         */
 Widget         iw_scroll;         /* Scrolled output widget        */
 Widget         iw_scrollpar;      /* Parent scrolled output widget */

 Window         iwin, iroot, iwtop;/* Image, root & top windows    */
 Pixmap         imagsav;           /* Image backing store           */
 int            idepth;            /* Display window depth          */
 unsigned int   ihighx, iwidex;    /* Display window x & Y dimen.   */
 int            ixulx, iyulx;      /* Display window location       */
 GC             icontx;            /* Usual graphics context        */
 GC             icontxb  = 0;      /* blanking graphics context     */
 Colormap       map;               /* Current colormap              */
 Visual         *visual;           /* Visual                        */
 FILEDATA       *filedata;         /* Current image file info       */
 XColor         colort[MAXCOL];    /* Temporary color setting array */
 int            nsam,nrow,nslice;  /* Last image size               */
 int            islice;            /* Current slice                 */
 int            nsam1,nsam2;       /* Col. range                    */
 int            nrow1,nrow2;       /* Row range                     */
 int            nslice1,nslice2;   /* Slice range                   */
 double         fmin_spi,fmax_spi; /* Last image range              */
 int            ixulw,iyulw;       /* Window position               */
 int            nsamw,nroww;       /* Window size                   */
 int            ixul,iyul;         /* Upper left corner last image  */
 int            ixuln,iyuln;       /* Corner of next image          */
 int            nprow = 1;         /* Pictures per row              */
 int            nbot,nrit;         /* Bottem & rit image position   */
 char           axis;              /* Axis of slicing               */
 XImage         *imagel = NULL;    /* Left image pointer            */
 XImage         *imager = NULL;    /* Right image pointer           */
 int            nsaml,nrowl;       /* Left image size               */
 int            nsamr,nrowr;       /* Right image size              */
 int            nsam1l,nrow1l;     /* Left image lower extent       */
 int            nsam2l,nrow2l;     /* Left image upper extent       */
 int            nsam1r,nrow1r;     /* Right image lower extent      */
 int            nsam2r,nrow2r;     /* Right image upper extent      */
 int            ixull, iyull;      /* left image position           */
 int            ixulr, iyulr;      /* Left image position           */
 int            ixulli,iyulli;     /* Right image position          */
 int            ixulri,iyulri;     /* Right image position          */
 int            ixullmin,ixlrlmax; /* Left image possible extent    */
 int            iyullmin,iylrlmax; /* Left image possible extent    */
 int            ixulrmin,ixlrrmax; /* Right image possible extent   */
 int            iyulrmin,iylrrmax; /* Right image possible extent   */

 unsigned long  ispicol[MAXCOL];   /* Web colormap values           */
 int            ispicoln[MAXCOL];  /* Color indices (colmen needs)  */
 Cursor         icursorx;          /* Current x cursor number       */
 XColor         colorcursf;        /* Cursor foreground color       */
 XColor         colorcursb;        /* Cursor background color       */
 float          param[102];        /* Web registers (unused)        */ 
 int            ioper    = 0;      /* Current operation number      */
 char           filnow[81] = "\0"; /* Name of last image file       */ 
 char           winnam[81] = "\0"; /* Name of last window file      */ 
 char           docnam[81] = "\0"; /* Name of last doc    file      */ 
 char           filnext[81]= "\0"; /* Name of next image file       */ 
 char           dirnow[81] = "\0"; /* Name of current directory     */ 
 int		ntimg     = FALSE; /* Start next image (marker)	    */
 int            newline   = FALSE; /* New line flag                 */
 int            nuscrn    = FALSE; /* New screen flag               */
 int            gotsurf   = FALSE; /* Surface in memory now         */
 int            samefile  = FALSE; /* Filnow == filnex              */
 int            placed    = FALSE; /* Image position sent flag      */
 int            drawing   = FALSE; /* Flag for xor drawing          */
 int            force     = FALSE; /* Force images / row flag       */
 int            ntop      = 0;     /* Top image position            */

 int            mapgo      ;       /* Image start in colormap       */
 int            colorend   ;       /* Colors end in colormap        */
 int            colorgo    ;       /* Colors start in colormap      */
 int            mapend     ;       /* Image  end in colormap        */
#ifdef WEB_TRUE
 int            imagego   =   0;   /* Image normalization range     */
 int            imageend  = 255;   /* Image normalization range     */
 int            ngo       =   0;   /* Map start range               */
 int            nend      = 255;   /* Map end range                 */
#else
 int            imagego   =  64;   /* Image normalization range     */
 int            imageend  = 127;   /* Image normalization range     */
 int            ngo       =  64;   /* Map start range               */
 int            nend      = 127;   /* Map end range                 */
#endif

 /* Following values do not seem to be used as initialized          */
 int	        nstack    = 0;     /* Stack indicator */
 int		inuse     = 0;     /* Stack file inused indicator   */
 int		maxim     = 0;     /* Max num of files in stack     */
 int		imginstack= 0;     /* Current img. number in stack  */
 int            ncolors   = 128;   /* Number of defined colors      */

 /* Following values appear no longer used!!!!!!!!                  */
 int            lastskip  = 1;     /* Last image mag. factor        */

 /* Following values accessible from defaults file                  */
 int            iskip     = 1;     /* Image mag. factor             */
 int            marx      = 1;     /* X image margin                */
 int            mary      = 1;     /* Y image margin                */
 int            marlef    = 0;     /* Left window margin            */
 int            martop    = 0;     /* Top window margin             */
 int            trace_web = FALSE; /* Does not make results file    */
 int            listfi    = TRUE;  /* List files in selector flag   */
 int            names     = FALSE; /* Print image names flag        */
 int            names_height = 0;  /* Pixel height of image filename */
 int            magsizx   = 64;    /* Magnification size (x)        */
 int            magsizy   = 64;    /* MAgnification size (y)        */
 int            magfac    = 4;     /* Magnification factor          */
 int            nsam8     = 0;     /* Size for raw files            */
 int            nrow8     = 0;     /* Size for raw files            */
 int            nslice8   = 1;     /* Size for raw files            */
 int            iradius   = 5;     /* Marker location radius        */
 float          scaleval  = 1.0;   /* Surface magnification         */
 float          refl      = 0.5;   /* Surface rendering reflection  */
 float          fct       = 1.0;   /* Surface rendering attenuation */
 int            icursor   =  22;   /* Web cursor number             */

 int            icolor    =   2;      /* Current foreground color ABOVE */
 int            icolorb   =   0;      /* Current background color ABOVE */
 int            icolorov  =   3;      /* Overlay foreground color ABOVE */
 int            icolorcf  =   7;      /* Cursor  foreground color ABOVE */
 int            icolorcb  =   0;      /* Cursor  background color ABOVE */

 unsigned long  icolorx   = 0x255;    /* Current x foreground          */
 unsigned long  icolorxb  = 0x0;      /* Current x background          */
 unsigned long  icolorxov = 0x80;     /* Current x overlay             */
 unsigned long  icolorxcf = 0x808080; /* Current x cursor foreground   */
 unsigned long  icolorxcb = 0x0;      /* Current x cursor background   */
 
 unsigned long  icolorxmb;            /* Current x menu background     */
 unsigned long  icolorxmf;            /* Current x menu foreground     */

 char           datexc[]  = "DAT";    /* File extension                */ 
 char           lutdir[81]= "\0"; 


 /* Following values should be made accessible from defaults file?  */
 char           filwant[81] = "*"; /* New file search pattern       */
 int            movie    = FALSE;  /* Image movie flag              */
 int            keepxy   = FALSE;  /* Retain values of x & y        */
 int            normit   = FALSE;  /* Normalize each slice          */
 int            xaxis    = FALSE;  /* X axis selected               */
 int            yaxis    = FALSE;  /* Y axis selected               */
 int            zaxis    = TRUE;   /* Z axis selected               */
 int	        show_num = TRUE;   /* Set show marker number flag   */ 
 int            leavit   = FALSE;  /* Leave window at position      */
 int            sayit    = TRUE ;  /* Say window position           */
 int            inimag   = FALSE;  /* Inside image data only        */
 int            getscreen = FALSE; /* Get screen pixel value        */
 int            getfile  = FALSE;  /* get file pixel value          */
 int            docit    = FALSE;  /* Store data in doc file        */
 int            ikey     = 1;      /* Doc. file key                 */
 int            ikeyc    = 1;      /* Doc. file key for categorizing */
 int            marker   = 0;      /* Set marker flag               */

/* Variables used here */ 
 static ApplicationData  AppData;

 /* Also used by actions.c */
 XtAppContext     app_context;


/********************* main  ***********************/

 int main(unsigned int argc, char **argv)
 
 { 
 int           i, n;
 char          whichweb[6]; 

 Arg           args[4];         /* Arg list */
 XVisualInfo   visual_data;
 Widget        iw_dummy;

 if (argc > 1 && strcmp(argv[1],"help") == 0)
   { 
   printf(" Purpose: A spider image viewer and analyzer \n");
   printf(" Usage:   web [options] file-extension \n");
   printf(" Sample:  web DAT \n");
   exit(1);
   }

 /************ debug **************  (removed) *****
 printf("Initial argc: %d\n",argc);
 for (i=0; i<argc; i++)
    printf(" %s",argv[i]);
    printf(" \n");
    printf("names: %d listfi: %d \n",names,listfi);
 *********** debug ****************/

  /* XtOpenApplication calls XtToolkitInitialize,
 followed by XtCreateApplicationContext, then calls
 XtOpenDisplay with display_string NULL and application_name
 NULL, and finally calls XtAppCreateShell with application_name
 NULL, widget_class applicationShellWidgetClass, and the
 specified args and num_args and returns the created shell.

 The modified argc and argv returned by XtDisplayInitialize
 are returned in argc_in_out and argv_in_out. If
 app_context_return is not NULL, the created application
 context is also returned. If the display specified by the
 command line cannot be opened, an error message is issued
 and XtOpenApplication terminates the application. If
 fallback_resources is non-NULL, XtAppSetFallbackResources is
 called with the value prior to calling XtOpenDisplay.  
 */

 // Open dummy widget to get visual info needed for non-default visual
 iw_dummy = XtAppInitialize(&app_context,"Dummy", NULL, 0, (int *) &argc, argv,
			   (String *) NULL, args, 0);

 idispl = XtDisplay(iw_dummy);
 if (!idispl)
   { printf("%s: can't open display !  EXITING !", argv[0]); exit(0); }

 /* Find default screen pointer */
 if ((iscreen = DefaultScreenOfDisplay(idispl)) == 0)
    {XtWarning("*** Web can not determine screen!"); exit (-1);}

 /* Find depth of screen (number of bit planes) */
 if ((idepth = DefaultDepthOfScreen(iscreen)) < idepth)
    {XtWarning("*** Screen depth too shallow need24 bits!"); exit (-1); }


#ifdef WEB_TRUE
// WEB_TRUE -------------------------------------------------
 strcpy(whichweb, "Web_24"); 

 // Find visual for 24 bit TrueColor
 if (!XMatchVisualInfo(idispl, XDefaultScreen(idispl), idepth,
                       TrueColor, &visual_data))
    {fprintf(stderr,"*** No %d bit, Truecolor visual!",idepth);  exit(2);  }

 // Shared colormap used here 
 map = defluts_true(TRUE);

#else
// WEB_24 -------------------------------------------------
 strcpy(whichweb, "Web_24"); 

 // Find visual for 24 bit DirectColor
 if (!XMatchVisualInfo(idispl, XDefaultScreen(idispl), idepth,
                       DirectColor, &visual_data))
    {fprintf(stderr,"*** No %d bit, Directcolor visual!",idepth);  exit(2);  }

 // Private colormap created here 
 map = defluts(TRUE);

#endif
  
 XtDestroyWidget (iw_dummy);  // Temp. widget no longer needed

 // Open actual top widget
 n = 0;
 XtSetArg(args[n], XtNvisual,   visual_data.visual); n++;
 XtSetArg(args[n], XtNdepth,    visual_data.depth);  n++;
 XtSetArg(args[n], XtNcolormap, map);                n++;

 iw_top = XtOpenApplication(&app_context, whichweb,options,
                            XtNumber(options), (int *) &argc, argv, NULL, 
                            applicationShellWidgetClass, args, n);

 /* Cache the display pointer */ 
 idispl  = XtDisplay(iw_top);
 iscreen = DefaultScreenOfDisplay(idispl);

 /* Find default graphics context */
 icontx  = DefaultGCOfScreen(iscreen);

 /* Check the application resources -------------------------------*/
 XtGetApplicationResources(iw_top, &AppData, resources,
                           XtNumber(resources), NULL, 0);

#ifdef NEVER
 /************ debug ********* (active) *****/
 if (argc > 1)
    {
    printf(" after resources, argc: %d :",argc);
    for (i=0; i<argc; i++)
       printf(" %s",argv[i]);
    printf(" \n");
    /* printf("iskip: %d\n",AppData.iskip);    */
    /* printf("geometry: %s\n",AppData.geom);  */
    /* printf("appdata.names: %d listfi: %d \n", */
    /*    AppData.names,AppData.listfi);         */
    }
 /**********************************************/
#endif

 /* Check iskip */
 if (AppData.iskip == 0)
   {
   printf("In resources, Image_reduction: %d must be non-zero\n",
          AppData.iskip);
   AppData.iskip = iskip;
   }
 iskip =  AppData.iskip;


 /* Check marx, mary */
 if (AppData.marx < 0 || AppData.mary < 0)
   {
   printf("In resources, imageXMargin:%d &  ImageYMargin: %d must be > zero\n",
          AppData.marx, AppData.mary);
   AppData.marx = marx;
   AppData.mary = mary;
   }
 marx =  AppData.marx;
 mary =  AppData.mary;

 /* Check marlef, martop */
 if (AppData.marlef < 0 || AppData.martop < 0)
   {
   printf("In resources, ImageLeftMargin:%d &  ImageTopMargin: %d must be > zero\n",
          AppData.marlef, AppData.martop);
   AppData.marlef = marlef;
   AppData.martop = martop;
   }
 marlef =  AppData.marlef;
 martop =  AppData.martop;

 /* Check magsizx, magsizy */
 if (AppData.magsizx <= 0 || AppData.magsizy <= 0)
   {
   printf("In resources, magSizeX:%d &  magSizeY: %d must > zero\n",
          AppData.magsizx, AppData.magsizy);
   AppData.magsizx = marlef;
   AppData.magsizy = magsizy;
   }
 magsizx =  AppData.magsizx;
 magsizy =  AppData.magsizy;

 /* Check nsam8, nrow8 nslice8*/
 if (AppData.nsam8 < 0 || AppData.nrow8 < 0 || AppData.nslice8 < 0 )
   {
   printf("In resources, nsam8:%d , nrow8: %d & nslice8: %d must be >= zero\n",
          AppData.nsam8, AppData.nrow8,AppData.nslice8);
   AppData.nsam8   = nsam8;
   AppData.nrow8   = nrow8;
   AppData.nslice8 = nslice8;
   }
 nsam8   =  AppData.nsam8;
 nrow8   =  AppData.nrow8;
 nslice8 =  AppData.nslice8;

 /* Check iradius */
 if (AppData.iradius <= 0)
   {
   printf("In resources, MarkerRadius: %d must be > zero\n",
          AppData.iradius);
   AppData.iradius = iradius;
   }
 iradius =  AppData.iradius;

 /* Check scaleval */
 if (AppData.scaleval <= 0.0)
   {
   printf("In resources, ScaleVal: %d must be > zero\n",
          AppData.scaleval);
   AppData.scaleval = scaleval;
   }
 scaleval =  AppData.scaleval;

 /* Check scaleval */
 if (AppData.scaleval <= 0.0)
   {
   printf("In resources, ScaleVal: %d must be > zero\n",
          AppData.scaleval);
   AppData.scaleval = scaleval;
   }
 scaleval =  AppData.scaleval;

 /* Check refl */
 if (AppData.refl < 0.0)
   {
   printf("In resources, ReflectionFactor: %d must be >= zero.\n",
          AppData.refl);
   AppData.refl = refl;
   }
 refl =  AppData.refl;

 /* Check fct */
 if (AppData.fct < 0.0)
   {
   printf("In resources, Fct: %d must be >= zero.\n",
          AppData.fct);
   AppData.fct = fct;
   }
 fct =  AppData.fct;

 /* Check icursor */
 icursor =  AppData.icursor;

 /* Check icolorcf, icolorcb */
 if (AppData.icolorcf < 0 || AppData.icolorcb < 0)
   {
   printf("In resources, cursorForeground:%d &  cursorBackground: %d must >= zero\n",
          AppData.icolorcf, AppData.icolorcb);
   AppData.icolorcf = icolorcf;
   AppData.icolorcb = icolorcb;
   }
 icolorcf =  AppData.icolorcf;
 icolorcb =  AppData.icolorcb;

 /* Check icolor, icolorb */
 if (AppData.icolor < 0 || AppData.icolorcb < 0)
   {
   printf("In resources, webcolor:%d &  webBackground: %d must >= zero\n",
          AppData.icolor, AppData.icolorb);
   AppData.icolor  = icolor;
   AppData.icolorb = icolorb;
   }
 icolor   = AppData.icolor;
 icolorb  = AppData.icolorb;
 icolorx  = icolor;   
 icolorxb = icolorb;   

 /* Check icolorov */
 if (AppData.icolorov < 0.0)
   {
   printf("In resources, overlayColor: %d must be >= zero.\n",
          AppData.icolorov);
   AppData.icolorov = icolorov;
   }
 icolorov = AppData.icolorov;

 /* Check iradius */

 if (*AppData.lutdir != 'n')
   {   /* lutdir is provided by resouces or command line */
   strncpy(lutdir,AppData.lutdir,80);
   }

 if (*(AppData.datexc) != '~')
   {   /* datexc is provided by resources or command line */
   strncpy(datexc,AppData.datexc,3);
   }

 if (argc > 1 && *argv[argc-1] != '-')
   {  /* Last arguement is datexc if does not begin with - */
   strncpy(datexc,argv[argc-1],3);
   }

 /* printf(" geometry: %s: %d \n", AppData.geom,iskip); */

 listfi    = AppData.listfi;
 names     = AppData.names;
 trace_web = AppData.trace_web;

 /* Start the application ------------------------------------ */
 web_main(iw_top);
 printf(" Data files:         %s \n", datexc);

 /* Loop to get events until exit is requested */
 XtAppMainLoop(app_context);

 return TRUE;

 }

