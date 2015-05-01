/*
*
*  Adapted from moviemen.c
*
c++*********************************************************************
c
c showmoviemen
c
C **********************************************************************
C * WEB.    AUTHOR: ARDEAN LEITH                                       *
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
c
c   CALL TREE:
c
c   imagemen      --> wid_file  --> imagemen_cb --> imagemen_cb_com 
c                                                        v
c                                                        v
c   showmovmen2_buta <-- showmovmen2  <--  openold  <-- imagemen1
c   showmovmen3_buta <-- showmovmen3
c      v
c      v
c   mont2         -->  imagedis -->  showimage  --> locc
c   (or)                                              v
c   mont3         -->  wipic                          v
c                        v                            v
c                        v                            v
c   showmovie    <-- showmoviemen <--  wipic   <-- redvol
c      v
c      v
c   movie_buts
c
c--*********************************************************************
*/

#include "common.h"
#include "routines.h"

#include <Xm/Scale.h>
#include <Xm/ToggleBG.h>
#include <Xm/Text.h>

/* external function prototypes */
extern void showmovie(void);

/* internal function prototypes */
void   showmovmen2          (void);
void   showmovmen2_buta     (Widget, XtPointer, XtPointer);
void   showmovmen2_butc     (Widget, XtPointer, XtPointer);
void   showmovmen3          (void);
void   showmovmen3_buta     (Widget, XtPointer, XtPointer);
void   showmovmen3_butc     (Widget, XtPointer, XtPointer);

void   showmoviemen         (int);
void   showmoviemen_buts    (Widget, XtPointer, XtPointer); 
void   showmoviemen_buta    (Widget, XtPointer, XtPointer);

/* external variables defined elsewhere and used here */
extern int        ndone;             /* total number of images */
extern XImage *   ximagelist[1000];  /* list of XImages in movie */
extern int        lastfile;          /* used in mont2 */
extern char *     cmont3buf ;        /* from mont3 */


/* external variables defined here */
Widget            iw_showmoviemen, iw_delayb, iw_twoways;
int               movie_twoways, movie_inc, movie_delay, movie_frame; 

/* file scope internal variables used here */
static Widget     iw_showmovmen2 = (Widget)0;
static Widget     iw_numfil, iw_pushc, iw_pusha;

static Widget     iw_showmovmen3 = (Widget)0;
static Widget     iw_push3c, iw_push3a;
static Widget     iw_scales[6],iw_xaxis, iw_yaxis, iw_zaxis, iw_norm;

 /************************************************************/

 void showmovmen2(void)
 { 
 Widget iw_rowcol, iw_dums;
 char   cval[9];

 if (iw_showmovmen2 == (Widget)0)
    {   /* create movie series menu widget first */

    iw_showmovmen2 = wid_dialog(iw_win, 0, "series options", -1, -1);
    iw_rowcol      = wid_rowcol(iw_showmovmen2, 'v', -1, -1);

    /* create text box for numfil input ---------------------- numfil */
    strcpy(cval,"*");
    iw_numfil = wid_textboxb(iw_rowcol, 0, "LAST FILE NUMBER:", 
                            cval, 6);

    /* create box for apply ----------------------------------- apply */
    wid_stdbut(iw_rowcol, iw_showmovmen2, &iw_dums, &iw_pushc, 
               &iw_pusha,  "CA",
               fin_cb, showmovmen2_butc ,showmovmen2_buta, NULL);
    }

 XtManageChild(iw_showmovmen2);
 }



 /***********  showmovmen2_buta  (accept button callback) *************/

 void showmovmen2_buta(Widget iw_temp, XtPointer data, XtPointer call_data)
 {
 char *string = NULL;
 int  lastfilt;

 /* find file numbers wanted */
 string = XmTextGetString(iw_numfil);

 if (strncmp(string,"*",1) == 0)
    {
    /* want all files in this series until first missing file */
    lastfile = -1;
    lastfilt = -1;
    }
 else
    {
    if (sscanf(string,"%d",&lastfilt) <= 0 || lastfilt < 0)
       {   /* can not interpret lastfilt */
       spout("*** Can not interpret number of last file in montage! ");
       if (string) XtFree (string); return;
       }
    lastfile = lastfilt;
    }
 if (string) XtFree (string);

 /* remove the montage menu */
 XtUnmanageChild(iw_showmovmen2);
 expos_cb(NULL,NULL,NULL);

 /* display the montage, imagego set in imagemen */
 mont2(imagego,imageend);

 showmoviemen(TRUE); 
 }



 /***********  showmovmen2_butc  (cancel button callback) ********/

 void showmovmen2_butc(Widget iw_temp, XtPointer data, XtPointer call_data)

 {
 XtUnmanageChild(iw_showmovmen2);
 closefile(filedata);
 filedata = NULL;
 }




 /************************************************************/

 void showmovmen3(void)
 { 
 Widget iw_rowcol, iw_rowcol1, iw_radio, iw_dums;
 Arg    args[2];
 int    i;

 if (iw_showmovmen3 == (Widget)0)
    {   /* create movie 3D series menu widget first */

    iw_showmovmen3 = wid_dialog(iw_win, 0, "series options", -1, -1);
    iw_rowcol      = wid_rowcol(iw_showmovmen3, 'v', -1, -1);

    /* create label box for axis -----------------------------  axis  */
    iw_rowcol1  = wid_rowcol(iw_rowcol,  'h', -1, -1);
                  wid_labelg(iw_rowcol1, 0, "AXIS:   ", -1, -1);

    /* create toggle boxes for axes */
    iw_radio  = wid_rowcol(iw_rowcol1,  'h', -1, -1);
    XtSetArg(args[0], XmNradioBehavior, TRUE);
    XtSetValues(iw_radio, args, 1);

    iw_xaxis = wid_togmen(iw_radio,iw_xaxis,"X  ",xaxis,-1,-1);
    iw_yaxis = wid_togmen(iw_radio,iw_yaxis,"Y  ",yaxis,-1,-1);
    iw_zaxis = wid_togmen(iw_radio,iw_zaxis,"Z  ",zaxis,-1,-1);

    /* create scale widgets for col, row & slice ------------- scales */
    for (i=0; i < 6; iw_scales[i++]  = (Widget) 0)
        ;
    wid_win(iw_rowcol, &iw_scales[0], nsam, nrow, nslice);

    /* create toggle box for normalize -------------------- normalize */
    iw_norm = wid_togmen(iw_rowcol,0,"NORM. BY SLICE",normit,-1,-1);

    /* create box for apply ----------------------------------- apply */
    wid_stdbut(iw_rowcol, iw_showmovmen3, &iw_dums, &iw_push3c, 
               &iw_push3a,  "CA",
               fin_cb, showmovmen3_butc ,showmovmen3_buta, NULL);
    }
 else
    {
    /* update scale widgets for col, row & slice ------------- scales */
    wid_win(iw_rowcol, &iw_scales[0], nsam, nrow, nslice);
     }

 XtManageChild(iw_showmovmen3);
 }



 /***********  showmovmen3_buta  (accept button callback) *************/

 void showmovmen3_buta(Widget iw_temp, XtPointer data, XtPointer call_data)
 {

/* get nsam1 & nsam2 */
 XmScaleGetValue(iw_scales[0],&nsam1);
 XmScaleGetValue(iw_scales[1],&nsam2);
 if (nsam2 < nsam1)
    {
    spout("*** ENDING COL MUST BE > STARTING COL");
    return;
    }

 /* get nrow1 & nrow2 */
 XmScaleGetValue(iw_scales[2],&nrow1);
 XmScaleGetValue(iw_scales[3],&nrow2);
 if (nrow1 > nrow2)
    {
    spout("*** ENDING ROW MUST BE > STARTING ROW");
    return;
    }

 /* get nslice1 & nslice2 */
 XmScaleGetValue(iw_scales[4],&nslice1);
 XmScaleGetValue(iw_scales[5],&nslice2);
 if (nslice1 > nslice2)
    {
    spout("*** ENDING SLICE MUST BE > STARTING SLICE");
    return;
    }

 if (iw_norm > (Widget)0)
    normit = XmToggleButtonGadgetGetState(iw_norm);

 if (nslice > 1) 
   {
   xaxis = XmToggleButtonGadgetGetState(iw_xaxis);
   yaxis = XmToggleButtonGadgetGetState(iw_yaxis);
   zaxis = XmToggleButtonGadgetGetState(iw_zaxis);
   }

 /* remove the montage menu */
 XtUnmanageChild(iw_showmovmen3);
 expos_cb(NULL,NULL,NULL);

 axis = 'z';
 if (xaxis) axis = 'x';
 if (yaxis) axis = 'y';

 /* display the montage, imagego set in imagemen */
 mont3(filedata, imagego, imageend);

 showmoviemen(TRUE); 
 }



 /***********  showmovmen3_butc  (cancel button callback) ********/

 void showmovmen3_butc(Widget iw_temp, XtPointer data, XtPointer call_data)

 {
 XtUnmanageChild(iw_showmovmen3);
 closefile(filedata);
 filedata = NULL;
 }





/**********************  showmoviemen   **********************************/

void showmoviemen(int first)
   {   
   Widget iw_rowcol, iw_buts, iw_buta, iw_butc;
 
   if (iw_showmoviemen <= (Widget) 0)
      { /* movie menu widget does not exist, need to create it */
                                  
      iw_showmoviemen = wid_dialog(iw_win,0,"Movie options",20,20);
      iw_rowcol = wid_rowcol(iw_showmoviemen,'v',-1,-1);
                                                   
      /* set default direction, delay & starting frame */
      movie_twoways = FALSE;
      movie_inc     = 1;
      movie_delay   = 0;
      movie_frame   = 1;

      /* create toggle box for movie_twoways ------- movie_twoways */
      iw_twoways = wid_toggleg(iw_rowcol,0,"two-way",movie_twoways,    
                        toggle2_cb, (char *) &movie_twoways, -1, -1);

      /* create a scale widget for delay ------------------- delay */
      iw_delayb = wid_scale(iw_rowcol,iw_delayb,"delay (msecs):",
                           0, 1020, movie_delay, 250, 50, -1, -1);
			   
      /* create a push button for apply -------------------- apply */
      wid_stdbut(iw_rowcol, iw_showmoviemen,
                 &iw_buts, &iw_butc, &iw_buta, "SCA",
                 showmoviemen_buts, fin_cb, showmoviemen_buta, NULL);
      }                                                      

   /* display the movie menu */
   XtManageChild(iw_showmoviemen); 
   XFlush(idispl);

   } 
                                   

/**********  showmoviemen_buta    (accept button callback)  *************/
 
void showmoviemen_buta(Widget iw_t, XtPointer data, XtPointer call_data)
   {
   /* get delay */
   XmScaleGetValue(iw_delayb,&movie_delay);
                  
   /* get movie_twoways toggle */
   movie_twoways =  XmToggleButtonGadgetGetState(iw_twoways);

   movie_inc     = 1;

   XtUnmanageChild(iw_showmoviemen);

   /* start movie going */
   showmovie();
   } 

/*********** showmoviemen_buts (stop button callback) ******************/

void showmoviemen_buts(Widget iw_t, XtPointer data, XtPointer call_data)
    {
    /* remove the movie menu widget */

    int i;

    movie_inc = 1;

    XtUnmanageChild(iw_showmoviemen);
    showbutx(NULL, NULL, NULL, TRUE);

    /* remove all translations from iw_win */
    XtUninstallTranslations(iw_win);
    
    /* free the XImage memory */
    for (i=1; i<=ndone; i++)
         /* {XDestroyImage(ximagelist[i]); }   */
         {XFree((void *)ximagelist[i]);}
    
    if (cmont3buf) 
       {free(cmont3buf);  cmont3buf = NULL;}
    } 
