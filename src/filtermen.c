
/*$Header: /usr8/web/src/RCS/filtermen.c,v 1.47 2011/05/10 13:33:44 leith Exp $*/

/*
C++*********************************************************************
C
C filtermen
C
C **********************************************************************
C *  AUTHOR:  ArDean Leith                                             *
 C=* FROM: WEB - VISUALIZER FOR SPIDER MODULAR IMAGE PROCESSING SYSTEM *
 C=* Copyright (C) 1992-2005  Health Research Inc.                     *
 C=*                                                                   *
 C=* HEALTH RESEARCH INCORPORATED (HRI),                               *   
 C=* ONE UNIVERSITY PLACE, RENSSELAER, NY 12144-3455.                  *
 C=*                                                                   *
 C=* Email:  spider@wadsworth.org                                      *
 C=*                                                                   *
 C=* This program is free software; you can redistribute it and/or     *
 C=* modify it under the terms of the GNU General Public License as    *
 C=* published by the Free Software Foundation; either version 2 of    *
 C=* the License, or (at your option) any later version.               *
 C=*                                                                   *
 C=* This program is distributed in the hope that it will be useful,   *
 C=* but WITHOUT ANY WARRANTY; without even the implied warranty of    *
 C=* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU *
 C=* General Public License for more details.                          *
 C=*                                                                   *
 C=* You should have received a copy of the GNU General Public License *
 C=* along with this program; if not, write to the                     *
 C=* Free Software Foundation, Inc.,                                   *
 C=* 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.     *
 C=*                                                                   *
C **********************************************************************
C
C    filtermen
C
C    PURPOSE:     Display filter menu
C
C    CALL TREE:
                                 filterfill
                                 denoise
                                 filtestb
                                 thresh
                                 filterskel
                 / filterbut --> filtere    
                 | showbimage    localareaeq
                 |               invert
                 |               filterv
                 |               filter
                 |                 |       filtermen
   imagemen      |                 |       /
      |          |                 `----> filinc --> filtersave
      v          |                           ^            |
  imagemen_cb    | filter-butrep <------------------------'
      |          |           \
      v          |            \
   imagemen1     |             filinc --> imagemen1 --> filterbut
      |          | 
      v          |
   filtermen --> | filterbut_flip --> 
                 |
                 | threshmen --> thresh -->
                 |
                 | lahemen --> lahe -->
                 |
                 \ fillmen --> fill -- >


C***********************************************************************
*/

#include <Xm/RowColumn.h>
#include <Xm/Text.h>
#include <Xm/Scale.h>
#include "common.h"
#include "routines.h"

 /* Routines called from here */
 extern int  filset     (float *, int, int, float, float,
                         float *, float *, float *);
 extern int  denoise    (float *, int, int, float *, int, int, 
                         float *, float *, float *, float);
 extern void setmen     (Widget, XtPointer, XtPointer);
 extern void denoisemen (Widget, XtPointer, XtPointer);
 extern void lahemen    (Widget, XtPointer, XtPointer);
 extern void threshmen  (Widget, XtPointer, XtPointer);
 extern void erodemen   (Widget, XtPointer, XtPointer);
 extern void fillmen    (Widget, XtPointer, XtPointer);
 extern int  filtestb   (float *, int, int, float, float, float *, 
                         float *, float *);

 /* Internal function prototypes */
 void          filter_buts   (Widget, XtPointer, XtPointer);
 void          filter_butsav (Widget, XtPointer, XtPointer);
 void          filter_butrep (Widget, XtPointer, XtPointer);
 void          filter_butflip(Widget, XtPointer, XtPointer);
 void          filtersav_cb  (Widget, XtPointer, XtPointer);
 void          filtersave    (char *);

 /* External global variables used here */
 extern int        nsams, nrows, nslices;
 extern float    * fimage;
 extern char       outstr[80];
 extern float      threshval, fillval;    /* From fillmen */
 extern int        ixseed, iyseed;
 extern int        fillconnect;
 extern float      noiselowerlim;

 /* Common globals used elsewhere */
 float           fminnew, fmaxnew,fminold, fmaxold;
 int             nsamold, nrowold;
 float           lower, lowerval, upper,  upperval; /* threshmen */
 float           erodethresh;    /* erodemen */
 int             bins   = 30;    /* lahemen */
 int             border = 10;    /* lahemen */
 float           oldset = 0.0;   /* setmen  */
 float           newset = 0.0;   /* setmen  */

 /* File scope variables used here */
 static char *   environm;
 static char*    cimage        = NULL;
 static char     manfile[81]   = "convolve.man";       
 static char     outnam[81]    = "\0";      /* Name for output file  */ 
 static char     prompt[]      = "Enter output file name"; 
 static char     lastfilter[1] = " ";
 static float*   newimage      = NULL;
 static Widget   iw_filtermen  = (Widget)0;
 static Widget   iw_text       = (Widget)0;
 static int      repeat        = FALSE;
 static int      saveinfile    = FALSE;

 /************************   filtermen   ******************************/

 void filtermen(void)

 { 
 Widget iw_rowcolv;

 Widget iw_pushs,   iw_pushc,   iw_dums;
 Widget iw_menubc1, iw_menubc2, iw_menubc3, iw_menubc4, iw_menubc5, 
        iw_menubc6, iw_menubc7;
 Widget iw_menupc1, iw_menupc2, iw_menupc3, iw_menupc4, iw_menupc5, 
        iw_menupc6, iw_menupc7;

 Arg     args[5];  

 if (iw_filtermen <= (Widget)0)
    {   /* create filter menu widget first */

    iw_filtermen  = wid_dialog(iw_win, 0, "Discrete convolution menu", -1,-1);
    iw_rowcolv    = wid_rowcol(iw_filtermen, 'v', -1, -1);

    /* Create  Laplacian menu ---------------------------- Laplacian */
    iw_menubc1 = wid_menubar(iw_rowcolv);

    iw_menupc1 = wid_pulldown(iw_menubc1,"Laplacian      =>",'A');
    wid_pushg(iw_menupc1,(Widget)0," 3 x 3",filter_but,"1",0,0);
    wid_pushg(iw_menupc1,(Widget)0," 5 x 5",filter_but,"2",0,0);
    wid_pushg(iw_menupc1,(Widget)0," 7 x 7",filter_but,"v",0,0);

    /* Create  Sobel menu ------------------------------------ Sobel */
    iw_menubc7 = wid_menubar(iw_rowcolv);

    iw_menupc7 = wid_pulldown(iw_menubc7,"Sobel         =>",'A');
    wid_pushg(iw_menupc7,(Widget)0," X ",filter_but,"C",0,0);
    wid_pushg(iw_menupc7,(Widget)0," Y ",filter_but,"D",0,0);

    /* Create a Gaussian button --------------------------- Gaussian */
    wid_pushg(iw_rowcolv,(Widget)0,"Gaussian (7x7)",filter_but,"3",0,0);

    /* Create a Hipass menu --------------------------------- Hipass */
    iw_menubc2 = wid_menubar(iw_rowcolv);

    iw_menupc2= wid_pulldown(iw_menubc2,"Highpass       =>",'A');
    wid_pushg(iw_menupc2,(Widget)0," 3 x 3",filter_but,"4",0,0);
    wid_pushg(iw_menupc2,(Widget)0," 5 x 5",filter_but,"5",0,0);

    /* Create a LoPass menu --------------------------------- LoPass */
    iw_menubc3 = wid_menubar(iw_rowcolv);

    iw_menupc3 = wid_pulldown(iw_menubc3,"LowPass      =>",'A');
    wid_pushg(iw_menupc3, 0, " (3x3)",filter_but,"6", -1,-1);
    wid_pushg(iw_menupc3, 0, " (5x5)",filter_but,"7", -1,-1);

    /* Create a Sharpen button ------------------------------ Sharpen */
    wid_pushg(iw_rowcolv, 0, "Sharpen (3x3)",filter_but,"8",-1,-1);

    /* Create a Erode menu ----------------------------------- Erode */
    iw_menubc4 = wid_menubar(iw_rowcolv);

    iw_menupc4 = wid_pulldown(iw_menubc4,"Erode      =>",'A');
    wid_pushg(iw_menupc4, 0, " (0)",erodemen,"a", -1,-1);
    wid_pushg(iw_menupc4, 0, " (1)",erodemen,"b", -1,-1);
    wid_pushg(iw_menupc4, 0, " (2)",erodemen,"c", -1,-1);
    wid_pushg(iw_menupc4, 0, " (3)",erodemen,"d", -1,-1);
    wid_pushg(iw_menupc4, 0, " (4)",erodemen,"e", -1,-1);
    wid_pushg(iw_menupc4, 0, " (5)",erodemen,"f", -1,-1);
    wid_pushg(iw_menupc4, 0, " (6)",erodemen,"g", -1,-1);
    wid_pushg(iw_menupc4, 0, " (7)",erodemen,"h", -1,-1);

    /* Create a Dilate menu ---------------------------------- Dilate */
    iw_menubc5 = wid_menubar(iw_rowcolv);

    iw_menupc5= wid_pulldown(iw_menubc5,"Dilate      =>",'A');
    wid_pushg(iw_menupc5, 0, " (0)",erodemen,"i", -1,-1);
    wid_pushg(iw_menupc5, 0, " (1)",erodemen,"j", -1,-1);
    wid_pushg(iw_menupc5, 0, " (2)",erodemen,"k", -1,-1);
    wid_pushg(iw_menupc5, 0, " (3)",erodemen,"l", -1,-1);
    wid_pushg(iw_menupc5, 0, " (4)",erodemen,"m", -1,-1);
    wid_pushg(iw_menupc5, 0, " (5)",erodemen,"n", -1,-1);
    wid_pushg(iw_menupc5, 0, " (6)",erodemen,"o", -1,-1);
    wid_pushg(iw_menupc5, 0, " (7)",erodemen,"p", -1,-1);

    /* Create a Variance menu -------------------------------- Variance */
    iw_menubc6= wid_menubar(iw_rowcolv); 

    iw_menupc6= wid_pulldown(iw_menubc6,"Variance      =>",'A');
    wid_pushg(iw_menupc6, 0, " (3x3)",filter_but,"q", -1,-1);
    wid_pushg(iw_menupc6, 0, " (5x5)",filter_but,"r", -1,-1);
    wid_pushg(iw_menupc6, 0, " (7x7)",filter_but,"s", -1,-1);

    /* Create a lahe button ----------------------------------- lahe */
    wid_pushg(iw_rowcolv, 0, "LAHE",lahemen,"t",-1,-1);

    /* Create a thresh button --------------------------------- thresh */
    wid_pushg(iw_rowcolv, 0, "Threshold",threshmen,"u",-1,-1);

    /* Create a fill button ----------------------------------- fill */
    wid_pushg(iw_rowcolv, 0, "Seed fill",fillmen,"x",-1,-1);

    /* Create a invert button --------------------------------- invert */
    wid_pushg(iw_rowcolv, 0, "Invert",filter_but,"y",-1,-1);

    /* Create a test button ----------------------------------- test */
    wid_pushg(iw_rowcolv, 0, "Test",filter_but,"w",-1,-1);

    /* Create a set button ------------------------------------ set */
    wid_pushg(iw_rowcolv, 0, "Set",setmen,"A",-1,-1);

    /* Create a thresh buttonmenu ----------------------------- denoise */
    wid_pushg(iw_rowcolv, 0, "Denoise",denoisemen,"B",-1,-1);

    /* Create a skeleton button  ------------------------------ skeleton */
    wid_pushg(iw_rowcolv, 0, "Binary skeleton",
                                      filter_but,"z",-1,-1);

    /* Create push button for repeating on file series. ------- repeat */
    wid_pushg(iw_rowcolv, 0, "Repeat on series",
                           filter_butrep,"0", -1,-1);

    /* Create push button for flipping. --------------------- flip I/O */
    wid_pushg(iw_rowcolv, 0, "Flip input and output",
                         filter_butflip,"0", -1,-1);

    /* Create push button for saving file. --------------------- save */
    wid_pushg(iw_rowcolv, 0, "Save in file",filter_butsav,"0", -1,-1);

    /* Create box for stop ------------------------------------- stop */
    wid_stdbut(iw_rowcolv,  iw_filtermen, 
               &iw_pushs,   &iw_pushc, &iw_dums,  "S",
               filter_buts, fin_cb ,fin_cb, manfile);
    }

 /* Set size and min, max as per input image */
 nsamold     = filedata -> nsam;
 nrowold     = filedata -> nrow;
 fminold     = filedata -> fmin_spi;
 fmaxold     = filedata -> fmax_spi;
 fminnew     = filedata -> fmin_spi;
 fmaxnew     = filedata -> fmax_spi;
 repeat      = FALSE;
 saveinfile  = FALSE;

 XtManageChild(iw_filtermen);
 }  


/****************  Filter  callback ***********************/

 void filter_but(Widget iw_temp, XtPointer data, XtPointer call_data)
 {

 static float sobelx[] = {-1.0,-2.0,-1.0,
                           0.0, 0.0, 0.0,
                           1.0, 2.0, 1.0};

 static float sobely[] = {-1.0, 0.0, 1.0,
                          -2.0, 0.0, 2.0,
                          -1.0, 0.0, 1.0};

static float laplacian[] = { 0.0,-1.0, 0.0,
                             -1.0, 4.0,-1.0,
                              0.0,-1.0, 0.0};


 static float laplac5[] = { -1, -1, -1, -1, -1,
                            -1, -1, -1, -1, -1,
                            -1, -1, 24, -1, -1,
                            -1, -1, -1, -1, -1,
                            -1, -1, -1, -1, -1,};

 static float laplac7[] = { -1, -1, -1,  -1, -1, -1, -1,
                            -1, -1, -1,  -1, -1, -1, -1,
                            -1, -1, -1,  -1, -1, -1, -1,
                            -1, -1, -1,  48, -1, -1, -1,
                            -1, -1, -1,  -1, -1, -1, -1,
                            -1, -1, -1,  -1, -1, -1, -1,
                            -1, -1, -1,  -1, -1, -1, -1,};

 static float gauss[] = {1, 1, 2, 2, 2, 1, 1,
                         1, 2, 2, 4, 2, 2, 1,
                         2, 2, 4, 8, 4, 2, 2,
                         2, 4, 8,16, 8, 4, 2,
                         2, 2, 4, 8, 4, 2, 2,
                         1, 2, 2, 4, 2, 2, 1,
                         1, 1, 2, 2, 2, 1, 1};

 static float hipass3[]  = {-1,-1,-1,
                            -1, 9,-1,
                            -1,-1,-1};

  static float denoise3[] = { 1, 1, 1,
                              1, 0, 1,
                              1, 1, 1};

 static float hipass5[] =  { 0,-1,-1,-1, 0,
                            -1, 2,-4, 2,-1,
                            -1,-4,13,-4,-1,
                            -1, 2,-4, 2,-1,
                             0,-1,-1,-1, 0};

 static float sharpen3[] = {-1,-2,-1,
                            -2,16,-2,
                            -1,-2,-1};

 static float lopass3[]  = { 1, 2, 1,
                             2, 4, 2,
                             1, 2, 1};


 static float lopass5[] = { 1, 1, 1, 1, 1,
                            1, 4, 4, 4, 1,
                            1, 4,12, 4, 1,
                            1, 4, 4, 4, 1,
                            1, 1, 1, 1, 1};
 
 static float variance3[]  = { 1, 1, 1,
                               1, 0, 1,
                               1, 1, 1};

 static float variance5[] = { 0, 1, 1, 1, 0,
                              1, 1, 1, 1, 1,
                              1, 1, 0, 1, 1,
                              1, 1, 1, 1, 1,
                              0, 1, 1, 1, 0};

 static float variance7[] = { 0, 0, 1, 1, 1, 0, 0,
                              0, 1, 1, 1, 1, 1, 0,
                              1, 1, 1, 1, 1, 1, 1,
                              1, 1, 1, 0, 1, 1, 1,
                              1, 1, 1, 1, 1, 1, 1,
                              0, 1, 1, 1, 1, 1, 0,
                              0, 0, 1, 1, 1, 0, 0};
 int            iok;
 float *        tempimage;
 int            isel;
 char           erodes[]  = "abcdefgh";
 char           dilates[] = "ijklmnop";
 char           *cptr;

 /* set size and min, max as per input image */
 nsamold   = filedata -> nsam;
 nrowold   = filedata -> nrow;
 fminold   = filedata -> fmin_spi;
 fmaxold   = filedata -> fmax_spi;
 fminnew   = filedata -> fmin_spi;
 fmaxnew   = filedata -> fmax_spi;

 if (!newimage)
    {
    /* allocate newimage space first */
    if ((newimage = (float *) malloc(nsamold * nrowold *
                      sizeof(float))) == (float *) 0)
      {spout("*** Unable to allocate memory in filter_but.");return;}
    }


 /* set waitcursor */
 waitcursor(iwin, TRUE, 79);

 if (!strcmp(data, "1"))
    {                                            /* laplacian filter */
    iok = filter(fimage, nsamold, nrowold, 
          laplacian, 3, 3,
          newimage , &fminnew, &fmaxnew);
    }
 else if (!strcmp(data, "2"))
    {  /* laplac5 filter */
    iok = filter(fimage, nsamold, nrowold, 
          laplac5, 5, 5,
          newimage , &fminnew, &fmaxnew);
    }
 else if (!strcmp(data, "v"))
    {  /* laplac7 filter */
    iok = filter(fimage, nsamold, nrowold, 
          laplac7, 7, 7,
          newimage , &fminnew, &fmaxnew);
    }

 else if (!strcmp(data, "C"))
    {                                                /* Sobel filter */
    iok = filter(fimage, nsamold, nrowold, 
          sobelx, 3, 3,
          newimage , &fminnew, &fmaxnew);
    }

 else if (!strcmp(data, "D"))
    {                                                
    iok = filter(fimage, nsamold, nrowold, 
          sobely, 3, 3,
          newimage , &fminnew, &fmaxnew);
    }

 else if (!strcmp(data, "3"))
    {                                                /* gauss filter */
    iok = filter(fimage, nsamold, nrowold, 
          gauss, 7, 7,
          newimage , &fminnew, &fmaxnew);
    }
 else if (!strcmp(data, "4"))
    {                                              /* hipass3 filter */
    iok = filter(fimage, nsamold, nrowold, 
          hipass3, 3, 3,
          newimage , &fminnew, &fmaxnew);
    }
 else if (!strcmp(data, "5"))
    {  /* hipass5 filter */
    iok = filter(fimage, nsamold, nrowold, 
          hipass5, 5, 5,
          newimage , &fminnew, &fmaxnew);
    }

 else if (!strcmp(data, "6"))
    {                                              /* lopass3 filter */
    iok = filter(fimage, nsamold, nrowold, 
          lopass3, 3, 3,
          newimage , &fminnew, &fmaxnew);
    }
 else if (!strcmp(data, "7"))
    {  /* lopass5 filter */
    iok = filter(fimage, nsamold, nrowold, 
          lopass5, 5, 5,
          newimage , &fminnew, &fmaxnew);
    }
 else if (!strcmp(data, "8"))
    {                                              /* sharpen filter */
    iok = filter(fimage, nsamold, nrowold, 
          sharpen3, 3, 3,
          newimage , &fminnew, &fmaxnew);
    }

 else if ((cptr = strstr(erodes, data)) != NULL )
    {                                                /* erode filter */
    isel = cptr - erodes;
    iok = filtere(fimage, nsamold, nrowold, 
          TRUE, (int) isel, erodethresh,
          newimage , &fminnew, &fmaxnew);
    }

 else if ((cptr = strstr(dilates, data)) != NULL )
    {                                                /* dilate filter */
    isel = cptr - dilates;
    iok = filtere(fimage, nsamold, nrowold, 
          FALSE, (int) isel, erodethresh,
          newimage , &fminnew, &fmaxnew);
    }

 else if (!strcmp(data, "q"))
    {                                            /* variance  filter */
    iok = filterv(fimage, nsamold, nrowold, 
          variance3, 3, 3,
          newimage , &fminnew, &fmaxnew);
    }
 else if (!strcmp(data, "r"))
    {  /* variance5 filter */
    iok = filterv(fimage, nsamold, nrowold, 
          variance5, 5, 5,
          newimage , &fminnew, &fmaxnew);
    }
 else if (!strcmp(data, "s"))
    {  /* variance7 filter */
    iok = filterv(fimage, nsamold, nrowold, 
          variance7, 7, 7,
          newimage , &fminnew, &fmaxnew);
    }

 else if (!strcmp(data, "t"))
    {                                        /* local area histogram */
    iok = localareaeq(fimage,nsamold,nrowold,
            &fminnew,&fmaxnew,bins, border, newimage);
    }

 else if (!strcmp(data, "u"))
    {                                                   /* threshold */
    iok = thresh(fimage,nsamold,nrowold,
                 lower,lowerval, upper, upperval, 
                 newimage, &fminnew, &fmaxnew);
    }

 else if (!strcmp(data, "w"))
    {  
                                                          /* filtest */
    iok = filtestb(fimage, nsamold,nrowold,
                1.0, 3.0, newimage, &fminnew,&fmaxnew);
    }

 else if (!strcmp(data, "A"))
    {  
                                                          /* filset */
    iok = filset(fimage, nsamold,nrowold,
                oldset, newset, newimage, &fminnew,&fmaxnew);
    }

 else if (!strcmp(data, "B"))
    {  
                                                          /* denoise */
    iok = denoise(fimage, nsamold, nrowold, 
          denoise3, 3, 3,


          newimage , &fminnew, &fmaxnew, noiselowerlim);
    }

 else if (!strcmp(data, "x"))
    {                                                     /*  fill */
    iok = filterfill(fimage, nsamold,nrowold,
                ixseed, iyseed, threshval, fillval, 
                newimage, &fminnew,&fmaxnew,fillconnect);
    }

 else if (!strcmp(data, "y"))
    {                                                      /* invert */
    iok = invert(fimage, nsamold,nrowold,
                newimage, &fminnew,&fmaxnew);
    }
 else if (!strcmp(data, "z"))
    {                                                     /* skeleton */
    iok = filterskel(fimage, nsamold,nrowold,
                newimage, &fminnew,&fmaxnew);
    }

 /* remove the filter menu to get out of way */
 XtUnmanageChild(iw_filtermen); 

 if (iok)
    {
    /* display filtered image */
    tempimage = fimage;
    fimage    = newimage;
    /* display image in floating point buffer */
    /* destroy imagel and cimage */
    showbimage(fimage, FALSE, &cimage,
                       FALSE, &imagel, 
                       nsam, nrow, fminnew, fmaxnew);
    fimage    = tempimage;
    }

 /* Cancel waitcursor */
 waitcursor(iwin, FALSE, 79);

 /* Remember filter type used */
 strncpy(lastfilter,(char *) data,1);

 if (!repeat)
    { /* Redisplay the filter choice menu */
    filtermen();
    }
 }

/***************************  repeat  callback ***********************/

 void filter_butrep(Widget iw_temp, XtPointer data, XtPointer call_data)
 {
 int nlet, numfil, iok;

 /* Free up the file descriptor on current file */
 closefile(filedata);

 do {
    /* Increment input file name */
    if (filinc(filnow,filnext,&nlet,&numfil) != FALSE)
       {   /* file number incrementing succeeded */
       /* Start the next convolution */
       repeat = TRUE;

       /* Open and read input file */
       ioper  = -10;
       iok    = imagemen1(filnext, TRUE);

       if (iok) 
          {
          /* Carry out filtration */
          filter_but(NULL, (XtPointer) lastfilter, NULL);

          if (saveinfile)
             {      /* Want to save filter output in file */
             /* Increment output file name */
             if (filinc(outnam, outnam, &nlet, &numfil) != FALSE)
                {   /* Place output in output file */
                filtersave(outnam);
                }/* end if (filinc      */
             }   /* end if (saveinfile) */

          /* Free up the file descriptor on current file */
          closefile(filedata);

          }      /* end if (iok)        */
       }         /* end if (filinc      */
    } while (iok == TRUE);

 }

/***********  stop convolving callback *******************************/

 void filter_buts(Widget iw_temp, XtPointer data, XtPointer call_data)
 {
 /* Free memory for image arrays */
 if (newimage) 
     { free(newimage); newimage = NULL; }
 if (fimage) 
     { free(fimage); fimage = NULL; }

 /* Free up the file descriptor */
 closefile(filedata);

 /* Remove the filter menu to get out of way */
 XtUnmanageChild(iw_filtermen);

 }

/***************************  save  callback *************************/

 void filter_butsav(Widget iw_temp, XtPointer data, XtPointer call_data)
 {
 int                            nlet, numfil;

 /* Increment the output filename if not null */
 if (outnam[1] != '\0')
     filinc(outnam, outnam, &nlet, &numfil);
 
 /* Create text input box */
 iw_text = wid_text(iw_win, iw_text, prompt, &outnam[0],
                    filtersav_cb, NULL, 0, 0);
 }

 /*****************  filtersav_cb  (runs after wid_text) *************/

 void filtersav_cb(Widget iw_temp, XtPointer data, XtPointer call_data)
 {
 XmSelectionBoxCallbackStruct * cbs;

 cbs = (XmSelectionBoxCallbackStruct *) call_data;

 strcpy(outnam,extractstr(cbs -> value));

 filtersave(outnam);

 saveinfile = TRUE;
 }

 /*****************  filtersave  (runs after filtersav_cb) ************/

 void filtersave(char * filnam)
 {
 FILEDATA      *filedatat;

 /* Redraw image in case damaged by window name box */ 
 expos_cb(NULL,NULL,NULL);

 /* Add datexc to filnam if not already there */
 if (strstr(filnam,datexc) == 0)
    {
    strcat(filnam,".");
    strcat(filnam,datexc);
    }

 /* Open new output file */
 filedatat = opennew(filnam, nsam, nrow, 1, 1, "u");

 /* Put newimage in output file */
 wrtvol(filedatat, newimage);

 closefile(filedatat);

 /* Remember the output file name */
 strcpy(outnam,filnam);
 }

/****************************  flip  callback ***********************/

 void filter_butflip(Widget iw_temp, XtPointer data, XtPointer call_data)
 {
 float * tempptr;

 tempptr     = newimage;
 newimage    = fimage;
 fimage      = tempptr;
 fminold     = fminnew;
 fmaxold     = fmaxnew;
 lowerval    = fminold;
 upperval    = fmaxold;
 erodethresh = fmaxold;

 /* Tell user new min and max, in case he wants them for new filters */
 sprintf(outstr,"New Range:  %f ...%f",fminnew,fmaxnew);
 spout(outstr);
 }


